#include <iostream>
#include <dlfcn.h>
#include <cassert>
#include <memory>
#include <vector>
#include <filesystem>
#include "AbstractInterp4Command.hh"
#include "../inc/preprocessor.hh"
#include "../inc/LibInterface.hh"

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

int main(int argc, char *argv[])
{
  LoadPlugin();

  return 0;
}
