#include "../inc/preprocessor.hh"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <sstream>

std::string RunPreprocessor(const std::string &filename)
{
    std::string cmd = "cpp -P " + filename;
    std::array<char, 4096> buffer;
    std::string result;

    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("Nie mogę uruchomić preprocesora cpp");
    }

    while (fgets(buffer.data(), buffer.size(), pipe)) {
        result += buffer.data();
    }

    pclose(pipe);

    return result;
}