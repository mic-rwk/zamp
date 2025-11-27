#include "../inc/LibInterface.hh"

bool LibInterface::LoadPlugin() {
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

void LibInterface::UnloadPlugin() {
    if (handle) {
        dlclose(handle);
        handle = nullptr;
    }
}