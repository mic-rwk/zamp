#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

#include <string>
#include <vector>

struct PluginInfo {
    std::string libName;
};

struct Object3D {
    std::string name;
    std::string shift = "0 0 0";
    std::string scale = "1 1 1";
    std::string rot = "0 0 0";
    std::string trans = "0 0 0";
    std::string rgb = "128 128 128";
};

class Configuration {
public:
    std::vector<PluginInfo> plugins;
    std::vector<Object3D> objects;

    void AddPlugin(const std::string & lib) { plugins.push_back({lib}); }
    void AddObject(const Object3D & obj) { objects.push_back(obj); }
};

#endif
