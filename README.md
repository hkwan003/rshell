# Linux ls command

## Ls functionality

Rshell is a c++ shell that is meant to replicate the same one used in UNIX. It only has basic functionalties for now but more will be added onto it later.

* A command prompt
* Recognition of various commands
* Exit functionality
* Clear functionality
* User login recognition
* Comment recognition

## How to use Ls

* First compile the program by typing ``make``
* After compiling, type in ``bin/ls``
* Special flags like -a, -l, and -R could be appended after typing in ``bin/ls``
* The program can also handle directories with the flags that can be appended behind it
``bin/ls -a src -R. -l``

##Limitations
* Commands has a limit of 50,000 characters
* host have a name character limit of 500 characters
* cd does not work in this shell
