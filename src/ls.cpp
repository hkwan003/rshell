#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>





using namespace std;


string convert_to_string(char *Data)
{
    return string(Data);
}


bool Alphabetical(string c1, string c2)
{
    for(int i = 0; i < c1.size(); i++)
    {
        c1.at(i) = tolower(c1.at(i));
    }
    for(int i = 0; i < c2.size(); i++)
    {
        c2.at(i) = tolower(c2.at(i));
    }
    return c1.compare(c2) < 0;
}


vector<int> flag_index;                     //global vector for checing which flags
vector<string> file_store;
string converted_string; 


void type_of_flag(string s)
{
    for(int x = 0; x < s.size(); x++)           // 0 = "-" dash
    {                                           // 1 = "a"
        if(s.at(x) == '-')                      // 2 = "l"
        {                                       // 3 = "R"
            flag_index.push_back(0);
        }
        else if(s.at(x) == 'a')
        {
            flag_index.push_back(1);
        }
        else if(s.at(x) == 'l')
        {
            flag_index.push_back(2);
        }
        else if(s.at(x) == 'R')
        {
            flag_index.push_back(3);
        }
        else
        {
            flag_index.push_back(-1);
        }
    }
}


void print_files(vector<string> V)
{
    for(int x = 0; x < V.size(); x++)
    {
        cout << V.at(x) << " ";
    }
}


void execute(int arg, char ** agv)
{
    DIR *dirp;
    dirp = opendir(agv[0]);
    if(NULL == dirp)
    { 
        perror("opendir()");
        exit(1);
    }
    struct dirent *filespecs;
    while(NULL != (filespecs = readdir(dirp)))
    {
        file_store.push_back(filespecs->d_name);
    }
    sort(file_store.begin(), file_store.end(), Alphabetical);
    print_files(file_store); 

    if(-1 == closedir(dirp))
    {
        perror("closedir");
        exit(1);
    }
}



int main(int argc, char ** argv)
{
    bool A_check;
    bool L_check;
    bool R_check;
    string flags;               //holds all flags of user input
    if(argc <= 1)
    {
        cout << "Nothing passed in to argv." << endl;
        exit(1);
    }
   
        vector<char *> flags_finder;
    
        for(int x = 1; x < argc; x++)
        {
            flags_finder.push_back(argv[x]);
        }
    
        char* Sec_Arr = new char[2];
        Sec_Arr[0] = '.';
        strcpy(Sec_Arr,".");
        argv[0] = Sec_Arr;
        for(int x = 1; x < argc; x++)
        {
            converted_string += convert_to_string(argv[x]);
            converted_string += " ";
        }
        //cout << converted_string;
        for(int x = 0; x < flags_finder.size(); x++)
        {
            converted_string += convert_to_string(flags_finder.at(x));
            for(int i = 0; i < converted_string.size(); i++)
            {
                if(converted_string.at(i) == '-')
                {
                    for(int v = 1; v < converted_string.size(); v++)
                    {
                        flags += converted_string.at(x);
                    }
                }
            }
        }
        for(int i = 0; i < flags.size(); i++)
        {   
            if(flags.at(i) == 'a')
            {
                A_check = true;
            }
            else if(flags.at(i) == 'l')
            {
                L_check = true;
            }
            else if(flags.at(i) == 'R')
            {
                R_check == true;
            }

        }
        if(A_check)
        {
            execute(argc, argv);
        }
        
      
      
       

        DIR *dirp;
        if(NULL == (dirp = opendir(argv[0])))
        {
            perror("There was an error with opendir(). ");
            exit(1);
        }
        struct dirent *filespecs;
        errno = 0;
        while(NULL != (filespecs = readdir(dirp)))
        {
            file_store.push_back(filespecs->d_name);
        }
        sort(file_store.begin(), file_store.end(), Alphabetical);
        print_files(file_store); 
        if(errno != 0)
        {
            perror("There was an error with readdir(). ");
            exit(1);
        }
        cout << endl;
        if(-1 == closedir(dirp))
        {
            perror("There was an error with closedir(). ");
        }
       
    
    return 0;
}
