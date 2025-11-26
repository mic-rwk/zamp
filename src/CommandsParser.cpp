#include "../inc/CommandsParser.hh"

#define STR(x)  #x

#define BIBLIOTEKA(commandName) "Interp4##commandName"

#define IF_CMD_THEN_READ(commandOption)           \
  if (commandName == STR(commandOption)) {        \
      if(!ReadCommand_##commandOption(iss, commandName)) \
        return false;                               \
    continue;                                   \
  }                                                 \

bool CommandsParser::ReadCommandsList(std::istream & inputStream){
    std::string line;
    bool inParallelBlock = false;

    while (std::getline(inputStream, line)) {

      if(line == "Begin_Parallel_Actions" && !inParallelBlock) {
          inParallelBlock = true;
          commands.emplace_back("Begin_Parallel_Actions", "", CmdType::ParallelBegin);
          continue;
      }

      if(line == "End_Parallel_Actions" && inParallelBlock){ 
          inParallelBlock = false;
          commands.emplace_back("End_Parallel_Actions", "", CmdType::ParallelEnd);
          continue;
      }

      std::istringstream iss(line);
      std::string commandName;
      iss >> commandName;
      std::cout << "Przetwarzanie polecenia: " << commandName << "\n";
      IF_CMD_THEN_READ(Move);
      IF_CMD_THEN_READ(Rotate);
      IF_CMD_THEN_READ(Set);
      IF_CMD_THEN_READ(Pause);

      return false;
    }

    return true;
}

bool CommandsParser::ReadCommand_Move(std::istream & inputStream, const std::string & commandName) {
  std::string rest;
  std::getline(inputStream, rest);
  commands.emplace_back(commandName, rest, CmdType::Normal);
  return true;
}

bool CommandsParser::ReadCommand_Rotate(std::istream &inputStream, const std::string &commandName) {
    std::string rest;
    std::getline(inputStream, rest);
    commands.emplace_back(commandName, rest, CmdType::Normal);
    return true;
}

bool CommandsParser::ReadCommand_Set(std::istream &inputStream, const std::string &commandName) {
    std::string rest;
    std::getline(inputStream, rest);
    commands.emplace_back(commandName, rest, CmdType::Normal);
    return true;
}

bool CommandsParser::ReadCommand_Pause(std::istream &inputStream, const std::string &commandName) {
    std::string rest;
    std::getline(inputStream, rest);
    commands.emplace_back(commandName, rest, CmdType::Normal);
    return true;
}