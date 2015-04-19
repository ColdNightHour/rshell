FLAGS = -W -Wall -ansi -pedantic
COM = g++ --std=c++11 
BIN = -o bin/rshell
MKDIR = mkdir bin
RMDIR = rm -rfv bin

all: rshell
	$(RMDIR)
	$(MKDIR)
	$(COM) $(FLAGS) src/main.cpp $(BIN)
rshell: 
	$(RMDIR)
	$(MKDIR)
	$(COM) $(FLAGS) src/main.cpp $(BIN)
run:
	bin/rshell

