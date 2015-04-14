#include <vector>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>

using namespace std;
void connectors(string userinput, vector<string> &x) {
	int pos = userinput.find("&&" || "||" || ";", 0);
	while(pos != string::npos) {
		x.push_back(user
	}
}
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
	
	cout << getlogin() << "@" << hostname << " $ ";	
	getline(cin, userinput);
	char *command;
	command = new char[userinput.size()];
	strcpy(command, userinput.c_str());
	char *command_a;
	char limits[5] = ";&| ";
	command_a = strtok(command,limits);
	int i = 0;
	vector<string> connector((userinput.size())/2);
	while(command_a != NULL){
		arg[i] = command_a;
		command_a = strtok(NULL, limits);
		i++;
	}

	if(execvp(arg[0], arg) == -1)
		perror("execvp");
	return 0;
}
