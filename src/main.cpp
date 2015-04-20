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
    char *fin_prompt = (char*)malloc(50000);
    char connect[4];
    connect[0] = ';';
    connect[1] = '&';
    connect[2] = '|';
    connect[3] = '#';
    //x is passed in prompt
    //i is finished prompt after changing spaces
    for(int x = 0,i = 0; org_prompt[x] != '\0'; x++, i++)
    {
        if(org_prompt[x] == connect[3])
        {
            org_prompt[x] = '\0'; fin_prompt[i] = '\0';
        }
        else if(org_prompt[x] == connect[0])
        {
            fin_prompt[i] = ' ';fin_prompt[++i] = ';';fin_prompt[++i] = ' ';
        }
        else if(org_prompt[x] == connect[1] && org_prompt[x + 1] == connect[1])
        {
            fin_prompt[i] = ' ';fin_prompt[++i] = '&';fin_prompt[++i] = '&';fin_prompt[++i] = ' ';
            ++x;
        }
        else if(org_prompt[x] == connect[3] && org_prompt[x + 1] == connect[3])
        {
            fin_prompt[i] = ' ';fin_prompt[++i] = '|';fin_prompt[++i] = '|';fin_prompt[++i] = ' ';
            ++x;
        }
        else
        {
            fin_prompt[i] = org_prompt[x];
        }
        if(org_prompt[x + 1] == '\0') fin_prompt[i + 1] = '\0';
    }
    strcpy(org_prompt, fin_prompt);        //copies altered version
}
bool conect_bool = false;
int exec_status;
bool str_continue = true;
void execute(char* command, char* command_list[], int conect_type)
{
    //cout << "beginning of execution: " << endl;
    int status;
    int process_ID = fork();
    if(process_ID <= -1)
    {
        perror("Error occured during forking()");
        exit(1);
    }
    else if(process_ID == 0)            //child process
    {
        //cout << "output exec status in function first: " << exec_status << endl;
        exec_status = (execvp(command, command_list));
        if(exec_status == -1)
        {
            conect_bool = 0;
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
    exec_status = (!(status == 0 && conect_type == -1) ||( status > 0 && conect_type == 2));
    //cout << "return value: "  << (!(status == 0 && conect_type == -1) || (status > 0 && conect_type == 2)) << endl;
    //cout << "output execstatus during return: " << exec_status << endl;
    //return(!(status == 0 && conect_type == -1) ||( status > 0 && conect_type == 2));
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
void check_clear(char *str)
{
    if(!strcmp(str, "clear"))
    {
        for(int x = 0; x < 80; x++)
        {
            cout << endl;
        }
    }
}
int main(int argc, char **argv)
{
    conect_bool = true;
    int sequence = 0;           //sequence of which is executable and flags
    char* host = (char*)malloc(500);
    string userinfo;
    bool prompter = true;
    char* token;            //will be bit of code strtok cuts off
    bool exec_result;
    if(getlogin() != NULL)      //checks if login is valid
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
        for(int x = 0; x < 50001; x++)              //reinitalizes all arrays to zero
        {
            prompt_holder[x] = 0;
            comd_arr[x] = 0;
        }
        unsigned int comd_arr_cnt = 0;
        str_continue = true; 
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
        while(token != NULL  && exec_result && str_continue)
        {
            // cout << "outputing token: " << token << endl;
            connect_check = check_connections(token);
            check_exit(token);
            check_clear(token);         //added clear function
            if(connect_check == -1 && sequence < 1 && str_continue)
            {
                check_exit(token);
                comd_arr[comd_arr_cnt] = token;
                comd_arr_cnt++;
                sequence++;                     //increment only once to see which is executable
            }
            else if(sequence > 0 && connect_check == -1 && str_continue)
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
                //cout << "does it output second iteration " << endl;
                execute(comd_arr[0], comd_arr, connect_check);
                //cout << "output exec_status: " << exec_status << endl;
                //cout << "output exec_result tho: " << exec_result << endl;
                //cout << "connect_check: " << connect_check << endl;
                //cout << "str_continue: " << str_continue << endl; 
                if(exec_status == 1)
                {
                    if(connect_check == 2)
                    {
                        str_continue = false;
                    }
                }
                }
                else if(exec_result == 0)
                {
                    if(connect_check == 1)
                    { 
                        cout << "is this outputting plz " << endl;
                        str_continue = false;
                    }
                }
            
            token = strtok(NULL, "\t ");
            if(connect_check == -1 && token == NULL && exec_result && str_continue)
            {
                //cout << "guess this executeisw ith this " << endl;
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

    
