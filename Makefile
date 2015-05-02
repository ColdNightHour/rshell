FLAGS = -W -Wall -ansi -pedantic
COM = g++ --std=c++11 
BIN = -o bin/rshell
BINLS = -o bin/ls
MKDIR = mkdir bin
RMDIR = rm -rfv bin

all: rshell ls
rshell: 
	$(RMDIR)
	$(MKDIR)
	$(COM) $(FLAGS) src/main.cpp $(BIN)
ls:
	$(COM) $(FLAGS) src/ls.cpp src/ls.h $(BINLS)
