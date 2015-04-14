#include <cstring>
#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>

using namespace std;

int main() {
        string userinput = "";
	string login;
	if(!getlogin())
		perror("getlogin"); 
	else 
		login = getlogin();
	char hostname[64];
	if(gethostname(hostname, sizeof hostname))
		perror("gethostname");
	getline(cin, userinput);
	char *command;
	command = new char[userinput.size()];
	strcpy(command, userinput.c_str());
	char *command_a;
	char limits[5] = ";&| ";
	command_a = strtok(command,limits);
	while(command_a != NULL){
		cout << command_a << " ";
		command_a = strtok(NULL, limits);
	}
	return 0;
}
