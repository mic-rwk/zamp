#include <iostream>
#include "Interp4Rotate.hh"

extern "C" {
    const char* GetCmdName() {return "Rotate"; }
    AbstractInterp4Command* CreateCmd(void);
}

AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Rotate::CreateCmd();
}

void Interp4Rotate::PrintCmd() const {
    std::cout << "Set command: " << GetCmdName() << "\n";
}

void Interp4Rotate::PrintSyntax() const {
    std::cout << "Rotate nazwa_obiektu nazwa_osi szybkosc_katowa kąt\n";
}

void Interp4Rotate::PrintParams() const {
    std::cout << "  Obiekt: " << objName << "\n";
    std::cout << "  Oś: " << axisName << "\n";
    std::cout << "  Prędkość kątowa: " << angularSpeed << "\n";
    std::cout << "  Kąt: " << angleDegree << "\n";
}

const char* Interp4Rotate::GetCmdName() const {
    return "Rotate";
}

bool Interp4Rotate::ExecCmd(AbstractScene &/*rScn*/, const char */*sMobObjName*/, AbstractComChannel &/*rComChann*/) {
    std::cout << "[Rotate] ExecCmd  dla obiektu " << objName << "\n";
    return true;
}

bool Interp4Rotate::ReadParams(std::istream &rStrm_CmdsList) {
    if (!(rStrm_CmdsList >> objName >> axisName >> angularSpeed >> angleDegree)) return false;
    return true;
}

AbstractInterp4Command* Interp4Rotate::CreateCmd()
{
  return new Interp4Rotate();
}

