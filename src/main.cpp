#include <string> 
#include <vector>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>

using namespace std;
void connectors(string userinput, vector<int> &x, bool &first) {
	if((userinput.at(0) == '&')||(userinput.at(0) == '|') || (userinput.at(0) ==';'))
		first = true;
	for(unsigned int i = 0; i < userinput.size(); i++) {
		if((userinput.at(i) == '&')||(userinput.at(i) == '|') || (userinput.at(i) ==';'))
			x.push_back(i);
	}
}

int main() {
	//char *arg[1000];
        string userinput = "";
	string login;
	if(!getlogin())
		perror("getlogin"); 
	else 
		login = getlogin(); 
	char hostname[128];  
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
	cout << command_a;
	vector<int> connector;
	bool first = false;
	connectors(userinput, connector, first);
	cout << connector.at(0) << connector.at(1);
	return 0;
}
