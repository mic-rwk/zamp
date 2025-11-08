#include "../inc/ProgramInterpreter.hh"
#include "preprocessor.hh"
#include <fstream>
#include <sstream>
#include <iostream>

bool ProgramInterpreter::ExecProgram(const char *fileName_Prog)
{
    std::string preprocessed = RunPreprocessor(fileName_Prog);
    std::istringstream cmdStream(preprocessed);

    CommandsParser parser;
    if (!parser.ReadCommandsList(cmdStream)) {
        std::cerr << "[ProgInterp] Błąd podczas parsowania komend.\n";
        return false;
    }

    for (const auto &cmd : parser.GetCommands()) {
        std::string libName = "libs/libInterp4" + cmd.name + ".so";

        auto lib = _LibManager.Find(cmd.name);
        if (!lib) {
            if (!_LibManager.AddLibrary(libName))
                continue;
            lib = _LibManager.Find(cmd.name);
        }

        auto command = lib->CreateCmd();
        if(!command) {
            std::cerr << "[ProgramInterpreter] Nie udało się utworzyć komendy: " << cmd.name;
            continue;
        }

        std::istringstream paramStream(cmd.params);
        if (!command->ReadParams(paramStream)) {
            std::cerr << "[ProgramInterpreter] Błąd parametrów dla komendy: " << cmd.name << "\n";
            continue;
        }

        command->PrintParams();

        if (!command->ExecCmd(_Scn, "ObiektA", _Chann2Serv)) {
            std::cerr << "[ProgramInterpreter] ExecCmd nie powiódł się dla " << cmd.name << "\n";
        }
    }
    return true;
}
