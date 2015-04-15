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

void connectors(string userinput, vector<int> &x, vector<int> &y, bool &first) {
	for(unsigned int i = 0; i < userinput.size() - 1; i++) {
		if((userinput.at(i) == '|') && (userinput.at(i + 1) == '|')) {
			x.push_back(0);
			y.push_back(i);
		}
		else if((userinput.at(i) == '&') && (userinput.at(i + 1) == '&')) {
			x.push_back(1);
			y.push_back(i);
		}
		else if((userinput.at(i) == ';')) {
			x.push_back(2);
			y.push_back(i);
		}
	}
	y.push_back(0);
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
	

	char limits[5] = ";&| ";
	bool exit  = false;
	while(!exit) {
		cout << getlogin() << "@" << hostname << " $ ";
		char *command_a;
		char *command;
		getline(cin, userinput);
		command = new char[userinput.size()];
		vector<int> c_pat;
		vector<int> c_pos;
		bool first = false;
		connectors(userinput, c_pat, c_pos, first);
		int x = 0;
		int b = 0;
		int y = 0;
		while(y <= 1) {
			if(c_pat.size() == 0)
				strcpy(command, userinput.c_str());
			else
				strcpy(command, userinput.substr(x, c_pos.at(y) - x).c_str());
			command_a = strtok(command,limits);
			char *arg[100000];
			while(command_a != NULL) {
				cout << command_a;
				arg[b] = command_a;
				command_a = strtok(NULL, limits);
				b++;
			}
			if(execvp(arg[0], arg) == -1)
				perror("execvp");
			y++;
			/*int pid = fork();
			if(pid == -1)
				perror("fork");
			if(pid == 0)
				cout << userinput << endl;
			else
				wait(0);*/
		}
	}	
	return 0;
}
