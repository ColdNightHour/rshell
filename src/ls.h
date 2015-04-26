#include <iostream>
#include <algorithm>
#include <string>

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

