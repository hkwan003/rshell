# Linux shell for assignment 0 CS100

## Shell Functions
Rshell is a linux terminal written in C++ and it does only basic operations from listing directories to echoing out messages

* A command prompt
* Recognition of various commands
* Exit functionality
* Clear functionality
* User login recognition
* Comment recognition

## How to use Rshell
* compile witht he prompt ``make``  
* A directory will be created called bin which will hold the rshell executable
* One should make clean everytime before executing the directory
* ``make clean``

##Prerequisities

* Knowledge of basic C++
* Usage of string tokenizer
* Fork() and Exec() command for executing commands

##Bugs
* when the first executable is false in &&, the seond part excutes as well, but needs to be fixed in the future
* The connector | one or symbol is not recognized and does not run with ls in front of it



##Prerequisities

* Knowledge of basic c++
* usage of string tokenizer
* Fork() and exec() command for executing commands


##Limitations
* Command has a limit of 50,000 characters
* host have a name character limit of 500 characters
* cd does not work in this shell
 

