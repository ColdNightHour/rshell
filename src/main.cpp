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
#include "main.h"
using namespace std;

int main() {
	string userinput; 
	string login = getlogin();
	if(login == "")
		perror("getlogin"); 
	char hostname[128];  
	if(gethostname(hostname, sizeof hostname))
		perror("gethostname");
	bool ext  = false;
	string exit_status = "exit";
	while(!ext) {
		cout << login << "@" << hostname << " $ ";
		char *command_a;
		char *command;
		getline(cin, userinput);
		if(userinput.find("#") != string::npos)
			userinput.erase(userinput.find("#"), userinput.size());
		command = new char[userinput.size()];
		vector<int> c_pat;
		vector<int> c_pos;
		bool first = false;
		bool multiple = false;
		if(userinput.size() != 0)
			connectors(userinput, c_pat, c_pos, first, multiple);
		int x = 0;
		unsigned int b = 0;
		int y = 0;
		char *arg[50000];
		int status;
		redir condition;
		if(userinput.size() != 0) {
			while(userinput.substr(x, 1) != "") {
				if(multiple) {
					cout << "Error: Incorrect connector config" << endl;
					break;
				}
				if(first) {
					cout << "Error: file does not exist" << endl;
					break;
				}
				redir_check(condition, userinput.substr(x, c_pos.at(y) -x).c_str());
							strcpy(command, userinput.substr(x, c_pos.at(y) - x).c_str());
				command_a = strtok(command, "&;| \t");
				while(command_a != NULL) {
					arg[b] = command_a;
					command_a = strtok(NULL, "&;| \t");
					b++;
				}
				if(userinput.substr(x, c_pos.at(y) - x).find("exit") != string::npos && b==1) {
					ext = true;
					break;
				}
				//int i = fork();
				//if(i ==  -1)
				//	perror("fork");
				if(condition.redir_x) {
					redir_action(condition);	
				}

				else {
					int i = fork();
					if(i == -1)
						perror("fork");
					if(i == 0) {
						if(execvp(arg[0], arg) == -1) {
							perror("execvp");
							exit(-1);
						}
						exit(0);
					}
				
				if(wait(&status) == -1)
					perror("wait");
				}
				x = c_pos.at(y);
				unsigned int help = c_pat.at(y);
				for(unsigned int i = 0; i < b; i++)
					arg[i] = NULL;
				nullify(condition);
				if((help == 1 && status == 0) || (help == 0 && status != 0) || help == 2)
					y++;
				else if(help == 1 && status != 0 && (userinput.find("||", x) != string::npos || userinput.find(";", x) != string::npos)) {
					x = c_pos.at(y + 1);
					y+=2;
				}
				else if(help == 0 && status == 0 && (userinput.find("&&", x) != string::npos || userinput.find(";", x) != string::npos)) {
					x = c_pos.at(y + 1);
					y+=2;
				}
				else 
					break;
				b = 0;			
			}
		}
		delete []command;
	}
	return 0;
}
