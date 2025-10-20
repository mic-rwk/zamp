#include <iostream>
#include "Interp4Pause.hh"

extern "C" {
    const char* GetCmdName() {return "Pause"; }
    AbstractInterp4Command* CreateCmd(void);
}

AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Pause::CreateCmd();
}

void Interp4Pause::PrintCmd() const {
    std::cout << "Set command: " << GetCmdName() << "\n";
}

void Interp4Pause::PrintSyntax() const {
    std::cout << "Pause stop_time_ms\n";
}

void Interp4Pause::PrintParams() const {
    std::cout << "  Czas: " << time_ms << " ms\n";
}

const char* Interp4Pause::GetCmdName() const {
    return "Pause";
}

bool Interp4Pause::ExecCmd(AbstractScene &/*rScn*/, const char */*sMobObjName*/, AbstractComChannel &/*rComChann*/) {
    // W etapie 1 nie wykonujemy wysyłania do serwera — tylko symulacja
    std::cout << "[Pause] ExecCmd na " << time_ms << " ms\n";
    return true;
}

bool Interp4Pause::ReadParams(std::istream &rStrm_CmdsList) {
    // oczekujemy: czas_ms
    if (!(rStrm_CmdsList >> time_ms)) return false;
    return true;
}

AbstractInterp4Command* Interp4Pause::CreateCmd()
{
  return new Interp4Pause();
}

