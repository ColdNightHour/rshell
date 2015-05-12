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
	arr cmd;
	char *subb;
	subb = new char[100];
	strcpy(subb, sub_str.substr(0, condition.places.at(0)).c_str());
	//cout << sub_str.substr(0, condition.places.at(0)) << endl;
	char *sub_y = strtok(subb, " ");
	int i = 0;
	while(sub_y != NULL) {
		cmd.ar[i] = sub_y;
		sub_y = strtok(NULL, " ");
		i++;
	}
	cmd.sz = i;
	condition.commands.push_back(cmd);
}

void o_redir_action(redir &condition) {
	int fd = 0;
	cout << fd << endl;
	if(condition.types.at(0) == ">") {	
		if((fd = open(condition.ofiles.at(1).c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644)) == -1)
			perror("open");
	}
	else {
		if((fd = open(condition.ofiles.at(1).c_str(), O_RDWR | O_CREAT | O_APPEND, 0644)) == -1)
			perror("open");
	}
	if(close(1) == -1)
		perror("close");
	if(dup(fd) == -1) 
		perror("dup");
	
	if(execvp(condition.commands.at(0).ar[0], condition.commands.at(0).ar) == -1) { 
		perror("execvp");
		exit(-1);
	}
}

void i_redir_action(redir &condition) {
	int fd;
	if((fd = open(condition.ofiles.at(1).c_str(), O_RDONLY)) == -1)
		perror("open");
	if(close(0) == -1)
		perror("close");
	if(dup(fd) == -1) 
		perror("dup");
	if(execvp(condition.commands.at(0).ar[0], condition.commands.at(0).ar) == -1) { 
		perror("execvp");
		exit(-1);
	}
}

void p_redir_action(redir &condition, int i, int fdid[]) {
	cout << condition.pip << " " << i << " " << fdid[1];
/*	int fd[2];
	pid_t pid1, pid2;
	char *argv[5];
	pipe(fd);
	pid1 = fork();
	if (pid1<0) {
		perror("First fork() failed!");
		return -1;
	}	
	if (pid1==0) {
		close(1);
		dup(fd[1]);
		close(fd[0]);
		close(fd[1]);
		argv[0] = (char*) malloc(5*sizeof(char));
		argv[1] = (char*) malloc(5*sizeof(char));
		strcpy(argv[0],"ls");
		strcpy(argv[1],"-l");
		argv[2] = NULL;
		fprintf(stderr,"************* Running ls -l *************\n");	
		execvp(argv[0],argv);
		perror("First execvp() failed");
		return -1;
	}
	pid2 = fork();
	if (pid2<0) {
		perror("Second fork() failed!");
		return -1;
	}
	if (pid2==0) {
		close(0);
		dup(fd[0]);
		close(fd[0]);
		close(fd[1]);
		argv[0] = (char*) malloc(5*sizeof(char));
		argv[1] = (char*) malloc(5*sizeof(char));
		strcpy(argv[0],"grep");
		strcpy(argv[1],"pipe");
		argv[2] = NULL;
		fprintf(stderr,"************* Running grep pipe *************\n");
		execvp(argv[0],argv);
		perror("Second execvp() failed");
		return -1;
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1,NULL,0);
	waitpid(pid2,NULL,0);
	printf("************* Father exitting... *************\n");
	return 0; */
}
void redir_action(redir &condition, int i, int fdid[]) {
	if(i == 0) {
		if(condition.types.at(0) == "<")
			i_redir_action(condition);
		else if(condition.types.at(0) == ">" || condition.types.at(0) == ">>")
			o_redir_action(condition);
		exit(0);
	}
	if(condition.types.at(0) == "|" && i == 0)
		p_redir_action(condition, i, fdid);
}
