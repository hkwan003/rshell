#include <sys/types.h>
#include <stack>
#include <pwd.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <cstring>
#include <algorithm>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;


bool a_flag = false;
bool l_flag = false;
bool r_flag = false;


void output_directories(const char *names, string file_path)
{
    string files = names;
    bool open_directory = false;
    bool directory = false;
    bool files_in_hiding = false;

    struct stat content;                //syscall stat to call my directories


    int curr_status = stat(file_path.c_str(), &content);
    if(curr_status == -1)
    {
        perror("Error with stat(file_path.c_str(), &content");
    }
    if(content.st_mode & S_IXUSR)
    {
        open_directory = true;
    }
    if(content.st_nlink > 1)
    {
        directory = true;
    }
    if(names[0] == '.')
    {
        if(files != ".." && files != ".")
        {
            files_in_hiding = true;
        }
    } 

    if(directory)
    {
        if(!files_in_hiding)
        {
            cout << "\033[1;34m" << names << "/\033[0m ";
        }
        else
        {
            cout << "\033[1;34m\033[1;40m" << names << "/\033[0m\033[0m ";
        }
    }

    else if(open_directory)
    {
        cout << "\033[1;32m" << names << "*\033[0m ";
    }
    else
    {
        cout << names << " ";
    }
    return;
}


int main(int argc, char *argv[])
{
    vector<string> user_input;

    string str;

    do
    {
        user_input.clear();
        getline(cin, str);
        istringstream in_ss(str);
        while(in_ss >> str)
        {
            input.push_back(str);
        }
    }

    while(user_input.at(0) != "ls")
    {
        bool address_block = false;
      
        string dir_names;           //names of directories passed in


        if(user_input.size() > 1)
        {
            for(int x = 0; x < user_input.size(); x++)
            {
                int sz_temp = user_input.at(x).size();
                if(user_input.at(0) == '-')
                {
                    for(int i = 1; i < sz_temp; i++)
                    {
                        if(user_input.at(x).at(i) == 'a')
                        {
                            a_flag = true;
                        }
                        else if(user_input.at(x).at(i) == 'l')
                        {
                            l_flag = true;
                        }
                        else if(user_input.at(x).at(i) == 'R')
                        {
                            r_flag = true;
                        }
                    }
                }
                else
                {
                    dir_names = user_input.at(x);
                    address_block = true;
                }
                
            }
        }
    
        const char *name_of_directory = ".";
        if(address_block)
        {
            name_of_directory = dir_names.c_str();
        }
        if(-r_flag)
        {
            string path = name_of_directory;
            cout << "work on recursive output later" << endl;
        }

        else
        {
            DIR *dirp = opendir(name_of_directory);
            if(opendir(name_of_directory) == NULL)
            {
                perror("Error in opendir(name_of_directory) while opening");
                exit(1);
            }

            dirent *directory_read;

            while(directory_read == readdir(dirp))
            {
                if(directory_read == NULL)
                {
                    perror("Error with reading readdir(dirp)");
                    exit(1);
                }

                const char *file_paths = directory_read->d_name;

                if(-a_flag && !-l_flag && !-r_flag)
                {
                    cout << "write the case for -a only" << endl;
                    cout << " ";
                }
                
                if( !-a_flag && !-l_flag && !-r_flag)
                {
                    if(file_paths[0] != '.')
                    {
                        output_directories(file_paths, file_paths); 
                        cout << " ";
                    }
                }
                if(-l_flag)
                {
                    string path = name_of_directory;

                    if(!-a_flag)
                    {
                        if(file_paths[0] != '.')
                        {
                            ls_list(name_of_directory, path);
                        }
                    }
                    else
                    {
                        ls_list(name_of_directory, path);
                    }
                }

            }
        }

        if(closeddir(dirp) == -1)
        {
            perror("Error with closedir(dirp)");
            exit(1);
        }

        return 0;
        
    }
}























