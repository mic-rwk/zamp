#include <iostream>
#include "Interp4Move.hh"

extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Move"; }
}


/*!
 * \brief
 *
 *
 */
AbstractInterp4Command* CreateCmd(void) {
  return Interp4Move::CreateCmd();
}

/*!
 *
 */
Interp4Move::Interp4Move(): objName(""), _Speed_mmS(0), distance_mm(0)
{}

/*!
 *
 */
void Interp4Move::PrintCmd() const {
  std::cout << GetCmdName() << " " << objName << " " << _Speed_mmS  << " " << distance_mm << "\n";
}

/*!
 *
 */
const char* Interp4Move::GetCmdName() const {
  return ::GetCmdName();
}

/*!
 *
 */
bool Interp4Move::ExecCmd( AbstractScene      &rScn, 
                           const char         *sMobObjName,
			   AbstractComChannel &rComChann
			 )
{
  std::cout << "[Move] ExecCmd (symulacja) dla obiektu " << objName  << "\n";
  return true;
}


/*!
 *
 */
bool Interp4Move::ReadParams(std::istream& Strm_CmdsList) {
  if (!(Strm_CmdsList >> objName >> _Speed_mmS >> distance_mm)) return false;
  return true;
}


/*!
 *
 */
AbstractInterp4Command* Interp4Move::CreateCmd() {
  return new Interp4Move();
}

 /*!
 *
 */
void Interp4Move::PrintSyntax() const {
  std::cout << "\tMove  NazwaObiektu  Szybkosc[m/s]  DlugoscDrogi[m]" << "\n";
}

void Interp4Move::PrintParams() const {
  std::cout << "  Obiekt: " << objName << "\n";
  std::cout << "  Szybkość: " << _Speed_mmS << " m/s\n";
  std::cout << "  Długość drogi: " << distance_mm << " m\n";
}