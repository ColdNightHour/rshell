#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
using namespace std;

int main() {
        string userinput = "";
	while(userinput != "exit") {
		cout << getlogin() << "$";
		getline(cin, userinput);
	}
	return 0;
}
