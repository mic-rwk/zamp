#include <iostream>
#include <sstream>
#include <memory>
#include <unistd.h>
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
  AbstractMobileObj * pObj = rScn.FindMobileObj(objName.c_str());
  if (!pObj) {
        std::cerr << "[Interp4Move] Nie znaleziono obiektu: " << objName.c_str() << "\n";
        return false;
    }

    double speed_m_s = _Speed_mmS;
    double distance_m = distance_mm;
    double yaw_deg = pObj->GetAng_Yaw_deg();
    double yaw_rad = yaw_deg * M_PI / 180.0;

    Vector3D pos = pObj->GetPositoin_m();

    double total_time_s = distance_m / speed_m_s;
    const double step_time_s = 0.05;
    int steps = static_cast<int>(total_time_s / step_time_s);

    double step_dist = speed_m_s * step_time_s;

    std::cout << "[Interp4Move] " << objName.c_str()
              << " porusza się z prędkością " << _Speed_mmS
              << " m/s przez " << distance_mm << " m ("
              << steps << " kroków)\n";

    for (int i = 0; i < steps; ++i) {
        pos[0] += step_dist * cos(yaw_rad);
        pos[1] += step_dist * sin(yaw_rad);
        {
            std::lock_guard<std::mutex> lock(rScn.scene_mutex);
            pObj->SetPosition_m(pos);
      }
        
        {
            std::lock_guard<std::mutex> lock(rComChann.comchann_mutex);
            std::ostringstream cmd;
            cmd << "UpdateObj Name=" << objName.c_str()
                << " Trans_m=(" << pos[0] << "," << pos[1] << "," << pos[2] << ")\n";

            rComChann.Send(cmd.str());
        }
        usleep(static_cast<useconds_t>(step_time_s * 1e6));
    }

    std::cout << "[Interp4Move] Zakończono ruch obiektu " << objName.c_str() << "\n";
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