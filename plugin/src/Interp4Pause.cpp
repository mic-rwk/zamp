#include <iostream>
#include "Interp4Pause.hh"
#include <thread>

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
    std::cout << "\tPause stop_time[ms]\n";
}

void Interp4Pause::PrintParams() const {
    std::cout << "  Czas: " << time_ms << " ms\n";
}

const char* Interp4Pause::GetCmdName() const {
    return ::GetCmdName();
}

bool Interp4Pause::ExecCmd(AbstractScene &/*rScn*/, const char */*sMobObjName*/, AbstractComChannel &/*rComChann*/) {
    std::cout << "[Interp4Pause] Pauza " << time_ms << " ms.\n";
    std::this_thread::sleep_for(std::chrono::milliseconds((int)time_ms));
    return true;
}

bool Interp4Pause::ReadParams(std::istream &rStrm_CmdsList) {
    if (!(rStrm_CmdsList >> time_ms)) return false;
    return true;
}

AbstractInterp4Command* Interp4Pause::CreateCmd()
{
  return new Interp4Pause();
}

