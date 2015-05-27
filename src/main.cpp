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
	struct sigaction Curr = {0};
	struct sigaction Prev = {0};
	sigset_t x;
	Curr.sa_mask = x;
	Curr.sa_sigaction = sigHandle;
	if(sigaction(SIGINT, &Curr, &Prev) == -1)
		perror("SIGINT sigaction");
	if(sigaction(SIGTSTP, &Curr, &Prev) == -1)
		perror("SIGTSTP sigaction");
	if(sigaction(SIGCHLD, &Curr, &Prev) == -1) 
		perror("SIGCHLD sigaction");	
	while(!ext) {
		char *pPath;
		pPath = getenv("PWD");
		cout << login << "@" << hostname << "~" << pPath << " $ ";
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
				if(condition.redir_x) {
					redir_action(condition);
					nullify(condition);	
				}
				else if(strcmp(arg[0], "cd") == 0) {
					if(b == 1) {
						char *home, *old;
						if((home = getenv("HOME")) == NULL)
							perror("getenv_1.1");
						if((old = getenv("PWD")) == NULL)
							perror("getenv_1.2");
						if(chdir(home) == -1)
							perror("chdir_1");	
						if(setenv("PWD", home, 1) == -1)
							perror("setenv_1.1");
						if(setenv("OLDPWD", old, 1) == -1)
							perror("setenv_1.2");
					}
					else if(strcmp(arg[1], "-") == 0) {
						char *old, *newDir;
						if((old = getenv("PWD")) == NULL) 
							perror("getenv_2.1");
						if((newDir = getenv("OLDPWD")) == NULL) 
							perror("getenv_2.2");
						if(chdir(newDir) == -1)
							perror("chdr_2");
						if(setenv("PWD", newDir, 1) == -1)
							perror("setenv_2.1");
						if(setenv("OLDPWD", old, 1) == -1) 
							perror("setenv_2.2");
					}
					else{
						char *old;
						if((old = getenv("PWD")) == NULL)
							perror("getenv_3.1");
						if(setenv("OLDPWD", "PWD", 1) == -1)
							perror("setenv_3.1");	
						if(chdir(arg[1]) == -1)
							perror("chdir_3");
						if(setenv("PWD", arg[1], 1) == -1) 
							perror("setenv_3.2");
					
					}

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
