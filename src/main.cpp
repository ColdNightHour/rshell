#include <cstdlib>
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
#include <stdlib.h>

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
	if(userinput.at(0) == '&' || userinput.at(0) == '|' || userinput.at(0) == ';')
		first = true;
	x.push_back(userinput.size());
	y.push_back(userinput.size());
}

int main() {
	string userinput; 
	string login;
	if(!getlogin())
		perror("getlogin"); 
	else 
		login = getlogin(); 
	char hostname[128];  
	if(gethostname(hostname, sizeof hostname))
		perror("gethostname");
	char limits[6] = ";&| \t";
	bool ext  = false;
	string exit_status = "exit";
	while(!ext) {
		cout << getlogin() << "@" << hostname << " $ ";
		char *command_a;
		char *command;
		getline(cin, userinput);
		if(userinput.size() == 0)
			userinput+="#";
		command = new char[userinput.size()];
		vector<int> c_pat;
		vector<int> c_pos;
		bool first = false;
		userinput.size();
		connectors(userinput, c_pat, c_pos, first);
		int x = 0;
		unsigned int b = 0;
		int y = 0;
		char *arg[100000];
		int status;
		while(userinput.substr(x, 1) != "") {
			if(first) {
				cout << "Error: file does not exist" << endl;
				break;
			}
			if(c_pat.size() == 1)
				strcpy(command, userinput.c_str());
			else
				strcpy(command, userinput.substr(x, c_pos.at(y) - x).c_str());
			
			command_a = strtok(command,limits);
			while(command_a != NULL) {
				if(command_a[0] == '#') { 
					break;
				}
				arg[b] = command_a;
				command_a = strtok(NULL, limits);
				b++;
			}
			if(userinput.substr(x, c_pos.at(y) - x).find("exit") != string::npos && b==1) {
				ext = true;
				break;
			}
			int i = fork();
			if(i ==  -1)
				perror("fork");
			if(i == 0) {
				if(execvp(arg[0], arg) == -1) {
					perror("execvp");
					exit(-1);
				}
				exit(0);
			}
			wait(&status);
			x = c_pos.at(y);
			unsigned int help = c_pat.at(y);
			for(unsigned int i = 0; i < b; i++)
				arg[i] = NULL;
			if(help == 0 && status == 0 && userinput.find("&&", y) != string::npos && userinput.find(";", y) != string::npos) 
				y++;
			else if(help == 0 && status == 0) {
				y++;
				break; 
			}
			else if(help == 1 && status != 0 && userinput.find("||", y) != string::npos && userinput.find(";", y) != string::npos)
				y++; 
			else if(help == 1 && status != 0) {
				y++;
				break;
			}	
			else 
				y++;
			b = 0;			
		}
	}	
	return 0;

}
