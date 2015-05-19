# Rshell Linux command

## Rshell functionality

Rshell is a c++ shell that is meant to replicate the same one used in UNIX. It only has basic functionalties for now but more will be added onto it later.

* A command prompt
* Recognition of various commands
* Exit functionality
* Clear functionality
* User login recognition
* Comment recognition

## How to use piping 

* First compile the program by typing ``make``
* After compiling, type in ``bin/rshell`
* Type in ``make clean`` to clear out the old bin
* ``make cp`` compiles the code for cp
* ``make rm`` compiles the code for rm
* ``make ls`` compiles ls for you
* ``make mv`` compiles mv command
* Input redirection < input redirection into a program like wc ``Word Count``
* Output redirection >, >>,`` >`` truncates orginal file, ``>>`` appends onto what was already in folder
* ls | cat, redirects the output of ls into input end of cat
* input and output redirection can be combined ``wc < file > file 1 >> file2``

## Bugs

* Multiple piping does not work
* piping only works when it is provide with two arguments
* the ``<<<`` will only work by itself, will not work with any other command
* The ``1>, 1>>, 2>, 2>>`` will also only work by itself.
* The only commands that can be combined together is input and output redirection



##Limitations
* Commands has a limit of 50,000 characters
* host have a name character limit of 500 characters
* every command in the piping assignment other than input and output redirection can only be used by itself
* piping can only take in two arguments
* <<< command only works by itself
* file descriptor change to stderr only works when used by itself
* `` cat < existingInputFile | tr A-Z a-z | tee newOutputfile1 | tr a-z A-Z > newOutputFile2`` this does not work
* Make file is not attached to other functions like rm, cp, or ls. Will fix in future so make file and deal with other functionality

##Sidenote
* I have two script files for piping assignment. One is long while the second one is short, I did it just to test one of the extra credit more. Please take a look
