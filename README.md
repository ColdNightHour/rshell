#Rshell Bash emulator
Rshell is a simple bash emulator that carries out commands similar to a Unix bash terminal.
##Installation
Installation is simple. Just run the following git commands.
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

* When there are even numbers of false statements connected by ``&&`` the last command will get executed, which isn't all that bad in most cases.
