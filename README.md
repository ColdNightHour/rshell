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
##Rshell: Bugs, behaviors, and limitations

* ``$ ls && ``:  This command will ignore the two connectors at the end. This goes for any connector.

* ``$ ls | ls``:  Results in an error rather than reading the commands.

* `` echo "Message && Message"``:  Results in an error due to the presence of a connector.

* ``echo >"This" hello there``:  Does not write to a file but rather echoes what comes after the command.

* ``cat >  FILENAME``:  Does not create a new file, but results in an error.

* When there are odd numbers of false statements connected by ``&&`` the last command will get executed, which isn't all that bad in most cases

Example-``false && false && ls ; pwd``: prints both ls and pwd, not just pwd.

* ``cd`` not supported.

* The array that holds the arguments and flags holds up to 50000 spots.

* Signals such as ``Ctrl - c`` not supported.

##New Feature, custom ls command
The latest rshell package comes equipped with a stripped down version of ls. This is downloaded the same way as rshell but instead run:

```
git checkout hw1
make
bin/ls
```
and use with any combination of the -l, -a, or -R commands. Now with colors!

##ls: Bugs, behaviors, and limitations
* `` ls #optional file parameter of not a directory``: results in an error, for the optional file parameters must be directories.

* Mutltiple listed files and directories are not in organized columns, and random ``\n`` characters may make output ugly.

* Multiple optional file parameters paired with ``.`` and ``..`` will automatically display ``.`` and ``..`` first.

* Alignment in ``-l`` flag is buggy and will sometimes be unaligned.

* Total number of 512-blocks for ``-l`` is on the bottom, not the top.

NOTE: Much life real bugs, bugs in programs and code are hard to find sometimes. If a bug is found let me know, or try to fix it yourself and notify me. Thanks!
