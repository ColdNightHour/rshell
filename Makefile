all: rshell
	g++ --std=c++11 -W -Wall -ansi -pedantic main.cpp
rshell: main.cpp
	g++ --std=c++11 -W -Wall -ansi -pedantic main.cpp 
