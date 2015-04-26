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
	char *flags[10000]; //will hold -a, -l, -R flags
	char *files[10000]; //will hold current directory and other directories
	flag_separator(argv, files, flags, argc); //separates flags from directories
	if(NULL == (current = opendir(files[0]))) {
		perror("Error in opening directory");
		exit(1);
	}
	struct dirent *filespecs;
	errno = 0;
	filespecs = readdir(current);
	vector<string> directories; //vector holds directories for display
	vector<string> flags; //holds flag for action use
	std::vector<string>::iterator flag_a;
	while(filespecs != NULL) {
		string dire(filespecs->d_name);
		directories.push_back(dire);
		filespecs = readdir(current);
	}
	sort(directories.begin(), directories.end(), alphabetical);
	for(unsigned int i = 0; i < directories.size(); i++) {
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
