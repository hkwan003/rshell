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

void print_files(vector<string> V)
{
    for(int x = 0; x < V.size(); x++)
    {
        cout << V.at(x) << " ";
    }
}







int main(int argc, char ** argv)
{ 
    vector<string> file_store;
      
    if(argc <= 1)
    {
        cout << "Nothing passed in to argv." << endl;
        exit(1);
    }
    else
    {
        char* Sec_Arr = new char[2];
        Sec_Arr[0] = '.';
        strcpy(Sec_Arr,".");
        argv[0] = Sec_Arr;
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
    }

    return 0;
}
