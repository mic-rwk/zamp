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
    std::cout << "\tRotate nazwa_obiektu nazwa_osi szybkosc_katowa[rd/s] kąt[stopnie]\n";
}

void Interp4Rotate::PrintParams() const {
    std::cout << "  Obiekt: " << objName << "\n";
    std::cout << "  Oś: " << axisName << "\n";
    std::cout << "  Prędkość kątowa: " << angularSpeed << "\n";
    std::cout << "  Kąt: " << angleDegree << "\n";
}

const char* Interp4Rotate::GetCmdName() const {
    return ::GetCmdName();
}

bool Interp4Rotate::ExecCmd(AbstractScene & rScn, const char * sMobObjName, AbstractComChannel & rComChann) {
    
    AbstractMobileObj *pMob = rScn.FindMobileObj(objName.c_str());
    if (!pMob) {
        std::cerr << "[Interp4Rotate] Brak obiektu o nazwie: " << objName << "\n";
        return false;
    }

    std::cout << "[Interp4Rotate] Obrót obiektu: " << objName
         << " o kąt " << angleDegree << " wokół osi " << axisName
         << " z prędkością " << angularSpeed << "°/s\n";

    double step_deg = (angularSpeed > 0 ? 1.0 : -1.0);
    double sleep_us = (1e6 * fabs(step_deg) / fabs(angularSpeed));
    double rotated = 0.0;

    double roll = pMob->GetAng_Roll_deg();
    double pitch = pMob->GetAng_Pitch_deg();
    double yaw = pMob->GetAng_Yaw_deg();

    while (fabs(rotated) < fabs(angleDegree)) {
        if (axisName == "OX")
            roll += step_deg;
        else if (axisName == "OY")
            pitch += step_deg;
        else if (axisName == "OZ")
            yaw += step_deg;

        {
            std::lock_guard<std::mutex> lock(rScn.scene_mutex);
            pMob->SetAng_Roll_deg(roll);
            pMob->SetAng_Pitch_deg(pitch);
            pMob->SetAng_Yaw_deg(yaw);
        }

        {
            std::lock_guard<std::mutex> lock(rComChann.comchann_mutex);
            std::ostringstream cmd;
            cmd << "UpdateObj Name=" << objName
                << " RotXYZ_deg=(" << roll << "," << pitch << "," << yaw << ")\n";
            rComChann.Send(cmd.str());
        }

        usleep((useconds_t)sleep_us);
        rotated += step_deg;
    }

    std::cout << "[Interp4Rotate] Zakończono obrót obiektu: " << objName << "\n";
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

