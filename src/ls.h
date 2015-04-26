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
	for(int i = 0; i < asize; i++) {
		if(argv[i][0] == '-') {
			flags[i] = argv[i];
			flagsz++;
			cout << flags[i];
		}
		else {
			files[i] = argv[i];
			cout << i;
		}
	}
}
void deallocator(char *x[], int sz) {
	for(int i = 0; i < sz; i++)
		delete [] x[i];
	delete []x;
}
void flag_con(vector<string> &x, char *flags[], int flagsz) {
	for(int i = 0; i < flagsz; i++) {
		string y(flags[i]);
		cout << y;
		x.push_back(y);
	}
}

