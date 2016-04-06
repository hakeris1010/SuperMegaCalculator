mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))
CC=g++
LCC=ar

CFLAGS =-std=c++11 -c -Wall
LDFLAGS=
INCLUDE=-I$(mkfile_path)/src -I$(mkfile_path)/src/**/ -I$(mkfile_path)/include
LINKER=-L$(mkfile_path)/lib

SOURCES=src/main.cpp src/calcstart.cpp src/compute/calc.cpp src/compute/transform.cpp src/compute/equational.cpp src/defines/formal.cpp src/tools/fun.cpp

SOURCES_LIB=src/compute/calc.cpp src/compute/transform.cpp src/compute/equational.cpp src/defines/formal.cpp src/tools/fun.cpp

SOURCES_FINAL_WLIB=test/maintest.cpp

OBJECTS=$(SOURCES:.cpp=.o)
OBJECTS_LIB=$(SOURCES_LIB:.cpp=.o)
OBJECTS_FINAL_WLIB=$(SOURCES_FINAL_WLIB:.cpp=.o)

EXECUTABLE=SuperMegaCalculator

.cpp.o:
	$(CC) $(INCLUDE) $(CFLAGS) $< -o $@

all: normal_exe static_lib exec_wlib 
    
normal_exe: $(SOURCES) $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

static_lib: $(SOURCES_LIB) $(OBJECTS_LIB)
	$(LCC) rvs lib/lib$(EXECUTABLE).a $(OBJECTS_LIB)

exec_wlib: static_lib $(SOURCES_FINAL_WLIB) $(OBJECTS_FINAL_WLIB)
	$(CC) $(LDFLAGS) $(LINKER) $(OBJECTS_FINAL_WLIB) -o $(EXECUTABLE)_LibTest -l$(EXECUTABLE)

clean_o:
	rm -f **/*.o **/**/*.o 

clean_all: clean_o
	rm -f $(EXECUTABLE) $(EXECUTABLE)_LibTest $(EXECUTABLE).exe $(EXECUTABLE)_LibTest.exe lib/lib$(EXECUTABLE).a

fresh: clean_all all

fresh_lib: clean_all static_lib

fresh_exec_wlib: clean_all exec_wlib