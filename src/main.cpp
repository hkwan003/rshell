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
#include <string.h> 


using namespace std;
void fixing_spacing_command(char *org_prompt)
{
    char *finished_prompt = (char*)malloc(50000);
    //x is passed in prompt
    //i is finished prompt after changing spaces
    for(int x = 0,i = 0; org_prompt[x] != '\0'; x++, i++)
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
            ++x;
        }
        else if(org_prompt[x] == '|' && org_prompt[x] == '|')
        {
            finished_prompt[i] = ' ';
            finished_prompt[++i] = '|';
            finished_prompt[++i] = '|';
            finished_prompt[++i] = ' ';
            ++x;
        }
        else
        {
            finished_prompt[i] = org_prompt[x];
        }
        if(org_prompt[x + 1] == '\0') finished_prompt[i + 1] = '\0';
    }
    strcpy(org_prompt, finished_prompt);
   
}

bool execute(char* command, char* command_list[], int conect_type)
{
    int exec_status;
    int status;
    int process_ID = fork();
    if(process_ID <= -1)
    {
        perror("Error occured during forking()");
        exit(1);
    }
    else if(process_ID == 0)            //child process
    {

        exec_status = (execvp(command, command_list));
        if(exec_status == -1)
        {
            perror("error with passed in argument list");
            exit(1);
        }
    }
    else if(process_ID > 0)                 //parent process
    {
        if(waitpid(process_ID, &status,0) == -1)
        {
            perror("error with waitpid()");
        }
    }
    return(!(status == 0 && conect_type == -1) ||( status > 0 && conect_type == 2));
}

int check_connections(char* check)
{
    if(!strcmp(check, ";")) return  0;
    else if(!strcmp(check, "||")) return  1;
    else if(!strcmp(check, "&&")) return  2;
    else return -1;
}
void check_exit(char *str)
{
    if(!strcmp(str, "exit"))
    {
        exit(0);
    }
}
        
    


int main(int argc, char **argv)
{
    int sequence = 0;           //sequence of which is executable and flags
    char* host = (char*)malloc(500);
    string userinfo;
    bool prompter = true;
    char* token;            //will be bit of code strtok cuts off
    bool exec_result = true;
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
        char prompt_holder[50000];//orginal array to hold prompt
        char *comd_arr[50000];
        for(int x = 0; x < 50001; x++)
        {
            prompt_holder[x] = 0;
            comd_arr[x] = 0;
        }
        unsigned int comd_arr_cnt = 0;
        
        //string converter;                           //converts all bits of string into one piece
        string to_be_tokenized;
        getline(cin, to_be_tokenized);
        for(unsigned int x = 0; x < to_be_tokenized.size(); x++)
        {
            prompt_holder[x] =  to_be_tokenized.at(x);
        }
        fixing_spacing_command(prompt_holder);
        int connect_check;                          //indicates which connection is in token
        token = strtok(prompt_holder, "\t ");
        exec_result = true;
        while(token != NULL  && exec_result)
        {
            connect_check = check_connections(token);
            check_exit(token);
            if(connect_check == -1 && sequence < 1)
            {
                check_exit(token);
                comd_arr[comd_arr_cnt] = token;
                comd_arr_cnt++;
                sequence++;                     //increment only once to see which is executable
            }
            else if(sequence > 0 && connect_check == -1)
            {
                comd_arr[comd_arr_cnt] = token;
                comd_arr_cnt++;
                
            }
            else if(connect_check != -1)
            {
                check_exit(token);
                comd_arr[comd_arr_cnt] = '\0' ;
                sequence = 0;
                comd_arr_cnt = 0;
                exec_result = execute(comd_arr[0], comd_arr, connect_check);
                if(exec_result != 0 && connect_check == 1)
                {
                   exec_result = false;
                }
                //if(exec_result == false && connect_check == 1)
                //{
                //    exec_result = true;
                //}
                if(connect_check  == 0)
                {
                    exec_result = true;
                }
                if(exec_result != 0  && connect_check == 2)
                {
                    exec_result = true;
                }
            }
            token = strtok(NULL, "\t ");
            if(connect_check == -1 && token == NULL && exec_result)
            {
                comd_arr[comd_arr_cnt] = '\0';
                execute(comd_arr[0], comd_arr, connect_check);
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

    
