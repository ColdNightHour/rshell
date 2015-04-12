#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
using namespace std;

int main() {
        string userinput = "";
	char hostname[64];
	while(userinput != "exit") {
		cout << getlogin() << "@" << gethostname(hostname, sizeof hostname) <<  "$ ";
		getline(cin, userinput);
	}
	return 0;
}
