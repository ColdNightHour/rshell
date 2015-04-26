#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>

using namespace std;

int main(int argc, char *argv[]) {
	struct stat s;
	DIR *current;
	if(NULL == (current = opendir(argv[i]))) {
		perror("Error in opening directory");
		exit(1);
	}
	struct dirent *filespecs;
	int result;
	errno = 0;
	while(result == (filespecs = readdir(current))) {
		cout << filespecs->d_name << " ";
	}
	return 0;
}
