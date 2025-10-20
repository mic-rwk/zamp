#include <iostream>
#include <dlfcn.h>
#include <cassert>
#include "AbstractInterp4Command.hh"

int main()
{
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
