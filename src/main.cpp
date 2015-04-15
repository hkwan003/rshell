#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <stdlib.h>   
#include <string.h>
#include <boost/tokenizer.hpp>
#include <unistd.h>

using namespace boost;
using namespace std;

void command_fix(char* command)
{
    char* altered_command[50000];
    for(int i = 0, int x = 0; command[x] != '\0'; ++x, ++j))
    {
        if(command[x] == '#')
        {
            







int main(int argc, char **argv)
{

    vector<string> command;
    cout  << "$ ";
    string converter;                           //converts all bits of string into one piece
    string to_be_tokenized;
    getline(cin, to_be_tokenized);
    char_separator<char> delim("^&| ", ";", keep_empty_tokens);
    tokenizer< char_separator<char>  > mytok(to_be_tokenized, delim); 
 
    for(auto it = mytok.begin(); it != mytok.end(); ++it)
    {
        converter += *it;
    }
    cout << converter << endl;
    converter += '\0';
    const char *cstr = converter.c_str();
    command.push_back(converter);
   int i = fork();
   if(i == 0)
   {
       if(-1 == execvp(cstr, argv))
       {
           perror("execvp");
       }
   }
   else
   {
       cout << "hello world" << endl;
   }







}

/* || && ;

succeeds ||
notsucceed &&

ls -a

[ls] [-a]

execvp([ls],[[ls],[-a]])
char* argumentList[50000];
char executable[50000];
argumentList[0] = executable;
unsigned int size = 0;
ls -a \0
execvp(argumentList[0], argumentList);
*/


    
