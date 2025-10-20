#include <iostream>
#include "Interp4Set.hh"

extern "C" {
    const char* GetCmdName() {return "Set"; }
    AbstractInterp4Command* CreateCmd(void);
}

AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Set::CreateCmd();
}

void Interp4Set::PrintCmd() const {
    std::cout << "Set command: " << GetCmdName() << "\n";
}

void Interp4Set::PrintSyntax() const {
    std::cout << "Set nazwa_obiektu wsp_x wsp_y wsp_z kat_OX kat_OY kat_OZ\n";
}

void Interp4Set::PrintParams() const {
    std::cout << "  Obiekt: " << objName << "\n";
    std::cout << "  Pozycja: (" << x << ", " << y << ", " << z << ")\n";
    std::cout << "  Kąty: (" << angX << "°, " << angY << "°, " << angZ << "°)\n";
}

const char* Interp4Set::GetCmdName() const {
    return "Set";
}

bool Interp4Set::ExecCmd(AbstractScene &/*rScn*/, const char */*sMobObjName*/, AbstractComChannel &/*rComChann*/) {
    // W etapie 1 nie wykonujemy wysyłania do serwera — tylko symulacja
    std::cout << "[Set] ExecCmd (symulacja) dla obiektu " << objName << "\n";
    return true;
}

bool Interp4Set::ReadParams(std::istream &rStrm_CmdsList) {
    // oczekujemy: nazwa x y z angX angY angZ
    if (!(rStrm_CmdsList >> objName)) return false;
    if (!(rStrm_CmdsList >> x >> y >> z >> angX >> angY >> angZ)) {
        return false;
    }
    return true;
}

AbstractInterp4Command* Interp4Set::CreateCmd()
{
  return new Interp4Set();
}

