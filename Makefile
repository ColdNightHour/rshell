FLAGS = -W -Wall -ansi -pedantic
COM = g++ --std=c++11 
all: rshell
	$(COM) $(FLAGS) main.cpp
rshell: 
	$(COM) $(FLAGS) main.cpp 
