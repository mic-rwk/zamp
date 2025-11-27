#include "../inc/Scene.hh"
#include "../inc/AbstractMobileObj.hh"

AbstractMobileObj * Scene::SetMobileObj(const std::string & name) {
    AbstractMobileObj * obj = new MobileObject();
    obj->SetName(name.c_str());
    return obj;
}


AbstractMobileObj * Scene::FindMobileObj(const char * sName) {
    if (!sName) return nullptr;
    auto it = _mobileObjs.find(sName);
    return it != _mobileObjs.end() ? it->second.get() : nullptr;
}

void Scene::AddMobileObj(AbstractMobileObj *pMobObj) {
        if (!pMobObj) {
            std::cerr << "[Scene] Próba dodania pustego obiektu!\n";
            return;
        }

        auto deleter = [](AbstractMobileObj *p){
            std::cout << "[Scene] deleter dla ptr=" << p << " name='"
                      << (p ? p->GetName() : std::string("(null)")) << "'\n";
            delete p;
        };

        _mobileObjs[pMobObj->GetName()] = std::shared_ptr<AbstractMobileObj>(pMobObj, deleter);
        std::cout << "[Scene] Dodano obiekt: " << pMobObj->GetName() << " ptr=" << pMobObj << "\n";
}

void Scene::PrintObjs() const {
        std::cout << "=== Obiekty w scenie ===\n";
        for (const auto &p : _mobileObjs)
            std::cout << " - " << p.first << "\n";
}

Scene::~Scene() {
    std::cout << "[Scene] Destruktor sceny - zaczynam czyszczenie (" << _mobileObjs.size() << " obiektow)\n";
    for (auto it = _mobileObjs.begin(); it != _mobileObjs.end(); ) {
        const std::string &n = it->first;
        auto &sp = it->second;
        std::cout << "[Scene] Niszcze obiekt: '" << n << 
                    "' ptr=" << sp.get() << " use_count=" << sp.use_count() << "\n";
        sp.reset();
        std::cout << "[Scene] Zniszczono obiekt: '" << n << "'\n";
        it = _mobileObjs.erase(it);
        std::cout << "[Scene] Usuwam obiekt z mapy: '" << n << "'\n";
    }
    std::cout << "[Scene] Destruktor sceny - koniec\n";
}