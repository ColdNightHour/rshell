FLAGS = -W -Wall -ansi -pedantic
COM = g++ --std=c++11 
all: rshell
	$(COM) $(FLAGS) src/main.cpp
rshell: 
	$(COM) $(FLAGS) src/main.cpp
run:
	./a.out
