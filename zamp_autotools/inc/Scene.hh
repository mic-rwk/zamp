#ifndef SCENE_HH
#define SCENE_HH

#include "AbstractScene.hh"
#include "AbstractMobileObj.hh"
#include "MobileObject.hh"
#include <map>
#include <memory>
#include <string>
#include <iostream>

class Scene : public AbstractScene {
    std::map<std::string, std::shared_ptr<AbstractMobileObj>> _mobileObjs;

public:
    Scene() = default;
    ~Scene() override;

    AbstractMobileObj * SetMobileObj(const std::string & name);
    AbstractMobileObj * FindMobileObj(const char *sName) override;
    void AddMobileObj(AbstractMobileObj *pMobObj) override;
    void PrintObjs() const;
};

#endif
