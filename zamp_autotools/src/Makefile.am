#src
AUTOMAKE_OPTIONS = subdir-objects
bin_PROGRAMS=interp
interp_SOURCES = main.cpp \
                     Scene.cpp \
                     ComChannel.cpp \
                     CommandsParser.cpp \
                     Configuration.cpp \
                     LibInterface.cpp \
                     MobileObject.cpp \
                     preprocessor.cpp \
                     ProgramInterpreter.cpp \
                     Set4LibInterfaces.cpp \
                     xmlinterp.cpp
interp_CXXFLAGS = -I../inc -I../../../inc -std=c++17
interp_LDFLAGS = -Wl,-rpath='$(libdir)' ${LIB_DLOPEN} -lxerces-c -lpthread