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



using namespace std;



int main(int argc, char ** argv)
{   
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
            cout << filespecs->d_name << " ";
        }
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
