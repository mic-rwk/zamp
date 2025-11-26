#ifndef PROGRAMINTERPRETER_HH
#define PROGRAMINTERPRETER_HH

#include "Scene.hh"
#include "Set4LibInterfaces.hh"
#include "ComChannel.hh"
#include "CommandsParser.hh"
#include "../inc/Configuration.hh"
#include <string>
#include <mutex>
#include <list>
#include <thread>

class ProgramInterpreter {
    Set4LibInterfaces _LibManager;
    Scene _Scn;
    ComChannel & _Chann2Serv;
    Configuration & _xmlConfig;

public:
    explicit ProgramInterpreter(Configuration & config, ComChannel & channel)
    : _Chann2Serv(channel), _xmlConfig(config) {}
    bool Read_XML_Config(const char *fileName);
    bool ExecProgram(const char *fileName_Prog);

    void ExecSingleCommand(CommandData cmd,
                       Scene &scn,
                       ComChannel &chan,
                       Set4LibInterfaces &_LibManager);
};

#endif
