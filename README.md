# Linux shell for assignment 0 CS100

## Shell Functions
Rshell is a linux terminal written in C++ and it does only basic operations from listing directories to echoing out messages

* A command prompt
* Recognition of various commands
* Exit functionality
* Clear functionality
* User recognition
* Comment recognition 

## How to use rshell
* compile with the prompt ``make``
* A directory will be created called bin which will hold rshell my executable
* One should make clean everytime before executing the directory
* ``make clean``


##Prerequisities

* Knowledge of basic c++
* usage of string tokenizer
* Fork() and exec() command for executing commands

##Bugs
* when first executable is false in &&, the second part executes also, needs to be fixed in the future

##Limitations
* Command has a limit of 50,000 characters
* host have a name character limit of 500 characters
* cd does not work in this shell
 

