#Rshell bash terminal emulator
Rshell is a simple bash terminal emulator that carries out commands similar to a Unix bash terminal.


##Installation
Installation is simple! Just run the following git commands.
```
$ git clone https://github.com/jdixo001/rshell.git
$ cd rshell
$ git checkout hw0
$ make
$ bin/rshell
```
##Bugs, behaviors, and limitations

* ``$ ls && ``:  This command will ignore the two connectors at the end. This goes for any connector.

* ``$ ls | ls``:  Results in an error rather than reading the commands.

* `` echo "Message && Message"``:  Results in an error due to the presence of a connector.

* ``echo >"This" hello there``:  Does not write to a file but rather echoes what comes after the command.

* ``cat >  FILENAME``:  Does not create a new file, but results in an error.

* When there are odd numbers of false statements connected by ``&&`` the last command will get executed, which isn't all that bad in most cases.

Example-``false && false && ls ; pwd'': prints both ls and pwd, not just pwd.

* ``cd`` not supported.

* The array that holds the arguments and flags holds up to 50000 spots.

* Signals such as ``Ctrl - c`` not supported.

NOTE: Much life real life bugs, bugs in programs and code are hard to find sometimes. If a bug is found let me know, or try to fix it yourself and notify me. Thanks!
