#pragma once
#include <string>
#include <dlfcn.h>
#include <iostream>
#include "inc/AbstractInterp4Command.hh"

// Typy funkcji, które powinna udostępniać wtyczka
using CreateCmdFunc = AbstractInterp4Command* (*)();
using GetCmdNameFunc = const char* (*)();

struct LibInterface {
    std::string libPath;
    void* handle {nullptr};
    CreateCmdFunc CreateCmd {nullptr};
    GetCmdNameFunc GetCmdName {nullptr};

    LibInterface() = default;
    LibInterface(const std::string &path): libPath(path) {}

    bool Load() {
        handle = dlopen(libPath.c_str(), RTLD_LAZY);
        if (!handle) {
            std::cerr << "dlopen failed for " << libPath << ": " << dlerror() << "\n";
            return false;
        }
        dlerror();
        CreateCmd = reinterpret_cast<CreateCmdFunc>(dlsym(handle, "CreateCmd"));
        const char* dlsym_error = dlerror();
        if (dlsym_error) {
            std::cerr << "Cannot load symbol CreateCmd: " << dlsym_error << "\n";
            dlclose(handle);
            handle = nullptr;
            return false;
        }
        GetCmdName = reinterpret_cast<GetCmdNameFunc>(dlsym(handle, "GetCmdName"));
        dlsym_error = dlerror();
        if (dlsym_error) {
            std::cerr << "Cannot load symbol GetCmdName: " << dlsym_error << "\n";
            dlclose(handle);
            handle = nullptr;
            return false;
        }
        return true;
    }

    void Unload() {
        if (handle) {
            dlclose(handle);
            handle = nullptr;
        }
    }

    ~LibInterface() {
        Unload();
    }
};
