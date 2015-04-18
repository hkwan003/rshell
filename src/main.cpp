#include <sstream>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "errno.h"
#include <string>
#include <list>
#include <string.h>
#include <vector>
#include <cstdlib>

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
    char* host = (char*)malloc(500);
    string userinfo;

    //error checks the user
    if(getlogin() != NULL)
    {
        userinfo = getlogin();
    }
    else
    {
        perror("error getting user info");
    }

    if(gethostname(host, 500) == -1)
    {
        perror("Error getting host name");
    }
    //outputs the userinfo with login and host 
    cout << userinfo << "@" << host << "$";
    //////////////////////////////////////////////login part done, next is all shell commands 
    char prompt_holder[50000];//orginal array to hold prompt
    char *token;//used to break up using string tokenizer
    char *comd_arr[50000];
    unsigned int comd_arr_cnt = 0;

    string converter;                           //converts all bits of string into one piece
    string to_be_tokenized;
    getline(cin, to_be_tokenized);
    for(unsigned int x = 0; x < to_be_tokenized.size(); x++)
    {
        prompt_holder[x] =  to_be_tokenized.at(x);
    }
    prompt_holder[to_be_tokenized.size()] = '\0';
    possible_connector(to_be_tokenized ,to_be_tokenized.size());//function to see pos of connectors

    token = (prompt_holder);
    while(token != NULL)
    {
        cout << *token << endl;
        comd_arr[comd_arr_cnt] = token;
        comd_arr_cnt++;
        token = strtok(NULL, "|; &");
    }
    
    for(unsigned int x  = 0; x < comd_arr_cnt; x++)
    {
        cout << comd_arr[x] << " "; 
    }
    
    int i = fork();
    if(i == 0)
    {
        if(-1 == execvp(comd_arr[0], comd_arr))
        {
            perror("There is a problem with the prompt entered");
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

exekcvp([ls],[[ls],[-a]])
char* argumentList[50000];
char executable[50000];
argumentList[0] = executable;
unsigned int size = 0;
ls -a \0
execvp(argumentList[0], argumentList);
*/


    
