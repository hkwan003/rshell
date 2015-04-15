#include <sstream>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "errno.h"
#include <string>
#include <list>
#include <string.h>
#include <vector>


using namespace std;





//Global Variable vectors
vector<int> connect_Pos; //holds the positions of possible connectors in prompt
vector<int> kinds_of_connect; //holds what kind of connector are in prompt


void possible_connector(string prompt,int size)
{
for(int x = 0; x < size; x++)
    {
        if(prompt.at(x)  == ';')
        {
            connect_Pos.push_back(x);
            kinds_of_connect.push_back(0);
        }
        else if(prompt[x] ==  '|' && (x + 1 < size &&  prompt[x + 1] == '|')) 
        {
            connect_Pos.push_back(x);
            kinds_of_connect.push_back(1);
            kinds_of_connect.push_back(1);
            x++;
        }
        else if(prompt[x] == '&' && (x + 1 < size && prompt[x + 1] == '&'))
        {
            connect_Pos.push_back(x);
            kinds_of_connect.push_back(2);
            kinds_of_connect.push_back(2);
            x++;

        }
        else
        {
            kinds_of_connect.push_back(-1);        
        }
    }
}



int main(int argc, char **argv)
{
    

    vector <string> Vctr_of_commands;
    char prompt_holder[50000];//orginal array to hold prompt
    char *token;//used to break up using string tokenizer


    cout  << "$ ";
    string converter;                           //converts all bits of string into one piece
    string to_be_tokenized;
    getline(cin, to_be_tokenized);
    for(unsigned int x = 0; x < to_be_tokenized.size(); x++)
    {
        prompt_holder[x] =  to_be_tokenized.at(x);
    }
    prompt_holder[to_be_tokenized.size()] = '\0';

    possible_connector(to_be_tokenized ,to_be_tokenized.size());

    token = (prompt_holder);
    cout << "output what is suppose to be tokenized" << endl;
    while(token != NULL)
    {
        cout << token << endl;
        char foo[10000] = {*token};
        string s(foo);
        cout << "output strings: " << s << endl;
        Vctr_of_commands.push_back(s);
        cout<< "What is suppose to output: "  << Vctr_of_commands[0] << endl;
        token = strtok(NULL, "|;&");
    }



    string finished_tok = strtok(prompt_holder, " &|;");
    
    int i = fork();
    if(i == 0)
    {
        if(-1 == execvp(prompt_holder, argv))
        {
            perror("execvp");
        }
    }
    else
    {
        cout << "hello world" << endl;
    }

    for(unsigned int x = 0; x < connect_Pos.size() ; x++)
    {
        cout << "Connect_pos: " << connect_Pos.at(x) << endl;
    }

    for(unsigned int x = 0; x < kinds_of_connect.size() ; x++)
    {
        cout << "kinds of connect: " << kinds_of_connect.at(x) << endl;
    }



}

/* || && ;

succeeds ||
notsucceed &&

ls -a

[ls] [-a]

exekcvp([ls],[[ls],[-a]])
char* argumentList[50000];
char executable[50000];
argumentList[0] = executable;
unsigned int size = 0;
ls -a \0
execvp(argumentList[0], argumentList);
*/


    
