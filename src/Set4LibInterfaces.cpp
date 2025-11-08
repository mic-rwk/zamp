#include "../inc/Set4LibInterfaces.hh"
#include <filesystem>

bool Set4LibInterfaces::AddLibrary(const std::string &libPath)
{
    if (!std::filesystem::exists(libPath)) {
        std::cerr << "[Set4LibInterfaces] Brak pliku: " << libPath << "\n";
        return false;
    }

    auto lib = std::make_shared<LibInterface>(libPath);
    if (!lib->LoadPlugin()) return false;

    std::string cmd = lib->GetCmdName();
    _libs[cmd] = lib;

    std::cout << "[Set4LibInterfaces] Załadowano: " << libPath
              << " (komenda: " << cmd << ")\n";
    return true;
}

std::shared_ptr<LibInterface> Set4LibInterfaces::Find(const std::string &cmdName)
{
    auto it = _libs.find(cmdName);
    return (it != _libs.end()) ? it->second : nullptr;
}

void Set4LibInterfaces::Print() const
{
    std::cout << "=== Załadowane biblioteki ===\n";
    for (auto &p : _libs)
        std::cout << " - " << p.first << "\n";
}
