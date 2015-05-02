#include <stdlib.h>
#include <sstream>
#include <dirent.h>
#include <grp.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <vector>
#include <iomanip>
using namespace std;

char dir[] = {"\033[1;34m"};
char hidden[] = {"\033[1;34m\033[1;40m"};
char exec[] = {"\033[1;32m"};

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
void flag_separator(char *argv[], vector<string> &file_param, string &sflags, int asize, bool &flag, bool &predir) {
	for(int i = 0; i < asize; i++) {
		if(strchr(argv[i],'-')) {
			string y(argv[i]);
			sflags+=y;
		}
		else {
			string param(argv[i]);
			file_param.push_back(param);
			if(param == "..")
				predir = true;
		}
	}
	cout <<sflags<<endl;
	if(sflags != "") {
		for(unsigned int i = 1; i < sflags.size(); i++) {
			if(sflags.at(i) != 'a' && sflags.at(i) != 'l' && sflags.at(i) != 'R' && sflags.at(i) != '-') 
				flag = false;
		}
	}
}
void g_rwx(string &permissions, struct stat file) {
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
	cout << permissions << " ";
}
void time_converter(time_t x) {
	struct tm timeinfo;
	localtime_r(&x, &timeinfo);
	char buffer[80];
	strftime((char*)& buffer, 80, " %b %d %H:%M", &timeinfo);
	string y(buffer);
	cout << setw(5) << right << buffer << " ";
}
void l_flag(struct stat file, string &permissions, int sz) {
	u_rwx(permissions, file);
	g_rwx(permissions, file);
	o_rwx(permissions, file);
	struct passwd *user;
	struct group *group;
	cout << file.st_nlink << " ";
	if((user = getpwuid(file.st_uid)) == NULL) {
		perror("getpwuid");
		exit(1);
	}
	string pwn(user->pw_name);
	cout << setw(pwn.size()) << right << user->pw_name << " ";
	if((group = getgrgid(file.st_gid)) == NULL) {
		perror("getgrgid");
		exit(1);
	}
	string num;
	ostringstream convert;
	convert << sz;
	num = convert.str();
	cout << group->gr_name << " ";
	cout << setw(num.size()) << right << file.st_size << " ";
	time_converter(file.st_mtime);
	permissions = "";
}
string Path_Creator(vector<string> &file_param, string folder, int x) {
	return (file_param.at(0) + "/" + file_param.at(x) + "/" + folder);
}
string R_path(vector<string> &file_param, int x) {
	return (file_param.at(0) + "/" + file_param.at(x));
}
void Color(struct stat file, string x) {
	if(x.at(0) == '.')
		cout << hidden << x  << " " << "\033[0m\033[0m";
	else if(S_ISDIR(file.st_mode))
		cout << dir << x << " " << "\033[0m";
	else if(S_ISREG(file.st_mode) && (file.st_mode & S_IXUSR))
		cout << exec << x << " " << "\033[0m";
	else 
		cout << x << " ";
}
void Size_Find(struct stat file, int &sz) {
	if(file.st_size > sz)
		sz = file.st_size;
}
void dots(bool predir, string vflags){
	int tot = 0;
	string dots = ".";
	if(predir)
		dots = "..";
	DIR *current;
	if(NULL == (current = opendir(dots.c_str()))) {
		perror("Error in opening directory");
		exit(1);
	}
	struct dirent *filespecs;
	filespecs = readdir(current);
	vector<string> directories;
	int size = 0;
	cout << dots << ":" << endl;
	while(filespecs != NULL) {
		string dire(filespecs->d_name);
		directories.push_back(dire);
		filespecs = readdir(current);
		struct stat file;
		if(stat(dire.c_str(), &file) == -1)
			perror("stat");
		Size_Find(file, size);
	}
	struct stat fle;
	string permissions;
	for(unsigned int i = 0; i < directories.size(); i++) {
		if(vflags.find("a") == string::npos && directories.at(i).at(0) == '.');
		else {
			if(stat(directories.at(i).c_str(), &fle) == -1)
				perror("stat");
			if(vflags.find('l') != string::npos) {
				l_flag(fle, permissions, size);
				Color(fle, directories.at(i));
				tot += fle.st_blocks;
				cout << endl;
			}
			else
				Color(fle, directories.at(i));
		}
	}
	if(vflags.find("l") != string::npos)
		cout << "total " << tot / 2 << endl;
	if(-1 == closedir(current)) {
		perror("Error in closing the directory");
		exit(1);
	}
}
void R_flag(string path, struct stat file, string vflags, int sz) {
	//cout << "Enter\n";
	if(!(S_ISDIR(file.st_mode))) {
		return;
	}
	//cout << fl << endl;
	if(path.find("./../") != string::npos)
		path.erase(0, 2);
	cout << endl << endl << path << ":";
	DIR *curr;
	//cout << "open dir \n";
	if(NULL == (curr = opendir(path.c_str()))) {
		perror("opendir");
		exit(1);
	}
	struct dirent *filespecs;
	errno = 0;
	int size = 0;
	filespecs = readdir(curr);//
	vector<string> directories; //vector holds directories for display
	//cout << "readdir loops \n";
	while(filespecs != NULL) {
		if(errno) {
			perror("Error in reading directory");
			exit(1);
		}
		string dire(filespecs->d_name);
		directories.push_back(dire);
		filespecs = readdir(curr);
		string opath;
		opath = path + "/" + dire;
		struct stat file;
		if(stat(opath.c_str(), &file) == -1)
			perror("stat");
		Size_Find(file, size);
	}
	cout << endl;
	if(directories.size() == 0) return;
	sort(directories.begin(), directories.end(), alphabetical);
	if(errno) {
		perror("Error in reading directory");
		exit(1);
	}
	errno = 0;
	string permissions;
	int tot = 0;
	//cout << "output loop \n";
	for(unsigned int i = 0; i < directories.size(); i++) {
		struct stat fle;
		string mpath = path + "/" + directories.at(i); 
		if(vflags.find("a") == string::npos && directories.at(i).at(0) == '.');
		else {
			if(stat(mpath.c_str(), &fle) == -1)
				perror("stat");
			if(vflags.find('l') != string::npos) {
				l_flag(fle, permissions, sz);
				Color(fle, directories.at(i));
				tot += fle.st_blocks;
				cout << endl;
			}
			else {
				Color(fle, directories.at(i));
			}
		}
	}
	if(vflags.find("l") != string::npos)
		cout << "total " << tot / 2 << endl;
	//cout << "\n recursive loop \n";
	directories.erase(directories.begin(), directories.begin() + 2);
	if(directories.size() == 0)
		return;
	for(unsigned int i = 0; i < directories.size(); i++) {
		if(vflags.find("a") == string::npos && directories.at(i).at(0) == '.');
		else	{
			string npath = path + "/" + directories.at(i);
			struct stat fil;
			//cout << npath << endl;
			if(stat(npath.c_str(), &fil) == -1)
				perror("stat");
			R_flag(npath, fil, vflags, size);
		}
	}
	if(-1 == closedir(curr)) {
		perror("Error in closing the directory");
		exit(1);
	}
}
