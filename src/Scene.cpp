#include "../inc/Scene.hh"
#include "../inc/AbstractMobileObj.hh"

void Scene::SetMobileObjs(const std::map<std::string, std::shared_ptr<AbstractMobileObj>> & objs) {
    _mobileObjs = objs;
}


AbstractMobileObj * Scene::FindMobileObj(const char * sName) {
    auto it = _mobileObjs.find(sName);
    return it != _mobileObjs.end() ? it->second.get() : nullptr;
}

void Scene::AddMobileObj(AbstractMobileObj *pMobObj) {
        if (!pMobObj) {
            std::cerr << "[Scene] Próba dodania pustego obiektu!\n";
            return;
        }

        _mobileObjs[pMobObj->GetName()] = std::shared_ptr<AbstractMobileObj>(pMobObj);
        std::cout << "[Scene] Dodano obiekt: " << pMobObj->GetName() << "\n";
}

void Scene::PrintObjs() const {
        std::cout << "=== Obiekty w scenie ===\n";
        for (const auto &p : _mobileObjs)
            std::cout << " - " << p.first << "\n";
}