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
bool cd_check = true;
bool set_last = false;
bool continue_program = true;
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
string input_hold;
string out_hold;
string add_hold;
bool files_descriptor_change = false;       //true if there is need to change file descriptor
string fix_file_name(string s);
char* host = (char*)malloc(500);
char *fin_prompt = (char*)malloc(50000);
vector<string> inputs_G;
vector<string> outputs_G;
vector<string> output_append_G;
vector<string> piping_str;
bool checks_pipes = false;
char *piparr[100];
bool in = false;
bool out = false;
bool add_out = false;
int return_file_descrption = 0;     //file descriptor of what file descriptor to change it to
bool check_change_DIR(char* arr[])
{
    char* dir = arr[0];
    char* path = arr[1];
    char* temp;
    if(!strcmp(dir, "cd"))
    {
        if(path == '\0')
        {
            char *home, *old;
            if((home = getenv("HOME")) == NULL)
            {
                perror("get enviromente");
            }
            if((old = getenv("PWD")) == NULL)
            {    
                perror("get enviromente2");
            }
            if(chdir(home) == -1)
            {    
                perror("changing directories 1");	
            }
            if(setenv("PWD", home, 1) == -1)
            {    
                perror("set those directories1");
            }
            if(setenv("OLDPWD", old, 1) == -1)
            {    
                perror("set those directories12");
            }
        }
        else if(!strcmp(path, "-"))
        {
            char *old, *newDir;
            if((old = getenv("PWD")) == NULL)
            { 
                perror("get working dir");
            }
            if((newDir = getenv("OLDPWD")) == NULL) 
            {    
                perror("get old working dir");
            }
            if(chdir(newDir) == -1)
            {    
                perror("change dir");
            }
            if(setenv("PWD", newDir, 1) == -1)
            {    
                perror("set dir");
            }
            if(setenv("OLDPWD", old, 1) == -1) 
            {    
                perror("setting old direcory");
            }
        }
        else
        {
           string strPath;
           for(int x = 0; path[x] != '\0'; x++)
           {
               strPath.push_back(path[x]);
           }
            if(strPath.size() > 0 && strPath.at(0) == '~')
            {
                if(strPath.size() == 1 || strPath.at(1) == '/')
                {
                    strPath = strPath.substr(1);
                    string tempString = getenv("HOME");
                    strPath = tempString + strPath;
                }
            }
            char fullPath[50000];
            if(NULL == realpath(strPath.c_str(), fullPath))
            {
                perror("something happened while finding full path"); 
                return -1;
            }
            if(-1 == chdir(fullPath))
            {
                perror("changing those directories");
                return -1;
            }
            if(-1 == setenv("OLDPWD", getenv("PWD"), 1))
            {
                perror("setting good oldPWD");
                return -1;
            }
            if(-1 == setenv("PWD", fullPath, 1))
            {
                perror("setting the usual PWD");
                return -1;
            }
        }
        return true;
    }
    return false;
}
int exec_status;
bool str_continue = true;
int process_ID = 0;
 int status;
bool execute(char* command, char* command_list[], int conect_type)
{
    cd_check = check_change_DIR(command_list);
    {
        if(!cd_check)
        {
            //cout << "beginning of execution: " << endl;
            process_ID = fork();
            if(process_ID <= -1)
            {
                perror("Error occured during forking()");
                exit(1);
            }
            else if(process_ID == 0) //child process
            {
                exec_status = (execvp(command, command_list));
                //cout << "output exec status: " << exec_status << endl;
                if(exec_status == -1)
                {
                    perror("error with passed in argument list");
                    exit(1);
                }
            }
            else if(process_ID > 0) //parent process
            {
                 int wpid;
					do {
						wpid = wait(&status);
					} while (wpid == -1 && errno == EINTR);
					if(wpid == -1) {
						//perror("wait error");

					}
            }
            return(!(status == 0 && conect_type == -1) ||( status > 0 && conect_type == 2));
        }
    }
}
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
    return false;
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
    if(i == 1)
    {
        return true;
    }
    return false;
	
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
int check_connections(char* check)
{
    if(!strcmp(check, ";")) return 0;
    else if(!strcmp(check, "||")) return 1;
    else if(!strcmp(check, "&&")) return 2;
    else return -1;
}
void check_exit(char *str)
{
    if(!strcmp(str, "exit"))
    {
        exit(0);
    }
}
int permanent = 0;
static void handle_ctrl_Z(int i, siginfo_t *info, void* ptr)
{
    permanent = process_ID;
    cout << endl;
}
static void signal_handler(int i, siginfo_t *info, void* ptr)
{
    if(i == SIGINT)
    {
        wait(0);
        cout << endl;
        return;
    }
}



int main(int argc, char **argv)
{
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
        continue_program = true;
        cd_check = true;
        cin.clear();
        struct sigaction new_one = {0};
        struct sigaction old_one = {0};
        sigset_t foo;
        new_one.sa_mask = foo;
        new_one.sa_sigaction = signal_handler;
        
        struct sigaction new_two = {0};
        struct sigaction old_two = {0};
        sigset_t bang;
        new_two.sa_mask = bang;
        new_two.sa_sigaction = handle_ctrl_Z;
        
        if(sigaction(SIGINT, &new_one, &old_one) == -1)
        {
            perror("problem with SIGINT");
        }
        if(sigaction(SIGTSTP, &new_two, &old_two) == -1)
        {
            perror("problem with SIGSTP");
        }
        string curr_wrkin_dir(get_current_dir_name());
        string fixed_dir;
        int i = 0;
        for(int x = curr_wrkin_dir.size(); x > 0; x--)
        {
            fixed_dir += curr_wrkin_dir[x];
            if(curr_wrkin_dir[x] == '/')
            {
                i++;
                if(i == 2)
                {
                    x = 0;
                }
            }
        }
        fixed_dir = string(fixed_dir.rbegin(), fixed_dir.rend());
        
        //char *pathname;
        //pathname = getenv("PWD");
        cout << userinfo << "@" << host << "; " << fixed_dir << " ";
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
        if(to_be_tokenized.size() != 0)
        {
            for(unsigned int x = 0; x < to_be_tokenized.size(); x++)
            {
                prompt_holder[x] = to_be_tokenized.at(x);
            }   
            fixing_spacing_command(prompt_holder);
            int connect_check; //indicates which connection is in token
            token = strtok(prompt_holder, "\t ");
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
                    exec_result = execute(comd_arr[0], comd_arr, connect_check);
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
                    execute(comd_arr[0], comd_arr, connect_check);
                }
            }
        }
    }
}

