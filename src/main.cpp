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
#include <sys/wait.h>




using namespace std;

void fixing_spacing_command(char *org_prompt)
{
    char *finished_prompt[50000];
    //x is passed in prompt
    //i is finished prompt after changing spaces
    for(int x = 0, int i = 0; org_prompt[x] != '\0'; x++, i++)
    {
        if(org_prompt[x] == '#')
        {
            org_prompt[x] = '\0';
            finished_prompt[i] = '\0';
        }
        else if(org_prompt[x] == ';')
        {
            finished_prompt[i] = ' ';
            finished_prompt[++i] = ';';
            finished_prompt[++i] = ' ';
        }
        else if(org_prompt[x] == '&' && org_prompt[x] == '&')
        {
            finished_prompt[i] = ' ';
            finished_prompt[++i] = '&';
            finished_prompt[++i] = '&';
            finished_prompt[++i] = ' ';
            ++i;
        }
        else if(org_prompt[x] == '|' && org_prompt[x] == '|')
        {
            finished_prompt[i] = ' ';
            finished_prompt[++i] = '|';
            finished_prompt[++i] = '|';
            finished_prompt[++i] = ' ';
            ++i;
        }
        else
        {
            finished_prompt[i] = org_prompt[x];
        }
        if(org_prompt[x + 1] == '\0') finished_prompt[i + 1] = '\0';
    }
    strcpy(org_prompt, finished_prompt);



int main(int argc, char **argv)
{
    int status;
    char* host = (char*)malloc(500);
    string userinfo;
    bool prompter = true;
    char* token;            //will be bit of code strtok cuts off

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
    while(prompter)
    {
        cout << userinfo << "@" << host << " $ ";
        //////////////////////////////////////////////login part done, next is all shell commands 
        int connect_check = ;           //holds to see what kind of connectors I have 
        char prompt_holder[50000];//orginal array to hold prompt
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
        fixed_spacing_command(prompt_holder);
        token = strtok(prompt_holder, "\t ";
        while(token != NULL && prompter)
        {
            if(!strcmp(token, ";")) connect_check = 0;
            else if(!strcmp(token, "||")) connect_check = 1;
            else if(!strcmp(token, "&&")) connect_check = 2;
            connect_check = -1;

            if(connect_check == -1)
            {
                comd_arr[comd_arr_cnt] = token;
                comd_arr_cnt++;
            }

        }

        
      
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
        
        int i = fork();
        if(i <= -1)     //this would be an error
        {
            perror("an error occured with fork()");
            exit(1);
        }
        else if(i == 0)
        {
            if(-1 == execvp(comd_arr[0], comd_arr))
            {
                perror("There is a problem with the prompt entered");
            }
        }
        else if(i > 0)          //parent process
        {
            if(waitpid(i,&status, 0) == -1)
            {
                perror("Error with waitpid");
            }

        }

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


    
