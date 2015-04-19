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
##Bugs, behaviors, anmd limitations
``$ ls && ``
The above command will ignore the two connectors at the end. This goes for any connector.
``$ ls | ls``
The above command results in an error rather than reading the commands.
`` echo "Message && Message"``
The above command results in an error due to the presence of a connector.
