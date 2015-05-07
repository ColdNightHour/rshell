FLAGS = -W -Wall -ansi -pedantic
COM = g++ --std=c++11 
BIN = -o bin/rshell
BINLS = -o bin/ls
BINCP = -o bin/cp
BINRM = -o bin/rm
BINMV = -o bin/mv
MKDIR = mkdir bin
RMDIR = rm -rfv bin

all: rshell ls cp mv rm
rshell: 
	$(RMDIR)
	$(MKDIR)
	$(COM) $(FLAGS) src/main.cpp $(BIN)
ls:
	$(COM) $(FLAGS) src/ls.cpp src/ls.h $(BINLS)
cp: 
	$(COM) $(FLAGS) src/cp.cpp $(BINCP)
mv:
	$(COM) $(FLAGS) src/mv.cpp $(BINMV)
rm:
	$(COM) $(FLAGS) src/rm.cpp $(BINRM)	
