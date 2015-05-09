#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
struct redir {
	bool redir_x;
	vector<int> places;
	vector<string> types;
	vector<string> ofiles;
	char *com_flags[10000];
	int b;
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
	for(unsigned int i = 0; i < sub_str.size() - 1; i++) {
		if(sub_str.at(i) == '<') {
			condition.redir_x = true;
			condition.places.push_back(i);
			condition.types.push_back(sub_str.substr(i, 1));
		}
		else if(sub_str.at(i) == '|' && sub_str.at(i + 1) != '|') {
			condition.redir_x = true;
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
	sub = new char[sub_str.size()];
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
	char *subb;
	subb = new char[sub_str.substr(0, condition.places.at(0)).size()];
	strcpy(subb, sub_str.substr(0, condition.places.at(0)).c_str());
	char *sub_y = strtok(subb, " <>|");
	for(int j = 0; j < condition.b; j++) {
		cout << condition.com_flags[j] << endl;
		condition.com_flags[j] = NULL;
	}

	condition.b = 0;
	while(sub_y != NULL) {
		condition.com_flags[condition.b] = sub_y;
		sub_y = strtok(NULL, " <>|");
		condition.b++;
	}
}

void o_redir_action(redir &condition) {
	int fd;
	if(condition.types.at(0) == ">") {	
		if((fd = open(condition.ofiles.at(1).c_str(), O_RDWR | O_CREAT | O_TRUNC, 0744)) == -1)
			perror("open");
	}
	else {
		if((fd = open(condition.ofiles.at(1).c_str(), O_RDWR | O_CREAT | O_APPEND, 0744)) == -1)
			perror("open");
	}
	if(close(1) == -1)
		perror("close");
	if(dup(fd) == -1) 
		perror("dup");
	if(execvp(condition.com_flags[0], condition.com_flags) == -1) { 
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
	if(execvp(condition.com_flags[0], condition.com_flags) == -1) { 
		perror("execvp");
		exit(-1);
	}
}

void redir_action(redir &condition) {
	if(condition.types.at(0) == "<")
		i_redir_action(condition);
	else if(condition.types.at(0) == ">")
		o_redir_action(condition);
}
