#include <cassert>
#include <dlfcn.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "AbstractInterp4Command.hh"
#include "../inc/preprocessor.hh"
#include "../inc/LibInterface.hh"
#include "../inc/CommandsParser.hh"
#include "../inc/ProgramInterpreter.hh"
#include "../inc/Configuration.hh"
#include "../inc/xmlinterp.hh"

int displayLibraryInfo(){

  void *pLibHnd_Set = dlopen("libInterp4Rotate.so",RTLD_LAZY);
  AbstractInterp4Command *(*pCreateCmd_Set)(void);
  void *pFun;

  if (!pLibHnd_Set) {
    std::cerr << "!!! Brak biblioteki: Interp4Rotate.so" << '\n';
    return 1;
  }


  pFun = dlsym(pLibHnd_Set,"CreateCmd");
  if (!pFun) {
    std::cerr << "!!! Nie znaleziono funkcji CreateCmd" << '\n';
    return 1;
  }
  pCreateCmd_Set = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun);


  AbstractInterp4Command *pCmd = pCreateCmd_Set();

  std::cout << '\n';
  std::cout << pCmd->GetCmdName() << '\n';
  std::cout << '\n';
  pCmd->PrintSyntax();
  std::cout << '\n';
  pCmd->PrintCmd();
  std::cout << '\n';
  
  delete pCmd;

  dlclose(pLibHnd_Set);

}

int runPreprocessor(int argc, char *argv[]){
  if (argc < 2) {
        std::cerr << "Użycie: " << argv[0] << " plik.cmd\n";
        return 1;
    }

    try {
        std::string output = RunPreprocessor(argv[1]);
        std::cout << "===== Wynik preprocesora =====\n";
        std::cout << output << "\n";
    } catch (const std::exception &e){
        std::cerr << "Błąd: " << e.what() << "\n";
        return 2;
    }
}

void LoadPlugin(){
  std::vector<std::string> pluginNames = {
          "./libs/libInterp4Set.so",
          "./libs/libInterp4Rotate.so",
          "./libs/libInterp4Pause.so",
          "./libs/libInterp4Move.so"
      };

      std::vector<std::unique_ptr<LibInterface>> libs;
      for (const auto &p : pluginNames) {
          if (!std::filesystem::exists(p)) {
              std::cout << "Plik wtyczki " << p << " nie istnieje — pomijam.\n";
              continue;
          }
          auto li = std::make_unique<LibInterface>(p);
          if (li->LoadPlugin()) {
              std::cout << "Załadowano wtyczkę: " << p << "  (komenda: " << li->GetCmdName() << ")\n";
              libs.push_back(std::move(li));
          } else {
              std::cerr << "Błąd ładowania wtyczki: " << p << "\n";
          }
      }

      for (const auto &lib : libs) {
          AbstractInterp4Command* cmd = lib->CreateCmd();
          std::cout << "\n=== Informacje o komendzie: " << cmd->GetCmdName() << " ===\n";
          cmd->PrintSyntax();
          std::cout << "\n";
          delete cmd;
      }
}

int main(int argc, char **argv)
{
    const char *xmlFile = "config/config.xml";
    if (argc > 1) {
        xmlFile = argv[1];
    }

    std::cout << "[MAIN] Uruchamianie programu z pliku: " << xmlFile << "\n";

    Configuration config;

    int Socket = 6217;
    ComChannel channel;
    channel.Init(Socket);
    channel.Connect("127.0.0.1", Socket);

    XMLInterp4Config xmlInterp(config);

    ProgramInterpreter program(config, channel);  

    if (!program.Read_XML_Config(xmlFile)) {
        std::cerr << "[MAIN] Błąd podczas wczytywania konfiguracji XML.\n";
        return 1;
    }

    auto fmt = [](const std::string &s) {
        std::string out;
        for (char c : s) out += (c == ' ' ? ',' : c);
        return out;
    };

    if (channel.GetSocket() < 0) {
        std::cerr << "[MAIN] Uwaga: brak połączenia do serwera graficznego - nie wysyłam konfiguracji.\n";
    } else {
        for (const auto &obj : config.objects) {
            std::ostringstream cmd;
            cmd << "AddObj Name=" << obj.name;
            if (!obj.rgb.empty()) cmd << " RGB=(" << fmt(obj.rgb) << ")";
            if (!obj.scale.empty()) cmd << " Scale=(" << fmt(obj.scale) << ")";
            if (!obj.shift.empty()) cmd << " Shift=(" << fmt(obj.shift) << ")";
            if (!obj.rot.empty()) cmd << " RotXYZ_deg=(" << fmt(obj.rot) << ")";
            if (!obj.trans.empty()) cmd << " Trans_m=(" << fmt(obj.trans) << ")";
            cmd << "\n";
            std::string s = cmd.str();
            std::cout << "[MAIN] Wysyłam konfigurację do serwera: " << s;
            if (!channel.Send(s)) {
                std::cerr << "[MAIN] Błąd wysyłania komendy konfiguracji do serwera.\n";
            }
        }
    }

        if (!program.ExecProgram(xmlFile)) {
            std::cerr << "[MAIN] Błąd podczas wykonywania programu.\n";
            return 2;
        }

    channel.Send("Clear \n");
    channel.Close();

    xercesc::XMLPlatformUtils::Terminate();

    std::cout << "\n[MAIN] Program zakończył się poprawnie\n";
    return 0;
}

