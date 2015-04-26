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
	char *arg;
 	*arg = '.';
	if(NULL == (current = opendir(argv[argc - argc + 1]))) {
		perror("Error in opening directory");
		exit(1);
	}
	struct dirent *filespecs;
	errno = 0;
	filespecs = readdir(current);
	vector<string> directories;
	while(filespecs != NULL) {
		string dire(filespecs->d_name);
		directories.push_back(dire);
		filespecs = readdir(current);
	}
	sort(directories.begin(), directories.end(), alphabetical);
	for(unsigned int i = 0; i < directories.size(); i++) {
		cout << directories.at(i) << " ";
	}
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
