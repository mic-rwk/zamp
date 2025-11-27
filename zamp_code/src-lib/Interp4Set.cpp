#include <iostream>
#include "../inc/Interp4Set.hh"

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
    std::cout << "\tSet nazwa_obiektu wsp_x wsp_y wsp_z kat_OX kat_OY kat_OZ\n";
}

void Interp4Set::PrintParams() const {
    std::cout << "  Obiekt: " << objName << "\n";
    std::cout << "  Pozycja: (" << x << ", " << y << ", " << z << ")\n";
    std::cout << "  Kąty: (" << angX << "°, " << angY << "°, " << angZ << "°)\n";
}

const char* Interp4Set::GetCmdName() const {
    return ::GetCmdName();
}

bool Interp4Set::ExecCmd(AbstractScene & rScn, const char * sMobObjName, AbstractComChannel & rComChann) {
    AbstractMobileObj * pObj = rScn.FindMobileObj(objName.c_str());
    if (!pObj) {
        AbstractMobileObj * newObj = rScn.SetMobileObj(objName.c_str());
        newObj->SetName(objName.c_str());
        {
            std::lock_guard<std::mutex> lock(rScn.scene_mutex);
            rScn.AddMobileObj(newObj);
            pObj = rScn.FindMobileObj(objName.c_str());
            if (!pObj) {
                std::cerr << "[Interp4Set] Błąd wewnętrzny: nie można znaleźć właśnie utworzonego obiektu: " 
                        << objName.c_str() << "\n";
                return false;
            }
        }
    }
        
        {
            std::lock_guard<std::mutex> lock(rComChann.comchann_mutex);
            std::ostringstream addCmd;
            addCmd << "AddObj Name=" << objName
                << " RGB=(128,128,128)"
                << " Scale=(1,1,1)"
                << " Shift=(0,0,0)"
                << " RotXYZ_deg=(" << angX << "," << angY << "," << angZ << ")"
                << " Trans_m=(" << x << "," << y << "," << z << ")\n";
        if (!rComChann.Send(addCmd.str())) {
            std::cerr << "[Interp4Set] Ostrzeżenie: nie udało się wysłać AddObj do serwera dla: " << objName << "\n";
        }
    }

    Vector3D newPos;
    newPos[0] = x;
    newPos[1] = y;
    newPos[2] = z;
    pObj->SetPosition_m(newPos);
    pObj->SetAng_Roll_deg(angX);
    pObj->SetAng_Pitch_deg(angY);
    pObj->SetAng_Yaw_deg(angZ);

    {
        std::lock_guard<std::mutex> lock(rComChann.comchann_mutex);
        std::ostringstream cmd;
        cmd << "UpdateObj Name=" << objName.c_str()
            << " Trans_m=(" << x << "," << y << "," << z << ")"
            << " RotXYZ_deg=(" << angX << "," << angY << "," << angZ << ")\n";
    if (!rComChann.Send(cmd.str())) {
        std::cerr << "[ComChannel] Błąd wysyłania danych przez socket.\n";
        return false;
    }
    std::cout << "[Interp4Set] Wysłano: " << cmd.str();
}
    
    return true;
}

bool Interp4Set::ReadParams(std::istream &rStrm_CmdsList) {
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

