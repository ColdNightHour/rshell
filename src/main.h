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
			int t = 1;
			condition.redir_x = true;
			condition.places.push_back(i);
			if(sub_str.at(i - 1) == '1' || sub_str.at(i - 1) == '2' || sub_str.at(i - 1) == '0') {
				t = 2;
				condition.types.push_back(sub_str.substr(i - 1, t));
				i++;
			}
			else {
				condition.types.push_back(sub_str.substr(i, t));
			}
		}
		else if(sub_str.at(i) == '>' && sub_str.at(i + 1) == '>') {
			int t = 2;
			condition.redir_x = true;
			condition.places.push_back(i);
			if(sub_str.at(i - 1) == '1' || sub_str.at(i - 1) == '2' || sub_str.at(i - 1) == '0') {
				t = 3;
				condition.types.push_back(sub_str.substr(i-1, t));
				i+=2;
			}
			else {
				condition.types.push_back(sub_str.substr(i, t));
				i++;
			}
		}
	}
	for(unsigned int i = 0; i < 1; i++) {
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
		//delete[] sub;
	}
	if(condition.places.size() == 0) 
		return;
	int x = 0;
	condition.places.push_back(sub_str.size());
	for(unsigned int i = 0; i < condition.places.size(); i++) {
		char *subb;
		subb = new char[100];
		arr cmd;
		strcpy(subb, sub_str.substr(x, condition.places.at(i) - x).c_str());
		char *sub_y = strtok(subb, " <>|\t");
		int j = 0;
		while(sub_y != NULL) {
			cmd.ar[j] = sub_y;
			sub_y = strtok(NULL, " 012<>|\t");
			j++;
		}
		cmd.ar[j] = sub_y;
		cmd.sz = j;
		condition.commands.push_back(cmd);
		x = condition.places.at(i);
		//delete[] subb;
	}
	int v = 100;
	arr end;
	end.sz = v;
	condition.commands.push_back(end);
	condition.commands.push_back(end);
	condition.types.push_back("end");
	condition.types.push_back("end");
}
void io_redir_action(redir &condition, int &prev_fd, int i, bool determinant, int fdid[], int of) {
	int fd;
	if(condition.types.at(i + of) == "|" && (condition.types.at(i + 1 + of) == ">>" || condition.types.at(i + 1 + of) == ">")) {
		if(close(0) == -1)
			perror("close");
		if(close(1) == -1)
			perror("close after one");
		if(close(fdid[1]) == -1) 
			perror("close after");
		if(condition.types.at(i + 1 + of) == ">") {
			if(open(condition.commands.at(i + 1).ar[0], O_RDWR | O_CREAT | O_TRUNC, 0644) == -1)
				perror("OPEN");
		}
		else if(condition.types.at(i + 1 + of) == ">>") {
			if(open(condition.commands.at(i + 1).ar[0], O_RDWR | O_CREAT | O_APPEND, 0644) == -1)
				perror("OpEn");
		}
		determinant = false;
		if(determinant) {};
	}
	else {
		while (condition.types.at(i) == "<" || condition.types.at(i).find(">>") != string::npos || condition.types.at(i).find(">") != string::npos) {
			if(condition.types.at(i).find(">") != string::npos || condition.types.at(i).find(">>") != string::npos) {
				if(condition.types.at(i).at(0) == '0' || condition.types.at(i).at(0) == '1' || condition.types.at(i).at(0) == '2') {
					int t = condition.types.at(i).at(0) - 48;
					if(close(t) == -1)
						perror("close");
				}
				else {
					if(close(1) == -1)
						perror("close");
				}
				if(condition.types.at(i).find(">") != string::npos) {	
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
	int of = 0;
	bool determinant = true;
	while(condition.commands.at(cnt + offset).sz != 100) {
		if(pipe(fdid) == -1)
			perror("pipe");
		while(condition.types.at(offset) == "<" || condition.types.at(offset).find(">") != string::npos || condition.types.at(offset).find(">>") != string::npos) 
			offset++;
		if((fid = fork()) == -1) {
			perror("piping fork");
			exit(1);
		}
		else if(fid == 0) {
			io_redir_action(condition, fd_input, cnt, determinant, fdid, of);
			if(condition.pip) {
				if(condition.types.at(cnt) == "|" || condition.types.at(cnt + 1) == "|" || condition.types.at(cnt) == "end") {
					if(dup2(fd_input, 0) == -1)
						perror("dup2");
				}
				if(condition.types.at(cnt + 1) == "|" || condition.types.at(cnt) == "|" || condition.types.at(cnt) == "end") {
					if(condition.commands.at(cnt + offset + 1).sz != 100 && determinant) {
						if(dup2(fdid[1], 1) == -1)
							perror("dup2");
					}
				}
			}
			if(close(fdid[0]) == -1)
				perror("close");
			if(execvp(condition.commands.at(cnt).ar[0], condition.commands.at(cnt).ar) == -1) 
				perror("execvp");
			exit(0);
		}
		else {
			if(wait(NULL) == -1)
				perror("wait"); 
			if(close(fdid[1])  == -1)
				perror("close");
			fd_input = fdid[0];
			if(condition.types.at(cnt) != "|" && condition.types.at(cnt + 1) == "|") {
				cnt+=2;
				offset--;
				of = -1;
			}
			else {
				if(condition.types.at(cnt) == "|" && (condition.types.at(cnt + 1) == ">>" || condition.types.at(cnt + 1) == ">"))
					offset++;
				cnt++;
			}
		}
	}
}
void redir_action(redir &condition) { 
	piping_io(condition);
}
