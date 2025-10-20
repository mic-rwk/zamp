#include <iostream>
#include <dlfcn.h>
#include <cassert>
#include "AbstractInterp4Command.hh"

int main()
{
  void *pLibHnd_Set = dlopen("libInterp4Set.so",RTLD_LAZY);
  AbstractInterp4Command *(*pCreateCmd_Set)(void);
  void *pFun;

  if (!pLibHnd_Set) {
    std::cerr << "!!! Brak biblioteki: Interp4Set.so" << '\n';
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

// #include <bits/stdc++.h>
// #include <cstdio>
// #include <memory>
// #include <array>
// #include <vector>
// #include <sstream>
// #include <filesystem>
// #include <dlfcn.h>

// #include "../inc/LibInterface.hh"
// #include "../inc/AbstractInterp4Command.hh"

// static std::string RunPreprocessor(const std::string &filename) {
//     // Używa systemowego cpp -P (usuwa linie z numerami linii)
//     // Jeśli cpp nie jest dostępny, użytkownik zobaczy błąd.
//     std::array<char, 4096> buffer;
//     std::string cmd = "cpp -P " + filename;
//     std::string result;
//     FILE *pipe = popen(cmd.c_str(), "r");
//     if (!pipe) {
//         throw std::runtime_error("Nie mogę uruchomić cpp. Sprawdź czy jest zainstalowany.");
//     }
//     try {
//         while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
//             result += buffer.data();
//         }
//     } catch (...) {
//         pclose(pipe);
//         throw;
//     }
//     int rc = pclose(pipe);
//     if (rc != 0) {
//         // Preprocesor mógł zwrócić kod błędu, ale nadal zwrócimy tekst (może być część wyników)
//     }
//     return result;
// }

// static std::vector<std::string> SplitTokens(const std::string &line) {
//     std::istringstream iss(line);
//     std::vector<std::string> tokens;
//     std::string t;
//     while (iss >> t) tokens.push_back(t);
//     return tokens;
// }

// int main(int argc, char** argv) {
//     std::cout << "Program - Etap0 + Etap1 (przykład)\n";
//     if (argc != 3) {
//         std::cerr << "Użycie: " << argv[0] << " opis_dzialan.cmd konfiguracja.xml\n";
//         return 1;
//     }
//     std::string cmdFile = argv[1];
//     std::string xmlFile = argv[2];

//     try {
//         std::string preproc = RunPreprocessor(cmdFile);
//         std::cout << "====== Zawartość pliku po przetworzeniu preprocesorem (cpp -P) ======\n";
//         std::cout << preproc << "\n";
//         std::cout << "=====================================================================\n";

//         // --- ładowanie wtyczek (tu: próbujemy załadować cztery biblioteki, jeśli istnieją)
//         std::vector<std::string> pluginNames = {
//             "./libSet.so",
//             "./libMove.so",
//             "./libRotate.so",
//             "./libPause.so"
//         };

//         std::vector<LibInterface> libs;
//         for (auto &p : pluginNames) {
//             if (!std::filesystem::exists(p)) {
//                 std::cout << "Plik wtyczki " << p << " nie istnieje — pomijam.\n";
//                 continue;
//             }
//             LibInterface li(p);
//             if (li.Load()) {
//                 std::cout << "Załadowano wtyczkę: " << p << "  (komenda: " << li.GetCmdName() << ")\n";
//                 libs.push_back(std::move(li));
//             } else {
//                 std::cerr << "Błąd ładowania wtyczki: " << p << "\n";
//             }
//         }

//         // Prost y parser linii: każda linia to jedno polecenie (nazwapolecenia param1 param2 ...)
//         std::istringstream iss(preproc);
//         std::string line;
//         size_t lineNo = 0;
//         while (std::getline(iss, line)) {
//             ++lineNo;
//             // usuń początkowe/końcowe spacje
//             auto start = line.find_first_not_of(" \t\r\n");
//             if (start == std::string::npos) continue; // pusta linia
//             auto end = line.find_last_not_of(" \t\r\n");
//             std::string trimmed = line.substr(start, end - start + 1);
//             if (trimmed.empty()) continue;
//             // tokeny
//             auto tokens = SplitTokens(trimmed);
//             if (tokens.empty()) continue;
//             std::string cmdName = tokens[0];

//             // szukamy wtyczki o takiej nazwie
//             bool found = false;
//             for (auto &li : libs) {
//                 const char* pluginCmdName = li.GetCmdName();
//                 if (pluginCmdName && cmdName == pluginCmdName) {
//                     found = true;
//                     // utwórz instancję polecenia i przekaż strumień z pozostałymi tokenami
//                     std::unique_ptr<AbstractInterp4Command> cmd(li.CreateCmd());
//                     if (!cmd) {
//                         std::cerr << "Wtyczka zwróciła null z CreateCmd()\n";
//                         break;
//                     }
//                     // zrekonstrukuj strumień parametrów z pozostałej części linii
//                     std::string rest;
//                     if (tokens.size() > 1) {
//                         for (size_t i = 1; i < tokens.size(); ++i) {
//                             if (i > 1) rest += " ";
//                             rest += tokens[i];
//                         }
//                     }
//                     std::istringstream paramsStream(rest);
//                     bool ok = cmd->ReadParams(paramsStream);
//                     if (!ok) {
//                         std::cerr << "Błąd wczytywania parametrów dla polecenia " << cmdName << " w linii " << lineNo << "\n";
//                     } else {
//                         std::cout << "Linia " << lineNo << " - polecenie: " << cmdName << "\n";
//                         cmd->PrintParams();
//                     }
//                     break;
//                 }
//             }
//             if (!found) {
//                 std::cout << "Linia " << lineNo << ": nieznane polecenie: '" << cmdName << "' — pomijam.\n";
//             }
//         }

//         // Zwolnienie wtyczek nastąpi w destruktorach LibInterface
//     } catch (const std::exception &ex) {
//         std::cerr << "Wyjątek: " << ex.what() << "\n";
//         return 2;
//     }

//     return 0;
// }

