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
char* host = (char*)malloc(500);
char *fin_prompt = (char*)malloc(50000);
vector<string> inputs_G;
vector<string> outputs_G;
vector<string> output_append_G;
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
            if(output_append_G.size() > 0 && add_out)
            {
                cout << "does this enteir" << endl;
                if(close(1) == -1)
                {
                    perror("close");
                }
                int fd;
                cout << "show appendation size: " << output_append_G.size() << endl;
                for(unsigned int x = 0; x < output_append_G.size(); x++)
                {   
                    cout << "outupt this stuff: " << output_append_G.at(output_append_G.size() - 1 - x) << endl; 
                    if(fd = (open(output_append_G.at(output_append_G.size() -1 - x).c_str(), O_APPEND | O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR) == -1))
                    {
                        perror("open");
                    }
                }
            }
            cout << "does this enter" << endl;
            if(outputs_G.size() > 0 && out)
            {	
                cout << "is this going in here" << endl;
                if(close(1) == -1)
                {
                    perror("close");
                }
                cout << "is this going in here" << endl;
                int fd;
                for(int x = 0; x < outputs_G.size(); x++)
                {
                    if(fd = (open(outputs_G.at(outputs_G.size() -1 - x).c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR) == -1))
                    {
                        perror("open");
                    }
                }
            }
            if(inputs_G.size() > 0 && in)
            {
                cout << "does this enter here" << endl;
                if(close(0) == -1)
                {
                    perror("close");
                }
                cout << "does this mess it up" << endl;
                int fd;
                for(int x = 0; x < inputs_G.size(); x++)
                {
                    if((fd = open(inputs_G.at(inputs_G.size() -1 - x).c_str(), O_RDONLY)) == -1)
                    {
                        perror("open");
                    }
                }
            }
            cout << "output right before exec " << endl;
            exec_status = (execvp(command, command_list));
            cout << "output exec status////////////////: " << exec_status << endl;
            if(exec_status == -1)
            {
                perror("error with passed in argument list");
                return exec_status;
                exit(1);
            }
        }
        else 
        {
            cout << "is this going in there" << endl;
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
void check_redirection(string &s)
{
    input_hold.clear();
    out_hold.clear();
    add_hold.clear();
    for(int x = 0; x < s.size(); x++)
    {
        if(s.at(x) == '<' && x < s.size())
        {
            s.at(x) = ' ';
            int g = x;
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
                cout << "is thsi workign t" << endl;
                s.at(x) = ' ';
                s.at(x + 1) = ' ';
                x+=2;
                int i = x;
                i++;
                cout << "is this shit working" << endl;
                cout << "s.at(i): " << s.at(i) << endl;
                for(; i < s.size() && s.at(i) != '>'; i++)
                {
                    cout << s.at(i) << endl;
                    add_hold.push_back(s.at(i));
                    s.at(i) = ' ';
                }
                cout << "add_hold: " << add_hold << endl;
                output_append_G.push_back(add_hold);
                cout << "first vecto space: " << output_append_G.at(0) << endl;
                add_hold.clear();
                add_out = true;
            }
            else if(s.at(x + 1) != '>')
            {
                cout << "string shows: " << s.at(x + 1) << endl;
                cout << "strings: " << s << endl;
                cout << "why are u mesing with me" << endl;
                s.at(x) = ' ';
                int i = x;
                i++;
                for(; i < s.size() && s.at(i) != '>'; i++)
                {
                    out_hold.push_back(s.at(i));
                    s.at(i) = ' ';
                }
                outputs_G.push_back(out_hold);
                out_hold.clear();
                out = true;
                cout << "check boolean: " << out << endl;
            }
        }
    }
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
    for(int x = 0; x < s.size(); x++)
    {
        if(s.at(x) == '>' || s.at(x) == '<')
        {
            return true;
        }
    }
}

int i;
bool chk_pipes(string s)
{
    for(int x = 0; x < s.size(); x++)
    {
        if(s.at(x) == '|')
        {
            i++;
        }
    }
    if(i > 1)
    {
        return false;
    }
    return true;
}


int main(int argc, char **argv)
{
    bool checks_pipes = false;
            //boolean to hold if I have pipes in getline()
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
        checks_pipes = false;
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
        if(checks_pipes)
        {
            //implement captureing files and commands
            cout << "yay found the correct piipes" << endl;
        }
        
        
        bool redir_checker = redir_exist(to_be_tokenized);
        check_redirection(to_be_tokenized);
        cout << "checkfasdfasdfboolean: " << add_out << endl;
        cout << "checkfasdfasdfboolean: " << out << endl;
        if(redir_checker)
        {
            for(int x = 0; x < inputs_G.size(); x++)
            {
                string s = fix_file_name(inputs_G.at(x));
                inputs_G.at(x) = s;
                s.clear();
            }
            for(int x = 0; x < outputs_G.size(); x++)
            {
                string s = fix_file_name(outputs_G.at(x));
                outputs_G.at(x) = s;
                s.clear();
            }
            cout << "outputssize: " << output_append_G.size() << endl;
            for(int x = 0; x < output_append_G.size(); x++)
            {
                string s = fix_file_name(output_append_G.at(x));
                output_append_G.at(x) = s;
            }
            
        }
        
        cout << "after string: " << to_be_tokenized << endl;
      
        
        for(int x = 0; x < inputs_G.size(); x++)
        {
            cout << "first: " << inputs_G.at(x) << endl;
        }
        for(int x = 0; x < output_append_G.size(); x++)
        {
            cout << "output_append: " << output_append_G.at(x) << endl;
        }
        
        
        
    
        //cout << "after string: " << to_be_tokenized << endl;
      
        
        
        
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
