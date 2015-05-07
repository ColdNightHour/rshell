#include <stdio.h>
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
		else if(sub_str.at(i) == '>') {
			condition.redir_x = true;
			condition.places.push_back(i);
			condition.types.push_back(sub_str.substr(i, 1));
		}
		else if(sub_str.at(i) == '>' && sub_str.at(i + 1) == '>') {
			condition.redir_x = true;
			condition.places.push_back(i);
			condition.types.push_back(sub_str.substr(i, 2));
		}
	}
	char *sub;
	char *sub_a;
	sub = new char[sub_str.size()];
	strcpy(sub, sub_str.c_str());
	sub_a = strtok(sub, " <>|");
	while(sub_a != NULL) {
		string x(sub_a);
		cout << "THIS" << x << endl;
		condition.ofiles.push_back(x);
		sub_a = strtok(NULL, " <>|");
	}
}

