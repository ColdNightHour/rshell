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
	condition.commands.push_back(end);
	condition.types.push_back("end");
	condition.types.push_back("end2");
	condition.types.push_back("end3");
}
void io_redir_action(redir &condition, int &prev_fd, int i) {
	int fd;
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
		prev_fd = fd;
	}
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
void piping_io(redir & condition)  {
	int fdid[2];
	pid_t fid;
	int fd_input = 0;
	int cnt = 0;
	int offset = 0;
	while(condition.commands.at(cnt + offset).sz != 100) {
		pipe(fdid);
		while(condition.types.at(offset) == "<" || condition.types.at(offset) == ">" || condition.types.at(offset) == "<<") 
			offset++;
		if((fid = fork()) == -1) {
			perror("piping fork");
			exit(1);
		}
		else if(fid == 0) {
			//if(condition.types.at(0) != "|") {
		//		dup2(fdid[1], 1);
		//		dup2(fdid[0], 0);
				io_redir_action(condition, fd_input, cnt);
			//}
			if(condition.pip) {
			if(condition.types.at(cnt) == "|" || condition.types.at(cnt + 1) == "|" || condition.types.at(cnt) == "end") {
				cout << "ENTERED HERE A\n";
				dup2(fd_input, 0);
			}
			if(condition.types.at(cnt + 1) == "|" || condition.types.at(cnt) == "|" || condition.types.at(cnt) == "end") {
				cout << "ENTERED HERE B\n";
				if(condition.commands.at(cnt + 1).sz != 100) { 
					cout << "ENTERED HERE C\n";
					dup2(fdid[1], 1);
				}
			}	
			close(fdid[0]);

			}
			if(execvp(condition.commands.at(cnt).ar[0], condition.commands.at(cnt).ar) == -1) 
				perror("execvp");
			exit(0);
		}
		else {
			wait(NULL);
			close(fdid[1]);
			fd_input = fdid[0];
			if(condition.types.at(cnt) != "|" && condition.types.at(cnt + 1) == "|") {
				cnt+=2;
				offset--;
			}
			else {
				cnt++;
			}
		}
	}
}


void redir_action(redir &condition) { 
	piping_io(condition);
}
