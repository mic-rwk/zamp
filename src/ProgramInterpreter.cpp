#include "../inc/ProgramInterpreter.hh"
#include "../inc/xmlinterp.hh"
#include "../inc/Configuration.hh"
#include "preprocessor.hh"
#include <fstream>
#include <sstream>
#include <iostream>
#include <thread>

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

    // const char *envMax = std::getenv("ZAMP_MAX_CMDS");
    // int maxCmds = envMax ? std::atoi(envMax) : -1;
    // int cmdCount = 0;

    for (const auto &cmd : parser.GetCommands()) {
        // if (maxCmds >= 0 && cmdCount >= maxCmds) break;
        std::string libName = "libs/libInterp4" + cmd.name + ".so";
        std::string cmdName = cmd.name;

        auto lib = _LibManager.Find(cmdName);
        if (!lib) {
            if (!_LibManager.AddLibrary(libName)) {
                std::cerr << "[ExecProgram] Nie udało się załadować biblioteki: " << libName << "\n";
                continue;
            }
            lib = _LibManager.Find(cmdName);
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
        // ++cmdCount;
    }

    std::cout << "\n[ExecProgram] Zakończono wykonanie programu.\n";
    return true;
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
