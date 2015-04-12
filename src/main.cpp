#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>

using namespace std;

int main() {
        string userinput = "";
	char hostname[64];
	if(gethostname(hostname, sizeof hostname))
		perror("failed to obtain hostname");
	while(userinput != "exit") {
		cout << getlogin() << "@" << hostname <<  "$ ";
		getline(cin, userinput);
	}
	return 0;
}
