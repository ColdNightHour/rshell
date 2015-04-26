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
	int flagsz = 0;
	char *flags[10000]; //will hold -a, -l, -R flags
	char *files[10000]; //will hold current directory and other directories
	flag_separator(argv, files, flags, argc, flagsz); //separates flags from directories
	if(NULL == (current = opendir(files[0]))) {
		perror("Error in opening directory");
		exit(1);
	}
	struct dirent *filespecs;
	errno = 0;
	filespecs = readdir(current);
	vector<string> directories; //vector holds directories for display
	string vflags; //holds flag for action use
	flag_con(vflags, flags, flagsz); //puts flags from array into string to have easy use
	while(filespecs != NULL) {
		string dire(filespecs->d_name);
		directories.push_back(dire);
		filespecs = readdir(current);
	}
	struct stat file;

	sort(directories.begin(), directories.end(), alphabetical);
	for(unsigned int i = 0; i < directories.size(); i++) {
		if(vflags.find("a") == string::npos && directories.at(i).at(0) == '.');
		else {
			stat(directories.at(i).c_str(), &file);
			cout << file.st_size << endl;
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
