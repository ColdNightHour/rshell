#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>

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
void flag_separator(char *argv[], char *files[], char *flags[], int asize) {
	for(int i = 0; i < asize; i++) {
		if(argv[i][0] == '-') {
			flags[i] = argv[i];
			cout << argv[i];
		}
		else
			files[i] = argv[i];
	}
}

