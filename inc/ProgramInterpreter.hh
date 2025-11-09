#ifndef PROGRAMINTERPRETER_HH
#define PROGRAMINTERPRETER_HH

#include "Scene.hh"
#include "Set4LibInterfaces.hh"
#include "ComChannel.hh"
#include "CommandsParser.hh"
#include "../inc/Configuration.hh"
#include <string>

class ProgramInterpreter {
    Scene _Scn;
    Set4LibInterfaces _LibManager;
    ComChannel _Chann2Serv;
    Configuration & _xmlConfig;

public:
    explicit ProgramInterpreter(Configuration & config) 
    : _xmlConfig(config) {}
    bool Read_XML_Config(const char *fileName);
    bool ExecProgram(const char *fileName_Prog);
};

#endif
