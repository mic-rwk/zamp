#include "AbstractMobileObj.hh"

#ifndef MOBILE_OBJ_HH
#define MOBILE_OBJ_HH

class MobileObject : public AbstractMobileObj {

    double roll_degree{0.0};
    double pitch_degree{0.0};
    double yaw_degree{0.0};
    Vector3D position_3D;
    std::string name;

    public:

    MobileObject() = default;
    virtual ~MobileObject() = default;

    double GetAng_Pitch_deg() const{
        return pitch_degree;
    }

    double GetAng_Roll_deg() const{
        return roll_degree;
    }

    double GetAng_Yaw_deg() const {
        return yaw_degree;
    }

    void SetAng_Pitch_deg(double pitch) {
        pitch_degree = pitch;
    }

    void SetAng_Roll_deg(double roll){
        roll_degree = roll;
    }

    void SetAng_Yaw_deg(double yaw) {
        yaw_degree = yaw;
    }

    const Vector3D & GetPositoin_m() const {
        return position_3D;
    }

    void SetPosition_m(const Vector3D &rPos) {
        position_3D = rPos;
    }

    void SetName(const char* sName) {
        name = sName;
    }

    const std::string & GetName() const {
        return name;
    }
};

#endif
