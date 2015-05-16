#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
struct arr{
	char *ar[100];
	int sz;
};
struct redir {
	bool redir_x; //checks to see if redirection is to be done
	bool pip; //checks to see if piping is to be down 
	vector<int> places; //vector which holds the places of each redirection element in a string
	vector<string> types; //vector which holds the types, whether |, <, >, or >>
	vector<string> ofiles; //vector which holds the files to be modified
	vector<arr> commands; //vector which holds com_flags
	int index;
};

void connectors(string userinput, vector<int> &x, vector<int> &y, bool &first, bool &multiple) {
	x.clear();
	y.clear();
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
	y.push_back(userinput.size());
	for(unsigned int i = 0; i < y.size() - 1; i++) {
		if(y.at(i + 1) == y.at(i) + 1)
			multiple = true;
	} 
	if(userinput.at(0) == '&' || userinput.at(0) == '|' || userinput.at(0) == ';')
		first = true;
	x.push_back(userinput.size());
}
void redir_check(redir &condition, string sub_str) {
	condition.redir_x = false;
	condition.pip = false;
	for(unsigned int i = 0; i < sub_str.size() - 1; i++) {
		if(sub_str.at(i) == '<') {
			condition.redir_x = true;
			condition.places.push_back(i);
			condition.types.push_back(sub_str.substr(i, 1));
		}
		else if(sub_str.at(i) == '|' && sub_str.at(i + 1) != '|') {
			condition.redir_x = true;
			condition.pip = true;
			condition.places.push_back(i);
			condition.types.push_back(sub_str.substr(i, 1));
		}
		else if(sub_str.at(i) == '>' && sub_str.at(i + 1) != '>') {
			condition.redir_x = true;
			condition.places.push_back(i);
			condition.types.push_back(sub_str.substr(i, 1));
		}
		else if(sub_str.at(i) == '>' && sub_str.at(i + 1) == '>') {
			condition.redir_x = true;
			condition.places.push_back(i);
			condition.types.push_back(sub_str.substr(i, 2));
			i++;
		}
	}
	char *sub;
	sub = new char[100];
	if(!condition.redir_x)
		return;
	strcpy(sub, sub_str.c_str());
	char *sub_x = strtok(sub, " <>|");
	while(sub_x != NULL) {
		string x(sub_x);
		if(x.find('-') == string::npos)
			condition.ofiles.push_back(x);
		sub_x = strtok(NULL, " <>|");
	}
	if(condition.places.size() == 0) 
		return;
	int x = 0;
	condition.places.push_back(sub_str.size());
	for(unsigned int i = 0; i < condition.places.size(); i++) {
		arr cmd;
		char *subb;
		subb = new char[100];
		strcpy(subb, sub_str.substr(x, condition.places.at(i) - x).c_str());
		char *sub_y = strtok(subb, " <>|");
		int j = 0;
		while(sub_y != NULL) {
			cmd.ar[j] = sub_y;
			sub_y = strtok(NULL, " <>|");
			j++;
		}
		cmd.ar[j] = sub_y;
		cmd.sz = j;
		condition.commands.push_back(cmd);
		x = condition.places.at(i);
	}
	int v = 100;
	arr end;
	end.sz = v;
	condition.commands.push_back(end);
	condition.types.push_back("end");
}
void io_redir_action(redir &condition) {
	int fd;
	int i = 0;
	pid_t pid = fork();
	if(pid == 0) {
	while (condition.types.at(i) == "<" || condition.types.at(i) == ">>" || condition.types.at(i) == ">") {
		if(condition.types.at(i) == ">" || condition.types.at(i) == ">>") {
			if(close(1) == -1)
				perror("close");
			if(condition.types.at(0) == ">") {	
				if((fd = (open(condition.commands.at(i + 1).ar[0], O_RDWR | O_CREAT | O_TRUNC, 0644))) == -1)
					perror("open");
			}
			else {
				if((fd = (open(condition.commands.at(i + 1).ar[0], O_RDWR | O_CREAT | O_APPEND, 0644))) == -1)
					perror("open");
			}
		}
		else if(condition.types.at(i) == "<") {
			if(close(0) == -1)
				perror("close"); 
			if((fd = (open(condition.commands.at(i + 1).ar[0], O_RDONLY))) == -1)
				perror("open");
		}
		i++;
		cout << fd << endl;
	}
	if(execvp(condition.commands.at(0).ar[0], condition.commands.at(0).ar) == -1) { 
		perror("execvp");
		exit(-1);
	}
	exit(0);
	}
	wait(NULL);
}
void nullify(redir &condition) {
	for(unsigned int i = 0; i < condition.commands.size() - 1; i++) {
		for(int j = 0; j < condition.commands.at(i).sz; j++) {
			condition.commands.at(i).ar[j] = NULL;	
		}	 	
	}
	condition.places.clear();
	condition.types.clear();
	condition.ofiles.clear();
}
void p_redir_action(redir &condition, unsigned int index) {
	if(condition.types.at(index) == "end") { 
		cout << "BASE CASE\n";
		return;  
	}
	int fdid[2];
	//cout << prev_fd;
	if(pipe(fdid) == -1) 
		perror("pipe");
	pid_t pid1 = fork();
	if(pid1 < 0) {
		perror("first fork");
		exit(1);
	}
	if(pid1 == 0) {
		dup2(fdid[1], 1);
		close(fdid[0]);
		close(fdid[1]);
		execvp(condition.commands.at(index).ar[0], condition.commands.at(index).ar);
		exit(0);
	}
	pid_t pid2 = fork();
	if(pid2 < 0) {
		perror("second fork");
		exit(1);
	}
	if(pid2 == 0) {
		//dup2(fdid[0], 0);
		close(fdid[0]);
		close(fdid[1]);
		execvp(condition.commands.at(index + 1).ar[0], condition.commands.at(index + 1).ar);
	//	prev_fd = fdid[0];
		p_redir_action(condition, index);
		exit(0);
	}
	close(fdid[0]);
	close(fdid[1]);
	waitpid(pid2, NULL, 0);
	waitpid(pid1, NULL, 0);
	index++;
}

void redir_action(redir &condition) { 
	if(condition.types.at(0) == "|") {
		p_redir_action(condition, 0);
	}
	else {
		io_redir_action(condition);	
	} 
}
