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
	vector<string> file_param;
	bool flag = true;
	bool predir = false;
	flag_separator(argv, file_param, vflags, argc, flag, predir); //separates flags from directories
	string path, x;
	sort(file_param.begin(), file_param.end(), alphabetical);
	if(!flag) {
		cerr << "Error";
		exit(1);
	}
	for(unsigned int indice = 0; indice < file_param.size(); indice++) {
		if(file_param.size() > 1 && indice == 0) indice++; 
		if(NULL == (current = opendir(file_param.at(indice).c_str()))) {
			perror("Error in opening directory");
			exit(1);
		}
		struct dirent *filespecs;
		errno = 0;
		filespecs = readdir(current);//
		vector<string> directories; //vector holds directories for display
		int size = 0;
		while(filespecs != NULL) {
			string dire(filespecs->d_name);
			directories.push_back(dire);
			filespecs = readdir(current);
			string path;
			path = Path_Creator(file_param, dire, indice);
			if(path.find("home") != string::npos) {
				path.erase(0, 2);
			}
			struct stat file;
			if(stat(path.c_str(), &file) == -1)
				perror("stat");
			Size_Find(file, size);
		}
		if(errno) 	{
			perror("Error in reading directory");
			exit(1);                 
		}
		struct stat file;
		string permissions;
		sort(directories.begin(), directories.end(), alphabetical);
		int total = 0;
		if(file_param.size() > 2 && vflags.find("R") == string::npos)
			cout << file_param.at(indice) << ":" << endl;
		//if(vflags.find("R") != string::npos && path.find("home") == string::npos)
		//	dots(predir, vflags);
		if(stat(file_param.at(indice).c_str(), &file) == -1)
			perror("stat");
		if(vflags.find("R") != string::npos) {
			R_flag(file_param.at(indice), file, vflags, size);
		}
		else if(vflags.find("R") == string::npos) {
			for(unsigned int i = 0; i < directories.size(); i++) {
				path = Path_Creator(file_param, directories.at(i), indice); //must change to accomodate optional files
				if(path.find("home") != string::npos) {
					path.erase(0, 2);
				}
				if(stat(path.c_str(), &file) == -1)
					perror("stat");
				if(vflags.find("a") == string::npos && directories.at(i).at(0) == '.');
				else {
					if(vflags.find('l') != string::npos) {
						l_flag(file, permissions, size);
						Color(file, directories.at(i));
						total+=file.st_blocks; 
						cout << endl;
					}
					else {
						cout << path << endl;
						Color(file, directories.at(i));
					}
				}
			}
		}
		if(vflags.find("l") != string::npos && vflags.find("R") == string::npos)
			cout << "total " <<  total/2;
		cout << endl;
		if(-1 == closedir(current)) { 
			perror("Error in closing the directory");
			exit(1);
		}	
	}
	return 0;
}
