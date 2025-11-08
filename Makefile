__start__: obj __lines_for_space__ interp __plugin__
	export LD_LIBRARY_PATH="./libs"; ./interp komendy.cmd

obj:
	mkdir obj


__lines_for_space__:
	@echo
	@echo
	@echo
	@echo
	@echo


__plugin__:
	cd plugin; make

CPPFLAGS=-Wall -pedantic -std=c++17 -Iinc
LDFLAGS=-Wall




interp: obj/main.o obj/preprocessor.o obj/LibInterface.o obj/CommandsParser.o obj/Scene.o obj/Set4LibInterfaces.o\
obj/ComChannel.o obj/ProgramInterpreter.o
	g++ ${LDFLAGS} -g -o interp  obj/main.o obj/preprocessor.o obj/LibInterface.o obj/CommandsParser.o obj/Scene.o\
	 obj/Set4LibInterfaces.o obj/ComChannel.o obj/ProgramInterpreter.o -ldl

obj/main.o: src/main.cpp inc/AbstractInterp4Command.hh inc/AbstractScene.hh\
            inc/AbstractComChannel.hh inc/preprocessor.hh inc/LibInterface.hh\
			inc/CommandsParser.hh inc/Scene.hh inc/Set4LibInterfaces.hh\
			inc/ComChannel.hh inc/ProgramInterpreter.hh
	g++ -c ${CPPFLAGS} -o obj/main.o src/main.cpp

obj/preprocessor.o: src/preprocessor.cpp inc/preprocessor.hh
	g++ -c ${CPPFLAGS} -o obj/preprocessor.o src/preprocessor.cpp

obj/LibInterface.o: src/LibInterface.cpp inc/LibInterface.hh
	g++ -c ${CPPFLAGS} -o obj/LibInterface.o src/LibInterface.cpp

obj/CommandsParser.o: src/CommandsParser.cpp inc/CommandsParser.hh
	g++ -c ${CPPFLAGS} -o obj/CommandsParser.o src/CommandsParser.cpp

obj/Scene.o: src/Scene.cpp inc/Scene.hh
	g++ -c ${CPPFLAGS} -o obj/Scene.o src/Scene.cpp

obj/Set4LibInterfaces.o: src/Set4LibInterfaces.cpp inc/Set4LibInterfaces.hh
	g++ -c ${CPPFLAGS} -o obj/Set4LibInterfaces.o src/Set4LibInterfaces.cpp

obj/ComChannel.o: src/ComChannel.cpp inc/ComChannel.hh
	g++ -c ${CPPFLAGS} -o obj/ComChannel.o src/ComChannel.cpp

obj/ProgramInterpreter.o: src/ProgramInterpreter.cpp inc/ProgramInterpreter.hh
	g++ -c ${CPPFLAGS} -o obj/ProgramInterpreter.o src/ProgramInterpreter.cpp

doc:
	cd dox; make

clean:
	rm -f obj/* interp core*


clean_plugin:
	cd plugin; make clean

cleanall: clean
	cd plugin; make cleanall
	cd dox; make cleanall
	rm -f libs/*
	find . -name \*~ -print -exec rm {} \;

help:
	@echo
	@echo "  Lista podcelow dla polecenia make"
	@echo 
	@echo "        - (wywolanie bez specyfikacji celu) wymusza"
	@echo "          kompilacje i uruchomienie programu."
	@echo "  clean    - usuwa produkty kompilacji oraz program"
	@echo "  clean_plugin - usuwa plugin"
	@echo "  cleanall - wykonuje wszystkie operacje dla podcelu clean oraz clean_plugin"
	@echo "             oprocz tego usuwa wszystkie kopie (pliki, ktorych nazwa "
	@echo "             konczy sie znakiem ~)."
	@echo "  help  - wyswietla niniejszy komunikat"
	@echo
	@echo " Przykladowe wywolania dla poszczegolnych wariantow. "
	@echo "  make           # kompilacja i uruchomienie programu."
	@echo "  make clean     # usuwa produkty kompilacji."
	@echo
 
