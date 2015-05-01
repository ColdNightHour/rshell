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

void flag_separator(char *argv[], vector<string> &file_param, string &sflags, int asize, bool &flag) {
	for(int i = 0; i < asize; i++) {
		if(strchr(argv[i],'-')) {
			string y(argv[i]);
			if(y.find("a") == string::npos && y.find("b") == string::npos && y.find("R") == string::npos)
				flag = false;
			sflags+=y;
		}
		else {
			string param(argv[i]);
			file_param.push_back(param);
		}
	}
}

void deallocator(char *x[], int sz) {
	for(int i = 0; i < sz; i++)
		delete [] x[i];
	delete []x;
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
	cout << buffer << " ";
}

void l_flag(struct stat file, string &permissions) {
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
	cout << user->pw_name << " ";
	if((group = getgrgid(file.st_gid)) == NULL) {
		perror("getgrgid");
		exit(1);
	}
	cout << group->gr_name << " ";
	cout << file.st_size << " ";
	time_converter(file.st_mtime);
	permissions = "";
}
void Path_Creator(vector<string> &file_param, string &path, string folder, int x) {
	path = file_param.at(0) + "/" + file_param.at(x) + "/" + folder;
}
void Color(struct stat file, string x) {
	if(x.at(0) == '.')
		cout << hidden << x << "/\033[0m\033[0m";
	else if(S_ISDIR(file.st_mode))
		cout << dir << x << " " << "\033[0m";
	else if(S_ISREG(file.st_mode) && (file.st_mode & S_IXUSR))
		cout << exec << x << " " << "\033[0m";
	else 
		cout << x << " ";
}
void R_flag(string path, struct stat file, string vflags, string fl) {
	if(!S_ISDIR(file.st_mode) || fl == "." || fl == "..")
		return;
	if(path.find("./../") != string::npos)
		path.erase(0, 2);
	cout << endl << endl << path.substr(2, path.size() - 1)  << ":"<< endl;
	DIR *curr;
	if(NULL == (curr = opendir(path.c_str()))) {
		perror("Error in opening directory");
		exit(1);
	}
	struct dirent *filespecs;
	errno = 0;
	filespecs = readdir(curr);//
	vector<string> directories; //vector holds directories for display
	while(filespecs != NULL) {
		string dire(filespecs->d_name);
		directories.push_back(dire);
		filespecs = readdir(curr);
	}
	if(directories.size() == 0) return;
	sort(directories.begin(), directories.end(), alphabetical);
	if(errno) {
		perror("Error in reading directory");
		exit(1);
	}
	string permissions;
	for(unsigned int i = 0; i < directories.size(); i++) {
		struct stat fle;
		string mpath = path + "/" + directories.at(i); 
		if(vflags.find("a") == string::npos && directories.at(i).at(0) == '.');
		else {
			if(stat(mpath.c_str(), &fle) == -1)
				perror("stat");
			if(vflags.find('l') != string::npos) {
				l_flag(fle, permissions);
				cout << directories.at(i) << endl;
			}
			else
				cout << directories.at(i) << " ";
		}
	}
	for(unsigned int i = 0; i < directories.size(); i++) {
		if(vflags.find("a") == string::npos && directories.at(i).at(0) == '.');
		else	{
			string npath =  path + "/" + directories.at(i);
			struct stat fle;
			if(stat(npath.c_str(), &fle) == -1)
				perror("stat");
			R_flag(npath, fle, vflags, directories.at(i));
		}
	}
	if(-1 == closedir(curr)) {
		perror("Error in closing the directory");
		exit(1);
	}

}
