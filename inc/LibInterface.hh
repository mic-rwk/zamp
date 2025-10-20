#pragma once
#include <string>
#include <dlfcn.h>
#include <iostream>
#include "AbstractInterp4Command.hh"

using CreateCmdFunc = AbstractInterp4Command* (*)();
using GetCmdNameFunc = const char* (*)();

class LibInterface {

public:
    std::string libPath;
    void* handle {nullptr};
    CreateCmdFunc CreateCmd {nullptr};
    GetCmdNameFunc GetCmdName {nullptr};

    LibInterface() = default;
    LibInterface(const std::string &path): libPath(path) {}

    ~LibInterface() {
        UnloadPlugin();
        CreateCmd = nullptr;
        GetCmdName = nullptr;
    }

    bool LoadPlugin();
    void UnloadPlugin();
};
