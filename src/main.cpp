#include <cstdlib>
#include <sys/wait.h>
#include <string> 
#include <vector>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
//#include <stdio.h>
#include <errno.h>
//#include <stdlib.h>

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
	bool ext  = false;
	while(!ext) {
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
		while(c_pos.at(y) != 0)  {
			char *arg[10];
			//arg = new char*[100];
			//arg[99] = '\0';
			if(c_pat.size() == 0)
				strcpy(command, userinput.c_str());
			else
				strcpy(command, userinput.substr(x, c_pos.at(y) - x).c_str());
			command_a = strtok(command,limits);
			while(command_a != NULL) {
				arg[b] = command_a;
				cout << arg[b] << "|";
				command_a = strtok(NULL, limits);
				b++;
			}
			//arg[b+1] = '\0';
			cout << endl;
			int i = fork();
			if(i ==  -1)
				perror("fork");
			if(i == 0) {
				if(execvp(arg[0], arg) == -1)
					perror("execvp");
				exit(0);
			}
			int status;
			wait(&status);
			x = c_pos.at(y);
			y++;
			b = 0;
			cout << "end reached";
			//for(int i = 0; i < 100; i++)
			//	delete[] arg[i];
			//delete[] arg;	
		}
	}	
	return 0;
}
