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
void connectors(string userinput, vector<string> &x, bool &first) {
	size_t pos = userinput.find("&&" || "||" || ";", 0);
	if(pos == 0)
		first = true;
	while(pos != string::npos) {
			x.push_back(userinput.substr(pos, 2));
			pos = userinput.find("&&" || "||" || ";", pos + 1);
			cout << x.at(0);
	}
}

int main() {
	//char *arg[1000];
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
	//int i = 0;
	cout << command_a; //here
	vector<string> connector((userinput.size())/2);
	bool first = false;
	connectors(userinput, connector, first);
	cout << first;
	cout << connector.at(0);
	cout << connector.at(1);
	cout << connector.at(2);
	/*while(command_a != NULL){
		arg[i] = command_a;
		command_a = strtok(NULL, limits);
		i++;
	}

	if(execvp(arg[0], arg) == -1)
		perror("execvp"); */
	return 0;
}
