#include "../inc/ProgramInterpreter.hh"
#include "../inc/xmlinterp.hh"
#include "../inc/Configuration.hh"
#include "preprocessor.hh"
#include <fstream>
#include <sstream>
#include <iostream>

#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>

using namespace xercesc;

bool ProgramInterpreter::ExecProgram(const char *fileName_Prog)
{

    std::cout << "\n[ExecProgram] Uruchamianie programu na podstawie danych XML\n";

    const Configuration &cfg = _xmlConfig;

    if (cfg.plugins.empty()) {
        std::cerr << "[ExecProgram] Brak bibliotek wczytanych z XML.\n";
        return false;
    }

    for (const auto &plugin : cfg.plugins) {
        std::string fullLibPath = "libs/" + plugin.libName;
        if (!_LibManager.AddLibrary(fullLibPath)) {
            std::cerr << "[ExecProgram] Nie udało się załadować biblioteki: " << fullLibPath << "\n";
        } else {
            std::cout << "[ExecProgram] Załadowano: " << fullLibPath << "\n";
        }
    }

    for (const auto &obj : cfg.objects) {
        std::cout << "\n[Obiekt] " << obj.name
                  << "\n  Shift: " << obj.shift
                  << "\n  Scale: " << obj.scale
                  << "\n  RotXYZ: " << obj.rot
                  << "\n  Trans: " << obj.trans
                  << "\n  RGB: " << obj.rgb << "\n";
    }

    for (const auto & plugin : cfg.plugins) {
        std::string baseName = plugin.libName;
        size_t pos = baseName.rfind(".so");   
        std::string cmdName = baseName.substr(10, pos-10);


        auto lib = _LibManager.Find(cmdName);
        if (!lib) {
            std::cerr << "[ExecProgram] Nie znaleziono wtyczki dla: " << cmdName << "\n";
            continue;
        }

        auto command = lib->CreateCmd();
        if (!command) {
            std::cerr << "[ExecProgram] Nie udało się utworzyć komendy: " << cmdName << "\n";
            continue;
        }

        std::istringstream params("");
        if (!command->ReadParams(params)) {
            std::cerr << "[ExecProgram] Błąd wczytywania parametrów dla komendy: " << cmdName << "\n";
            continue;
        }

        command->PrintParams();

        if (!command->ExecCmd(_Scn, "ObiektA", _Chann2Serv)) {
            std::cerr << "[ExecProgram] Nie udało się wykonać komendy: " << cmdName << "\n";
        }
    }

    std::cout << "\n[ExecProgram] Zakończono wykonanie programu XML.\n";
    return true;
    // std::string preprocessed = RunPreprocessor(fileName_Prog);
    // std::istringstream cmdStream(preprocessed);

    // CommandsParser parser;
    // if (!parser.ReadCommandsList(cmdStream)) {
    //     std::cerr << "[ProgInterp] Błąd podczas parsowania komend.\n";
    //     return false;
    // }

    // for (const auto &cmd : parser.GetCommands()) {
    //     std::string libName = "libs/libInterp4" + cmd.name + ".so";

    //     auto lib = _LibManager.Find(cmd.name);
    //     if (!lib) {
    //         if (!_LibManager.AddLibrary(libName))
    //             continue;
    //         lib = _LibManager.Find(cmd.name);
    //     }

    //     auto command = lib->CreateCmd();
    //     if(!command) {
    //         std::cerr << "[ProgramInterpreter] Nie udało się utworzyć komendy: " << cmd.name;
    //         continue;
    //     }

    //     std::istringstream paramStream(cmd.params);
    //     if (!command->ReadParams(paramStream)) {
    //         std::cerr << "[ProgramInterpreter] Błąd parametrów dla komendy: " << cmd.name << "\n";
    //         continue;
    //     }

    //     command->PrintParams();

    //     if (!command->ExecCmd(_Scn, "ObiektA", _Chann2Serv)) {
    //         std::cerr << "[ProgramInterpreter] ExecCmd nie powiódł się dla " << cmd.name << "\n";
    //     }
    // }
    // return true;
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

   SAX2XMLReader* pParser = XMLReaderFactory::createXMLReader();

   pParser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
   pParser->setFeature(XMLUni::fgSAX2CoreValidation, true);
   pParser->setFeature(XMLUni::fgXercesDynamic, false);
   pParser->setFeature(XMLUni::fgXercesSchema, true);
   pParser->setFeature(XMLUni::fgXercesSchemaFullChecking, true);

   pParser->setFeature(XMLUni::fgXercesValidationErrorAsFatal, true);


   DefaultHandler* pHandler = new XMLInterp4Config(_xmlConfig);
   pParser->setContentHandler(pHandler);
   pParser->setErrorHandler(pHandler);

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

   delete pParser;
   delete pHandler;
   return true;
}
