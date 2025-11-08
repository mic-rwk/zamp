#ifndef SET4LIBINTERFACES_HH
#define SET4LIBINTERFACES_HH

#include "LibInterface.hh"
#include <map>
#include <memory>
#include <string>
#include <iostream>

class Set4LibInterfaces {
    std::map<std::string, std::shared_ptr<LibInterface>> _libs;

public:
    bool AddLibrary(const std::string &libPath);
    std::shared_ptr<LibInterface> Find(const std::string &cmdName);
    void Print() const;
};

#endif
