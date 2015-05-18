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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

 
  
using namespace std;
void fixing_spacing_command(char *org_prompt, int check_redir)
{
    char *fin_prompt = (char*)malloc(50000);
    char connect[4];
    connect[0] = ';';
    connect[1] = '&';
    connect[2] = '|';
    connect[3] = '#';
    connect[4] = '>';
    //x is passed in prompt
    //i is finished prompt after changing spaces
    for(int x = 0,i = 0; org_prompt[x] != '\0'; x++, i++)
    {
        if(org_prompt[x] == connect[3])
        {
            org_prompt[x] = '\0'; 
            fin_prompt[i] = '\0';
        }
        else if(org_prompt[x] == connect[0])
        {
            fin_prompt[i] = ' ';
            fin_prompt[++i] = ';';
            fin_prompt[++i] = ' ';
        }
        else if(org_prompt[x] == connect[1] && org_prompt[x + 1] == connect[1])
        {
            fin_prompt[i] = ' ';
            fin_prompt[++i] = '&';
            fin_prompt[++i] = '&';
            fin_prompt[++i] = ' ';
            ++x;
        }
        else if(org_prompt[x] == connect[3] && org_prompt[x + 1] == connect[3])
        {
            fin_prompt[i] = ' ';
            fin_prompt[++i] = '|';
            fin_prompt[++i] = '|';
            fin_prompt[++i] = ' ';
            ++x;
        }
        else if(org_prompt[x] == connect[4])
        {
            check_redir = 1;        //redir 1 means one output bracket
            fin_prompt[i] = ';';
            fin_prompt[++i] = ' '; 
            fin_prompt[++i] = ' ';
            fin_prompt[++i] = ' ';
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
int exec_status;
bool str_continue = true;
bool execute(char* command, char* command_list[], int conect_type, int redir, string path_name)
{
    //cout << "conect:type: " << conect_type << endl;
    int status;
    int process_ID = fork();
    if(process_ID <= -1)
    {
        perror("Error occured during forking()");
        exit(1);
    }
    else if(process_ID == 0) //child process
    {
        if(redir == 1)
        {	
            //cout << "did this output" << endl;
            int fd;
            if((fd = open(path_name.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0744)) == -1)
            {
                perror("open");
            }
            if(close(1) == -1)
            {
                perror("close");
            }
            if(dup(fd) == -1)
            {
                perror("dup");
            }
            exec_status = (execvp(command, command_list));
            //cout << "output exec status: " << exec_status << endl;
            if(exec_status == -1)
            {
                perror("error with passed in argument list");
                return exec_status;
                exit(1);
            }
        }
        if(redir == 2)
        {
            //cout << "yay this should output now" << endl;
            int fd;
            if((fd = open(path_name.c_str(), O_RDWR | O_CREAT | O_APPEND, 0744)) == -1)
            {
                perror("open");
            }
            if(close(1) == -1)
            {
                perror("close");
            }
            if(dup(fd) == -1)
            {
                perror("dup");
            }
            exec_status = (execvp(command, command_list));
            //cout << "output exec status: " << exec_status << endl;
            if(exec_status == -1)
            {
                perror("error with passed in argument list");
                return exec_status;
                exit(1);
            }
        }
        if(redir == 3)
        {
            int fd;
            if((fd = open(path_name.c_str(), O_RDONLY)) == -1)
            {
                perror("open");
            }
            if(close(0) == -1)
            {
                perror("close");
            }
            if(dup(fd) == -1)
            {
                perror("dup");
            }
            exec_status = (execvp(command, command_list));
            if(exec_status == -1)
            {
                perror("error with input redirection argument list");
                return exec_status;
                exit(1);
            }
            
            //int yd = 
        }
            
        else
        {
            //cout << "this is wrong" << endl;
            exec_status = (execvp(command, command_list));
            //cout << "output exec status: " << exec_status << endl;
            if(exec_status == -1)
            {
                perror("error with passed in argument list");
                return exec_status;
                exit(1);
            }
        }
    }
    else if(process_ID > 0) //parent process
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
    if(!strcmp(check, ";")) return 0;
    else if(!strcmp(check, "||")) return 1;
    else if(!strcmp(check, "&&")) return 2;
    else if(!strcmp(check, ">")) return 3;
    else return -1;
}
void check_exit(char *str)
{
    if(!strcmp(str, "exit"))
    {
        exit(0);
    }
}
int to_redir;
string file_name;   //holds file name of file passed in
void chk_redir(string &s)
{
    for(int x = 0; x < s.size(); x++)
    {
        //cout << "output x: " << x << endl;
        if(s.at(x) == '>')
        {
            //cout << "output x: " << endl;
            int y = x;
            if(y++ < s.size() && s.at(x) == '>')
            {
                for(int i = y; i < s.size(); i++)
                {
                    file_name.push_back(s.at(i));
                    s.at(i) = ' ';
                    if(s.at(x) != ' ' && x < s.size())
                    {
                        continue;
                    }
                    break;
                }
                to_redir = 2;
            }
            else
            {
                for(int i = y; i < s.size(); i++)
                {
                    file_name.push_back(s.at(i));
                    s.at(i) = ' ';
                    if(s.at(x) != ' ' && x < s.size())
                    {
                        continue;
                    }
                    break;
                }
                to_redir = 1;
            }
        }
        else if(s.at(x) == '<')
        {
            int g = x;
            if(g++ < s.size() && s.at(x) == '>')
            {
                for(int i = g; i < s.size(); i++)
                {
                    file_name.push_back(s.at(i));
                    s.at(i) = ' ';
                    if(s.at(x) != ' ' && x < s.size())
                    {
                        continue;
                    }
                    break;
                }
                to_redir = 3;
            }
        }
    }
}
string final_file_name;
string fix_file_name(string s)
{
    int sz = s.size();
    for(int x = 0; x < sz; x++)
    {
        if(s.at(x) != ' ')
        {
            final_file_name.push_back(s.at(x));
        }
    }
    return final_file_name;
}
bool out_flag = false;
bool in_flag = false;
string infile;
string outfile;
void chk_mult_redir(string s)
{
    cout << "is this goign in there" << endl;
    for(int g = 0; g < s.size(); g++)
    {
        if(s.at(g) == '<')
        {
            in_flag = true;
    
        }
        if(s.at(g) == '>')
        {
            out_flag = true;
        }
    }
}

void grab_mult_redir(string &s)
{
    for(int x = 0; x < s.size(); x++)
    {
        if(s.at(x) == '<')
        {
            int g = x;
            if(g++ < s.size() && s.at(x) == '<')
            {
                for(int v = g; s.at(v) != '>'; v++)
                {
                    cout << s.at(v) << endl;
                    infile.push_back(s.at(v));
                    s.at(v) = ' ';
                }
            }
        }
        if(s.at(x) == '>')
        {
            int g = x;
            if(g++ < s.size())
            {
                for(int v = g; v < s.size(); v++)
                {
                    cout << s.at(v) << endl;
                    outfile.push_back(s.at(v));
                    s.at(v) = ' ';
                }
            }
        }
    }
}
        

int main(int argc, char **argv)
{
    int multi_redir_chk = 0;
    int check_redir = 0;
    ///////////////////////////////////////////////
    int sequence = 0; //sequence of which is executable and flags
    char* host = (char*)malloc(500);
    string userinfo;
    bool prompter = true;
    char* token; //will be bit of code strtok cuts off
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
        to_redir = 0;
        out_flag = false;
        in_flag = false;
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
        str_continue = true;
        //string converter; //converts all bits of string into one piece
        string to_be_tokenized;
        getline(cin, to_be_tokenized);
        chk_mult_redir(to_be_tokenized);
        if(out_flag  == true && in_flag == true)
        {
            grab_mult_redir(to_be_tokenized);
            outfile = fix_file_name(outfile);
            cout << "outfile: " << outfile << endl;
            final_file_name.clear();
            infile = fix_file_name(infile);
            cout << "infile: " << infile << endl;
        }
        else
        {
            cout << "hi there" << endl;
            chk_redir(to_be_tokenized);//checks if there is redirection sign in command
            final_file_name = fix_file_name(file_name);
        }
        
      
        
        
        
        for(unsigned int x = 0; x < to_be_tokenized.size(); x++)
        {
            prompt_holder[x] = to_be_tokenized.at(x);
        }   
        fixing_spacing_command(prompt_holder, check_redir);
        int connect_check; //indicates which connection is in token
        token = strtok(prompt_holder, "\t ");
        //cout << "output token: " << token << endl;
        exec_result = true;
        while(token != NULL && exec_result && str_continue)
        {
            connect_check = check_connections(token);
            check_exit(token);
            if(connect_check == -1 && sequence < 1 && str_continue)
            {
                //cout << "does this come out on top" << endl;
                check_exit(token);
                comd_arr[comd_arr_cnt] = token;
                comd_arr_cnt++;
                sequence++; //increment only once to see which is executable
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
                exec_result = execute(comd_arr[0], comd_arr, connect_check, to_redir, final_file_name);
                //cout << "output exec_status: " << exec_status << endl;
                //cout << "output exec_result tho: " << exec_result << endl;
                //cout << "connect_check: " << connect_check << endl;
                //cout << "str_continue: " << str_continue << endl;
                if(exec_status == 0)
                {
                    if(connect_check == 2)
                    {
                        str_continue = false;
                        //cout << "str_continue: " << str_continue << endl;
                    }
                    if(connect_check == 1)
                    {
                        str_continue = false;
                    }
                }
                if(exec_result == 1)
                {
                    if(connect_check == 2)
                    {
                        str_continue = true;
                    }
                }
                
                    
            }
            token = strtok(NULL, "\t ");
            if(connect_check == -1 && token == NULL && exec_result && str_continue)
            {
            //cout << "guess this executeisw ith this " << endl;
            comd_arr[comd_arr_cnt] = '\0';
            execute(comd_arr[0], comd_arr, connect_check, to_redir, final_file_name);
            }
        }
    }
}
