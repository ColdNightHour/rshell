#include <sys/wait.h>
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
void connectors(string userinput, vector<vector<int> > &x) {
	for(unsigned int i = 0; i < userinput.size() - 1; i++) {
		vector<int> c(1);
		if((userinput.at(i) == '|') && (userinput.at(i + 1) == '|')) {
			c.at(0) = 0;
			x.push_back(c);
			x.at(x.size() - 1).push_back(i);
		}
		else if((userinput.at(i) == '&') && (userinput.at(i + 1) == '&')) {
			c.at(0) = 1;
			x.push_back(c);
			x.at(x.size() - 1).push_back(i);
		}
		else if((userinput.at(i) == ';')) {
			c.at(0) = 2;
			x.push_back(c);
			x.at(x.size() - 1).push_back(i);
		}
	}
}

int main() {
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
	//char *arg[100000];
	char *command;

	char *command_a;
	char limits[5] = ";&| ";
	bool exit  = false;
	while(!exit) {
		getline(cin, userinput);
		command = new char[userinput.size()];
		vector<vector<int> > connector;
		connectors(userinput, connector);
		strcpy(command, userinput.c_str());
		cout << getlogin() << "@" << hostname << " $ ";
		command_a = strtok(command,limits);
	//	cout << command_a;
		int pid = fork();
		if(pid == -1)
			perror("fork");
		if(pid == 0)
			cout << userinput << endl;
		else
		wait(0);
	//	cout << endl;
		
	}	
	return 0;
}
