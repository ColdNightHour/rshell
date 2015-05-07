#include <errno.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

using namespace std;
void r_recurse(string dir, struct stat buf, bool flag) {
	cout << buf.st_mode << " " << dir << endl;
	if(!flag) {
		cout << "first" << endl;
		if(unlink(dir.c_str()) == -1)
			perror("unlink");
		return;
	}
	if(!(S_ISDIR(buf.st_mode))) {
		cout << "second" << endl;
		if(unlink(dir.c_str()) == -1)
			perror("unlink");
		return;
	}
	DIR *current;
	if(NULL == (current = opendir(dir.c_str()))) {
		perror("opendir");
		exit(1);	
	}
	struct dirent *filespecs;
	errno = 0;
	filespecs = readdir(current);
	if(errno)
		perror("Error in reading directory");
	vector<string> directories;
	while(filespecs != NULL) {
		string dire(filespecs->d_name);
		if(dire != "." && dire != "..") {
			dire = dir + "/" +  dire;
			directories.push_back(dire);
		}
		filespecs = readdir(current);
	}
	if(directories.size() == 0) {
		rmdir(dir.c_str());
		return;
	}
	if(errno) {
		perror("Error in reading directory");
		exit(1);
	}
	struct stat x;
	for(unsigned int i = 0; i < directories.size(); i++) {
		stat(directories.at(i).c_str(), &x);
		r_recurse(directories.at(i), x, flag);
		rmdir(dir.c_str());
	}
	if(-1 == closedir(current)) {
		perror("Error in closing the directory");
		exit(1);
	}
}

int main(int argc, char *argv[]) {
	vector<string> x;
	bool flag = false;
	for(int i = 1; i < argc; i++) {
		string y(argv[i]);
		if(y == "-r")
			flag = true;
		else
			x.push_back(y);
	}
	struct stat buf;
	for(unsigned int i = 0; i < x.size(); i++) {
		stat(x.at(i).c_str(), &buf);
		string c = "./" + x.at(i); 
		r_recurse(c, buf, flag);
	}
	return 0;
}
