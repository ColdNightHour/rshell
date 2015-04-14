#include <cstring>
#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>

using namespace std;

int main(int argc, char **argv) {
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
	cout << argc;
	while(command_a != NULL){
		i++;
		if(command_a[0] == '-')
			argv[i] = command_a;
		cout << argv[i];
		command_a = strtok(NULL, limits);
	}
	return 0;
}
