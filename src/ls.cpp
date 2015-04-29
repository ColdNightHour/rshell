#include <pwd.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <vector>
#include <algorithm>
#include <string>
#include "ls.h"

using namespace std;

int main(int argc, char *argv[]) {
	DIR *current;
	char *arg = new char[2];
	strcpy(arg, ".");
	argv[0] = arg;
	string vflags;


	char *files[10000]; //will hold current directory and other directories
	vector<string> file_param;
	flag_separator(argv, files, vflags, argc); //separates flags from directories
	string path;
	bool home;
	vec_con(files, file_param, home);    //puts the files themselfs into a vector for easy use
	if(file_param.size() == 1) {
		if(NULL == (current = opendir(file_param.at(0).c_str()))) {
			perror("Error in opening directory");
			exit(1);
		}
	}
	else {
		if(NULL == (current = opendir(file_param.at(1).c_str()))) {
			perror("Error in opening directory");
				exit(1);
                }
	}
	struct dirent *filespecs;
	errno = 0;
	filespecs = readdir(current); //
	vector<string> directories; //vector holds directories for display



	while(filespecs != NULL) {
		string dire(filespecs->d_name);
		directories.push_back(dire);
		filespecs = readdir(current);
	}
	struct stat file;
	string permissions;
	sort(directories.begin(), directories.end(), alphabetical);
				
	for(unsigned int i = 0; i < directories.size(); i++) {
		if(vflags.find("a") == string::npos && directories.at(i).at(0) == '.');
		else {
			if(vflags.find('l') != string::npos) {
				Path_Creator(file_param, path, directories.at(i));
				if(path.find("home") != string::npos) 
					l_flag(path.substr(2, path.size() - 1), file, permissions);
				else 
					l_flag(path, file, permissions);
				cout << directories.at(i) << endl;
				path = "";
			}
			else
				cout << directories.at(i) << " ";
		}
	}
	cout << endl;
	if(errno != 0) {
		perror("Error in reading directory");
		exit(1);
	}
	if(-1 == closedir(current)) { 
		perror("Error in closing the directory");
		exit(1);
	}
	return 0;
}
