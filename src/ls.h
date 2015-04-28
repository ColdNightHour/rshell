#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

bool alphabetical(string first, string second) {
	transform(first.begin(), first.end(), first.begin(), ::tolower);
	transform(second.begin(), second.end(), second.begin(), ::tolower);
	if(first.at(0) == '.')
		return lexicographical_compare(first.begin() + 1, first.end(), 
			second.begin(), second.end());
	else if(second.at(0) == '.')
		return lexicographical_compare(first.begin(), first.end(),
			second.begin() + 1, second.end());
	else if(first.at(0) == '.' && second.at(0) == '.')
		return lexicographical_compare(first.begin() + 1, first.end(),
			second.begin() + 1, second.end());
	else 
		return lexicographical_compare(first.begin(), first.end(),
			second.begin(), second.end());
}
void flag_separator(char *argv[], char *files[], char *flags[], int asize, int &flagsz) {
	int file = 0;
	for(int i = 0; i < asize; i++) {
		if(strchr(argv[i],'-')) {
			flags[flagsz] = argv[i];
			flagsz++;
		}
		else {
			files[file] = argv[i];
			file++;
		}
	}
}
void deallocator(char *x[], int sz) {
	for(int i = 0; i < sz; i++)
		delete [] x[i];
	delete []x;
}
void flag_con(string &sflags, char *flags[], int flagsz) {
	for(int i = 0; i < flagsz; i++) {
		char *d = flags[i];
		string y(d);
		sflags+=y;
	}
}
/*
S_ISREG(m)  is it a regular file?
S_ISDIR(m)  directory
S_ISCHR(m)  character device?
S_ISBLK(m)  block device?
*/
void g_rwx(string &permissions, struct stat file) {
	//S_IRWXG --> 00070 --> 000 000 111 000 --> 000 000 rwx 000
	(file.st_mode & S_IRGRP) ? permissions += "r" : permissions += "-";
	(file.st_mode & S_IWGRP) ? permissions += "w" : permissions += "-";
	(file.st_mode & S_IXGRP) ? permissions += "x" : permissions += "-";
}
void u_rwx(string &permissions, struct stat file) {
	if (file.st_mode & S_IFREG) permissions += "-";
	else if (file.st_mode & S_IFDIR) permissions += "d"; 
	else if (file.st_mode & S_IFCHR) permissions += "c"; 
	else if (file.st_mode & S_IFBLK) permissions += "b"; 
	(file.st_mode & S_IRUSR) ? permissions += "r" : permissions += "-";
	(file.st_mode & S_IWUSR) ? permissions += "w" : permissions += "-";
	(file.st_mode & S_IXUSR) ? permissions += "x" : permissions += "-";
}
void o_rwx(string &permissions, struct stat file) {
	(file.st_mode & S_IROTH) ? permissions+= "r" : permissions += "-";
	(file.st_mode & S_IWOTH) ? permissions+= "w" : permissions += "-";
	(file.st_mode & S_IXOTH) ? permissions+= "x" : permissions += "-";
	cout << permissions;
}
