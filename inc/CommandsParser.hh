#ifndef COMMANDSPARSER_HH
#define COMMANDSPARSER_HH

#include <string>
#include <iostream>
#include <istream>
#include <sstream>
#include <vector>

struct CommandData {
    std::string name;
    std::string params;

    CommandData(const std::string &cmdName, const std::string &cmdParams)
        : name(cmdName), params(cmdParams) {}

    std::istringstream GetParamStream() const {
        return std::istringstream(params);
    }
};

class CommandsParser{

    private:
        std::vector<CommandData> commands;

    public:
    CommandsParser() = default;

    bool ReadCommandsList(std::istream & inputStream);
    bool ReadCommand_Move(std::istream & inputStream, const std::string & commandName);
    bool ReadCommand_Rotate(std::istream & inputStream, const std::string & commandName);
    bool ReadCommand_Pause(std::istream & inputStream, const std::string & commandName);
    bool ReadCommand_Set(std::istream & inputStream, const std::string & commandName);

    const std::vector<CommandData> GetCommands() const {return commands; };
};




#endif