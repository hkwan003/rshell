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
#include <stdlib.h>


using namespace std;


char* host = (char*)malloc(500);
char *fin_prompt = (char*)malloc(50000);
vector<string> inputs_G;
vector<string> outputs_G;
vector<string> output_append_G;
vector<string> piping_str;
bool checks_pipes = false;
char *piparr[100];
void fixing_spacing_command(char *org_prompt, int check_redir)
{
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
        else if(org_prompt[x] == connect[3])
        {
            fin_prompt[i] = ' ';
            fin_prompt[++i] = '|';
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
bool in = false;
bool out = false;
bool add_out = false;
int exec_status;
bool str_continue = true;
int return_file_descrption = 0;     //file descriptor of what file descriptor to change it to
bool execute(char* command, char* command_list[], int conect_type, bool redir, string path_name)
{
    int status;
    int process_ID = fork();
    if(process_ID <= -1)
    {
        perror("Error occured during forking()"); 
        exit(1);
    }
    else if(process_ID == 0) //child process
    {
        if(redir)
        {
            //cout << "add_out: " << add_out << endl;
            if(output_append_G.size() > 0 && add_out)
            {
                //~ cout << "double carrot signs" << endl;
                //~ cout << "return_file descript: " << return_file_descrption << endl;
                if(return_file_descrption == 0)
                {
                    if(close(1) == -1)
                    {
                        perror("close");
                    }
                }
                int fd;
                if(return_file_descrption == 4)
                {
                    if(close(1) == -1)
                    {
                        perror("close(1)");
                    }
                }
                if(return_file_descrption == 5)
                {
                    if(close(2) == -1)
                    {
                        perror("close(2)");
                    }
                }
                //cout << "show appendation size: " << output_append_G.size() << endl;
                for(unsigned int x = 0; x < output_append_G.size(); x++)
                {   
                    //cout << "outupt this stuff: " << output_append_G.at(output_append_G.size() - 1 - x) << endl; 
                    if((fd = (open(output_append_G.at(output_append_G.size() -1 - x).c_str(), O_APPEND | O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR)) == -1))
                    {
                        perror("open");
                    }
                }
            }
            if(outputs_G.size() > 0 && out)
            {	
                //cout << "is this being outputted" << endl;
                if(return_file_descrption == 0)
                {
                    if(close(1) == -1)
                    {
                        perror("close");
                    }
                }
                if(return_file_descrption == 6)
                {
                    if(close(0) == -1)
                    {
                        perror("close(1)");
                    }
                }
                if(return_file_descrption == 1)
                {
                    if(close(1) == -1)
                    {
                        perror("close(1)");
                    }
                }
                if(return_file_descrption == 2)
                {
                    if(close(2) == -1)
                    {
                        perror("close(2)");
                    }
                }
                int fd;
                for(unsigned int x = 0; x < outputs_G.size(); x++)
                {
                    if((fd = (open(outputs_G.at(outputs_G.size() -1 - x).c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR)) == -1))
                    {
                        perror("open");
                    }
                }
            }
            if(inputs_G.size() > 0 && in)
            {
                //cout << "does this enter here" << endl;
                if(close(0) == -1)
                {
                    perror("close");
                }
                //cout << "does this mess it up" << endl;
                int fd;
                for(unsigned int x = 0; x < inputs_G.size(); x++)
                {
                    if((fd = open(inputs_G.at(inputs_G.size() -1 - x).c_str(), O_RDONLY)) == -1)
                    {
                        perror("open");
                    }
                }
            }
            //cout << "output right before exec " << endl;
            exec_status = (execvp(command, command_list));
            //cout << "output exec status////////////////: " << exec_status << endl;
            if(exec_status == -1)
            {
                perror("error with passed in argument list");
                return exec_status;
                exit(1);
            }
        }
        else 
        {
            //cout << "is this going in there" << endl;
            exec_status = (execvp(command, command_list));
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
string input_hold;
string out_hold;
string add_hold;
bool files_descriptor_change = false;       //true if there is need to change file descriptor
void check_redirection(string &s)
{
    return_file_descrption = 0;
    input_hold.clear();
    out_hold.clear();
    add_hold.clear();
    for(unsigned int x = 0; x < s.size(); x++)
    {
        if(s.at(x) == '<' && x < s.size())
        {
            s.at(x) = ' ';
            unsigned int g = x;
            if(g++ < s.size())
            {
                for(; g < s.size() && s.at(g) != '>'; g++)
                {
                    input_hold.push_back(s.at(g));
                }
            }
            in = true;
            inputs_G.push_back(input_hold);
        }
        if(s.at(x) == '>')
        {
            if(s.at(x + 1) == '>' && x +1 < s.size())
            {
                s.at(x) = ' ';
                s.at(x + 1) = ' ';
                x+=2;
                unsigned int i = x;
                i++;
                //cout << "s.at(i): " << s.at(i) << endl;
                for(; i < s.size() && s.at(i) != '>'; i++)
                {
                    //cout << s.at(i) << endl;
                    add_hold.push_back(s.at(i));
                    s.at(i) = ' ';
                }
                //cout << "add_hold: " << add_hold << endl;
                output_append_G.push_back(add_hold);
                //cout << "first vecto space: " << output_append_G.at(0) << endl;
                add_hold.clear();
                add_out = true;
            }
            else if(s.at(x + 1) != '>')
            {
                //cout << "string shows: " << s.at(x + 1) << endl;
                //cout << "strings: " << s << endl;
                //cout << "why are u mesing with me" << endl;
                s.at(x) = ' ';
                unsigned int i = x;
                i++;
                for(; i < s.size() && s.at(i) != '>'; i++)
                {
                    out_hold.push_back(s.at(i));
                    s.at(i) = ' ';
                }
                outputs_G.push_back(out_hold);
                out_hold.clear();
                out = true;
                //cout << "check boolean: " << out << endl;
            }
        }
        if(s.at(x) == '0' || s.at(x) == '1' || s.at(x) == '2')
        {
            for(; x < s.size(); x++)
            {
                files_descriptor_change = true;
                if(s.at(x) == '<' && x < s.size())
                {
                    s.at(x) = ' ';
                    unsigned int g = x;
                    if(g++ < s.size())
                    {
                        for(; g < s.size() && s.at(g) != '>'; g++)
                        {
                            input_hold.push_back(s.at(g));
                        }
                    }
                    in = true;
                    inputs_G.push_back(input_hold);
                }
                if(x + 1 < s.size())
                {
                    x++;
                }
                if(s.at(x) == '>')
                {
                    if(s.at(x + 1) == '>' && x +1 < s.size())
                    {
                        if(s.at(x - 1) == '0')
                        {
                            return_file_descrption = 7;      //3 equals stdin append   
                        }
                        if(s.at(x - 1) == '1')
                        {
                            s.at(x - 1) = ' ';
                            return_file_descrption = 4;     //4 equals stdout append 
                        }
                        if(s.at(x - 1) == '2')
                        {
                            s.at(x - 1) = ' ';
                            return_file_descrption = 5;     //5 equals stderr append 
                        }
                        s.at(x) = ' ';
                        s.at(x + 1) = ' ';
                        x+=2;
                        unsigned int i = x;
                        i++;
                        //cout << "s.at(i): " << s.at(i) << endl;
                        for(; i < s.size() && s.at(i) != '>'; i++)
                        {
                            //cout << s.at(i) << endl;
                            add_hold.push_back(s.at(i));
                            s.at(i) = ' ';
                        }
                        //cout << "add_hold: " << add_hold << endl;
                        output_append_G.push_back(add_hold);
                        //cout << "first vecto space: " << output_append_G.at(0) << endl;
                        add_hold.clear();
                        add_out = true;
                    }
                    else if(s.at(x + 1) != '>')
                    {
                        if(s.at(x - 1) == '0')
                        {
                            return_file_descrption = 6;         //0 equals stdin trunc
                        }
                        if(s.at(x - 1) == '1')
                        {
                            s.at(x - 1) = ' ';
                            return_file_descrption = 1;         //1 equals stdout trunc
                        }
                        if(s.at(x - 1) == '2')
                        {
                            s.at(x - 1) = ' ';
                            return_file_descrption = 2;         //2 equals stderr trunc
                        }
                        s.at(x) = ' ';
                        unsigned int i = x;
                        i++;
                        for(; i < s.size() && s.at(i) != '>'; i++)
                        {
                            out_hold.push_back(s.at(i));
                            s.at(i) = ' ';
                        }
                        outputs_G.push_back(out_hold);
                        out_hold.clear();
                        out = true;
                    }
                }
            }
        }
            
    }
}

void execute_piping()
{
    int status;
    
    int process_ID = fork();
    cout << "process: " << process_ID << endl;
    if(process_ID <= -1)
    {
        perror("Error occured during forking()");
        exit(1);
    }
    if(process_ID == 0) //child process
    {
        
        //cout << "arguments1: " << argument1[0] << endl;
        //exec_status = (execvp(argument1[0], argument1));
        cout << "output exec status: " << exec_status << endl;
        if(exec_status == -1)
        {
            perror("error with passed in argument list");
            exit(1);
        }
    }
    if(process_ID > 0) //parent process
    {
        if(waitpid(process_ID, &status,0) == -1)
        {
            perror("error with waitpid()");
        }
    }
    
    //execvp(argument1[0], argument1);
    
}
string final_file_name;
string fix_file_name(string s)
{
    final_file_name.clear();
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

bool redir_exist(string s)
{
    for(unsigned int x = 0; x < s.size(); x++)
    {
        if(s.at(x) == '>' || s.at(x) == '<')
        {
            return true;
        }
    }
}

unsigned int i;
bool chk_pipes(string s)
{
    for(unsigned int x = 0; x < s.size(); x++)
    {
        //cout << "is this actually checking rn" << endl;
        if(s.at(x) == '|')
        {
            i++;
            //cout << "output I: " << i << endl;
        }
    }
    if(i > 1)
    {
        return false;
    }
}

void push_piping_string(string s)
{
    string t;
    for(unsigned int x = 0; x < s.size(); x++)
    {
        //cout << "X: " << x << " " << s.at(x) <<endl;
        
        if(s.at(x) != '|' && x < s.size())
        {
            t.push_back(s.at(x));
        }
        if(s.at(x) == '|' || x == s.size() - 1)
        {
            //cout << "does it actually push back" << endl;
            piping_str.push_back(t);
            if(x < s.size()- 1)
            {
                t.clear();
                t.push_back(' ');
                piping_str.push_back(t);
                t.clear();
            }
        }
    }
    
    for(unsigned int x = 0; x < piping_str.size(); x++)
    {
        string s = piping_str.at(x);
        
    }
    //unsigned int r = 0;
    //~ for(r; r < piping_str.size(); r++)
    //~ {
        //~ piparr[r] = const_cast<char*> (piping_str.at(r).c_str());
    //~ }
    
}
void fix_pipe_argument(string& s)
{
    bool word_start = false;
    string fixed;
    for(unsigned int x = 0; x < s.size(); x++)
    {
        //cout << "words: " << s.at(x) << endl;
        if(s.at(x) == ' ' && word_start == false)
        {
            //cout << "1" << endl;
            continue;
        }
        if(s.at(x) == ' ' && word_start == true)
        {
            //cout << "2" << endl;
            fixed.push_back(s.at(x));
        }
        if(s.at(x) != ' ' && word_start == true)
        {
            fixed.push_back(s.at(x));
        }
        if(s.at(x) != ' ' && word_start == false)
        {
            //cout << "3" << endl;
            fixed.push_back(s.at(x));
            word_start = true;
        }
        if(s.at(x) == '|')
        {
            //cout << "4" << endl;
            word_start = false;
        }
    }
    s.clear();
    //cout << "output s with in function: " << fixed << endl;
    for(unsigned int x = 0; x < fixed.size(); x++)
    {
        s.push_back(fixed.at(x));
    }
}

int main(int argc, char **argv)
{
    outputs_G.clear();
    inputs_G.clear();
    int multi_redir_chk = 0;
    int check_redir = 0;
    ///////////////////////////////////////////////
    int sequence = 0; //sequence of which is executable and flags
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
        i = 0;
            //reset counter for increment for pipe checker
        checks_pipes = true;
        in = false;
        out = false;
        add_out = false;
        inputs_G.clear();
        outputs_G.clear();
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
        //gets user input
        checks_pipes = chk_pipes(to_be_tokenized);
        bool three_bracket = false;
        for(unsigned int x = 0; x < to_be_tokenized.size(); x++)
        {
            if(to_be_tokenized.at(x) == '<' && to_be_tokenized.at(x + 1) == '<' && to_be_tokenized.at(x + 2) == '<')
            {
                checks_pipes = true;
                three_bracket = true;
            }
        }
        //cout << "checking pipes: " << checks_pipes << endl;
        if(checks_pipes)
        {
            string tmp_str;         //holds 
            bool words_begin = false;    //when it senses words, it will become true
            fix_pipe_argument(to_be_tokenized);
            int array1 = 0;      //counter for array argument1
            int array2 = 0;      //counter for array argument2
            //cout << "new prompt: " << to_be_tokenized << endl;
            int tmp_pos = 0;        //checks if there is more than one space
            char *argument1[50000];
            char *argument2[50000];
            char *token;
            string first_half;
            string second_half;
            char first[50000];
            char second[50000];
            for(int x = 0; x < 50000; x++)
            {
                argument1[x] = '\0';
                argument2[x] = '\0';
                first[x] = '\0';
                second[x] = '\0';
            }
             //clearing arrays all arrays
            if(three_bracket)
            {
                for(unsigned int x = 0; x < to_be_tokenized.size(); x++)
                {
                    if(to_be_tokenized.at(x) != '<')
                    {
                        second_half.push_back(to_be_tokenized.at(x));
                    }
                    if(to_be_tokenized.at(x) == '<')
                    {
                        break;
                    }
                }
                first_half+= "echo ";
                for(unsigned int x = 0; x < to_be_tokenized.size(); x++)
                {
                    if(to_be_tokenized.at(x) == '<')
                    {
                        if(x + 3 < to_be_tokenized.size())
                        {
                            x+= 3;
                            for(; x < to_be_tokenized.size(); x++)
                            {
                                if(to_be_tokenized.at(x) == '"')
                                {
                                    to_be_tokenized.at(x) = ' ';
                                }
                                first_half.push_back(to_be_tokenized.at(x));
                            }
                        }
                    }
                }
                for(unsigned int x = 0; x < first_half.size(); x++)
                {
                    first[x] = first_half.at(x);
                } 
                
                for(unsigned int x = 0; x < second_half.size(); x++)
                {
                    second[x] = second_half.at(x);
                }
                      token = strtok(first, "\t ");
                while(token != NULL)
                {
                    //cout<< "first: "  << token << endl;
                    argument1[array1] = token;
                    array1++;
                    token = strtok(NULL, "\t ");
                }
                
                argument1[array1 + 1] = '\0'; 

                token = strtok(second, "\t ");
                while(token != NULL)
                {
                    //cout << "Second: "  << token << endl;
                    argument2[array2] = token;
                    array2++;
                    token = strtok(NULL, "\t ");
                }

                argument2[array2 + 1] = '\0';
                int fd[2];
                pid_t pid1, pid2;

                pipe(fd);
                pid1 = fork();
                if(pid1 < 0)
                {
                    perror("first fork failed()");
                    exit(1);
                }
                if(pid1 == 0)
                {
                    close(1);
                    dup(fd[1]);
                    close(fd[0]);
                    close(fd[1]);
                    exec_status = execvp(argument1[0], argument1);
                    if(exec_status == -1)
                    {
                        perror("error with passed in argument");
                        exit(1);
                    } 
                }
                
                pid2 = fork();
                if(pid2 < 0)
                {
                    perror("second fork() failed");
                    exit(1);
                }
                if(pid2 == 0)
                {
                    close(0);
                    dup(fd[0]);
                    close(fd[0]);
                    close(fd[1]);
                    exec_status = execvp(argument2[0], argument2);
                    if(exec_status == -1)
                    {
                        perror("error with passed in argument");
                        exit(1);
                    } 
                }
                close(fd[0]);
                close(fd[1]);
                waitpid(pid1, NULL, 0);
                waitpid(pid1, NULL, 0);
                
            }
        
            else
            {
                for(unsigned int x = 0; x < to_be_tokenized.size(); x++)
                {
                    if(to_be_tokenized.at(x) != '|')
                    {
                        first_half.push_back(to_be_tokenized.at(x));
                    }
                    if(to_be_tokenized.at(x) == '|')
                    {
                        break;
                    }
                }
                    
                for(unsigned int x = 0; x < to_be_tokenized.size(); x++)
                {
                    if(to_be_tokenized.at(x) == '|')
                    {
                        x++;
                        for(; x < to_be_tokenized.size(); x++)
                        {
                            second_half.push_back(to_be_tokenized.at(x));
                        }
                    }
                }
                for(unsigned int x = 0; x < first_half.size(); x++)
                {
                    first[x] = first_half.at(x);
                } 
                
                for(unsigned int x = 0; x < second_half.size(); x++)
                {
                    second[x] = second_half.at(x);
                }

                token = strtok(first, "\t ");
                while(token != NULL)
                {
                    //cout<< "first: "  << token << endl;
                    argument1[array1] = token;
                    array1++;
                    token = strtok(NULL, "\t ");
                }
                
                argument1[array1 + 1] = '\0'; 

                token = strtok(second, "\t ");
                while(token != NULL)
                {
                    //cout << "Second: "  << token << endl;
                    argument2[array2] = token;
                    array2++;
                    token = strtok(NULL, "\t ");
                }

                argument2[array2 + 1] = '\0';


                int fd[2];
                pid_t pid1, pid2;

                pipe(fd);
                pid1 = fork();
                if(pid1 < 0)
                {
                    perror("first fork failed()");
                    exit(1);
                }
                if(pid1 == 0)
                {
                    close(1);
                    dup(fd[1]);
                    close(fd[0]);
                    close(fd[1]);
                    exec_status = execvp(argument1[0], argument1);
                    if(exec_status == -1)
                    {
                        perror("error with passed in argument");
                        exit(1);
                    } 
                }
                
                pid2 = fork();
                if(pid2 < 0)
                {
                    perror("second fork() failed");
                    exit(1);
                }
                if(pid2 == 0)
                {
                    close(0);
                    dup(fd[0]);
                    close(fd[0]);
                    close(fd[1]);
                    exec_status = execvp(argument2[0], argument2);
                    if(exec_status == -1)
                    {
                        perror("error with passed in argument");
                        exit(1);
                    } 
                }
                close(fd[0]);
                close(fd[1]);
                waitpid(pid1, NULL, 0);
                waitpid(pid1, NULL, 0);
            }
            
        }
        else
        {
            bool redir_checker = redir_exist(to_be_tokenized);
            check_redirection(to_be_tokenized);
            //~ cout << "checkfasdfasdfboolean: " << add_out << endl;
            //~ cout << "checkfasdfasdfboolean: " << out << endl;
            if(redir_checker)
            {
                for(unsigned int x = 0; x < inputs_G.size(); x++)
                {
                    string s = fix_file_name(inputs_G.at(x));
                    inputs_G.at(x) = s;
                    s.clear();
                }
                for(unsigned int x = 0; x < outputs_G.size(); x++)
                {
                    string s = fix_file_name(outputs_G.at(x));
                    outputs_G.at(x) = s;
                    s.clear();
                }
                //cout << "outputssize: " << output_append_G.size() << endl;
                for(unsigned int x = 0; x < output_append_G.size(); x++)
                {
                    string s = fix_file_name(output_append_G.at(x));
                    output_append_G.at(x) = s;
                }
                
            }
          
            
            for(int x = 0; x < inputs_G.size(); x++)
            {
                //cout << "first: " << inputs_G.at(x) << endl;
            }
            for(int x = 0; x < output_append_G.size(); x++)
            {
                //cout << "output_append: " << output_append_G.at(x) << endl;
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
                //cout << "tokens: " << *token << endl;
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
                    exec_result = execute(comd_arr[0], comd_arr, connect_check, redir_checker,final_file_name);
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
                execute(comd_arr[0], comd_arr, connect_check, redir_checker,final_file_name);
                }
            }
            
        }
    }
}
