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
void vec_con(char *files[], vector<string> &file_param, bool &home) {
	int i = 0;
	while(files[i] != NULL) {
		string param(files[i]);
		if(param.find("home"))
			home = true;
		file_param.push_back(param);
		i++;
	}

}
void flag_separator(char *argv[], char *files[], string &sflags, int asize) {
	int file = 0;
	for(int i = 0; i < asize; i++) {
		if(strchr(argv[i],'-')) {
			string y(argv[i]);
			sflags+=y;
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

void l_flag(string path, struct stat file, string &permissions) {
	if(stat(path.c_str(), &file) == -1)
		perror("stat");
	u_rwx(permissions, file);
	g_rwx(permissions, file);
	o_rwx(permissions, file);
	struct passwd *user;
	struct group *group;
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
void Path_Creator(vector<string> &file_param, string &path, string folder) {
	for(unsigned int i = 0; i < file_param.size(); i++) {
			path += file_param.at(i);	
			path += "/";
	}
	path += folder;
}
void R_flag(vector<string> directories, string vflags) {
	DIR *current;
	char *files[10000]; //will hold current directory and other directories
	vector<string> file_param;
	string path;
/*		if(NULL == (current = opendir(file_param.at(0).c_str()))) {
			perror("Error in opening directory");
			exit(1);
		}
	}
	else {
		if(NULL == (current = opendir(file_param.at(1).c_str()))) {
			perror("Error in opening directory");
			exit(1);
		}
	}*/
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
}
