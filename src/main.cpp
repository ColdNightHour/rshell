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
	getline(cin, userinput);
	//char *arg[100000];
	char *command;
	command = new char[userinput.size()];
	strcpy(command, userinput.c_str());
	char *command_a;
	char limits[5] = ";&| ";
	command_a = strtok(command,limits);
	vector<vector<int> > connector;
	cout << command_a;
	connectors(userinput, connector);
	cout << connector.at(0).at(0) << connector.at(0).at(1);	
	cout << connector.at(1).at(0) << connector.at(1).at(1);
	cout << connector.at(2).at(0) << connector.at(2).at(1);
	return 0;
}

