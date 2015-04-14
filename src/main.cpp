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
	cout << strlen(command);
	char *command_a;
	char limits[4] = ";&|";
	command_a = strtok(command,limits);
	return 0;
}
