#include "../inc/ProgramInterpreter.hh"
#include "../inc/xmlinterp.hh"
#include "../inc/Configuration.hh"
#include "preprocessor.hh"
#include <fstream>
#include <sstream>
#include <iostream>
#include <thread>
#include <list>

#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>

using namespace xercesc;

bool ProgramInterpreter::ExecProgram(const char *fileName_Prog)
{
    std::cout << "\n[ExecProgram] Uruchamianie programu z pliku: " << fileName_Prog << "\n";

    std::string preprocessed;
    try {
        preprocessed = RunPreprocessor("komendy.cmd");
    } catch (const std::exception &e) {
        std::cerr << "[ExecProgram] Błąd podczas uruchamiania preprocesora: " << e.what() << "\n";
        return false;
    }

    CommandsParser parser;
    std::istringstream cmdStream(preprocessed);
    if (!parser.ReadCommandsList(cmdStream)) {
        std::cerr << "[ExecProgram] Błąd podczas parsowania komend.\n";
        return false;
    }

    std::cout << "[ExecProgram] Ładowanie pluginów z XML\n";

    for (const auto &p : _xmlConfig.plugins) {
        std::string fullPath = "libs/" + p.libName;

        if (!_LibManager.AddLibrary(fullPath)) {
            std::cerr << "[ExecProgram] Błąd ładowania biblioteki: " << fullPath << "\n";
        } else {
            std::cout << "[ExecProgram] Załadowano: " << fullPath << "\n";
        }
    }

    std::list<CommandData> parallelBlock;
    bool inParallel = false;

    for (const auto &cmd : parser.GetCommands()) {

        if (cmd.type == CmdType::ParallelBegin) {
            inParallel = true;
            parallelBlock.clear();
            continue;
        } else if (cmd.type == CmdType::ParallelEnd) {
            inParallel = false;
            std::vector<std::thread> threads(10);
            for (const auto &pCmd : parallelBlock) {
                threads.emplace_back(
                    &ProgramInterpreter::ExecSingleCommand,
                    this,
                    pCmd,
                    std::ref(_Scn),
                    std::ref(_Chann2Serv),
                    std::ref(_LibManager)
                );
        }
            for (auto &t : threads) {
                if (t.joinable()) {
                    t.join();
                }
            }
            continue;
        } else if(!inParallel) {

        std::string cmdName = cmd.name;

        auto lib = _LibManager.Find(cmdName);
        if (!lib) {
                std::cerr << "[ExecProgram] Nie udało się załadować biblioteki dla: " << cmdName << "\n";
                continue;
        }

        auto command = lib->CreateCmd();
        if (!command) {
            std::cerr << "[ExecProgram] Nie udało się utworzyć komendy: " << cmdName << "\n";
            continue;
        }

        std::istringstream paramStream(cmd.params);
        if (!command->ReadParams(paramStream)) {
            std::cerr << "[ExecProgram] Błąd wczytywania parametrów dla komendy: " << cmdName << "\n";
            continue;
        }
        std::istringstream objStream(cmd.params);
        std::string objNameParam;
        objStream >> objNameParam;

        command->PrintParams();

        if (!command->ExecCmd(_Scn, objNameParam.c_str(), _Chann2Serv)) {
            std::cerr << "[ExecProgram] Nie udało się wykonać komendy: " << cmdName << "\n";
        }
        delete command;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } else{
            parallelBlock.push_back(cmd);
            continue;
        }
    }

    std::cout << "\n[ExecProgram] Zakończono wykonanie programu.\n";
    return true;
}

void ProgramInterpreter::ExecSingleCommand(CommandData cmd,
                       Scene &scn,
                       ComChannel &chan,
                       Set4LibInterfaces &_LibManager)
{
    {
        std::cout << "[Thread " << std::this_thread::get_id()
                  << "] Start polecenia: " << cmd.name
                  << "  Params: " << cmd.params << "\n";
    }

    auto lib = _LibManager.Find(cmd.name);
    if (!lib) {
        std::cerr << "Brak biblioteki dla: " << cmd.name << "\n";
        return;
    }

    auto command = lib->CreateCmd();
    std::istringstream paramStream(cmd.params);
    command->ReadParams(paramStream);

    std::istringstream objStream(cmd.params);
    std::string objName;
    objStream >> objName;

        command->ExecCmd(scn, objName.c_str(), chan);


    delete command;

        std::cout << "[Thread " << std::this_thread::get_id()
                  << "] KONIEC polecenia: " << cmd.name << "\n";
    
}

bool ProgramInterpreter::Read_XML_Config(const char *fileName) {
    try {
            XMLPlatformUtils::Initialize();
   }
   catch (const XMLException& toCatch) {
            char* message = XMLString::transcode(toCatch.getMessage());
            std::cerr << "Error during initialization! :\n";
            std::cerr << "Exception message is: \n"
                 << message << "\n";
            XMLString::release(&message);
            return 1;
   }

   auto pParser = std::unique_ptr<SAX2XMLReader>(XMLReaderFactory::createXMLReader());

   pParser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
   pParser->setFeature(XMLUni::fgSAX2CoreValidation, true);
   pParser->setFeature(XMLUni::fgXercesDynamic, false);
   pParser->setFeature(XMLUni::fgXercesSchema, true);
   pParser->setFeature(XMLUni::fgXercesSchemaFullChecking, true);

   pParser->setFeature(XMLUni::fgXercesValidationErrorAsFatal, true);


   auto pHandler = std::make_unique<XMLInterp4Config>(_xmlConfig);
   pParser->setContentHandler(pHandler.get());
   pParser->setErrorHandler(pHandler.get());

   try {
     
     if (!pParser->loadGrammar("config/config.xsd",
                              xercesc::Grammar::SchemaGrammarType,true)) {
       std::cerr << "!!! Plik grammar/actions.xsd, '" << "\n"
            << "!!! ktory zawiera opis gramatyki, nie moze zostac wczytany."
            << "\n";
       return false;
     }
     pParser->setFeature(XMLUni::fgXercesUseCachedGrammarInParse,true);
     pParser->parse(fileName);
   }
   catch (const XMLException& Exception) {
            char* sMessage = XMLString::transcode(Exception.getMessage());
            std::cerr << "Informacja o wyjatku: \n"
                 << "   " << sMessage << "\n";
            XMLString::release(&sMessage);
            return false;
   }
   catch (const SAXParseException& Exception) {
            char* sMessage = XMLString::transcode(Exception.getMessage());
            char* sSystemId = xercesc::XMLString::transcode(Exception.getSystemId());

            std::cerr << "Blad! " << "\n"
                 << "    Plik:  " << sSystemId << "\n"
                 << "   Linia: " << Exception.getLineNumber() << "\n"
                 << " Kolumna: " << Exception.getColumnNumber() << "\n"
                 << " Informacja: " << sMessage 
                 << "\n";

            XMLString::release(&sMessage);
            XMLString::release(&sSystemId);
            return false;
   }
   catch (...) {
            std::cout << "Zgloszony zostal nieoczekiwany wyjatek!\n" ;
            return false;
   }

//    XMLPlatformUtils::Terminate();
   return true;
}
