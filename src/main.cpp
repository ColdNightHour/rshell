#include <cstring>
#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>

using namespace std;

int main() {
	char *arg[1000];
        string userinput = "";
	string login;
	//used to get login info
	if(!getlogin())
		perror("getlogin"); 
	else 
		login = getlogin();    //will only execute if syscall works
	char hostname[64];
	//gets hostname, though only works if 
	if(gethostname(hostname, sizeof hostname))
		perror("gethostname");
	getline(cin, userinput);
	char *command;
	command = new char[userinput.size()];
	strcpy(command, userinput.c_str());
	char *command_a;
	char limits[5] = ";&| ";
	command_a = strtok(command,limits);
	int i = 0;
	while(command_a != NULL){
		arg[i] = command_a;
		command_a = strtok(NULL, limits);
		i++;
	}
	execvp(arg[0], arg);	
	return 0;
}
