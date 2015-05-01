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
#include <sstream>
#include <pwd.h>
#include <grp.h>

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

void check_colored_output(string s)
{
    int size = s.size();
    char  index = s.at(size - 1);
    if(index == '1')
    {
        cout << "\033[1;34m" << s.substr(0, size - 2) << "/\033[0m ";
    }
    else if(index == '2')
    {
        cout << "\033[1;34m\033[1;40m" << s.substr(0, size - 2)  << "/\033[0m\033[0m ";
    }
    else if(index == '3')
    {
        cout << "\033[1;32m" << s.substr(0, size - 2)  << "*\033[0m ";
    }
    else if(index == '4')
    {
        cout << s.substr(0, size - 2);
    }
    else 
    {
        cout << s;
    }
}
    
int main(int argc, char *argv[])
{
	vector<bool> flags(false);        //initalizes all boolean values in vector to false;      
	flags.resize(3);
	vector<string> paths;
	vector<string> filenames;
	if (argc < 1)
	{
		cout << "Error opening file" << endl;
		exit(1);
	}
	else if (argc > 1) //run ls with path or flags
	{
		for (int i = 1; i < argc; ++i)
		{
			tokenize(flags, argv[i], paths);
		}
	}
	if (!paths.size()) 
    {
        paths.push_back("."); 
    }	
	dirent *direntp;
	stack<string> rdir;
	string processdir;
	for (int i = paths.size() - 1; i >= 0; --i) 
    {
        rdir.push(paths[i]);
    }
	while (!rdir.empty())
	{
		processdir = rdir.top();
		rdir.pop();
		cout << processdir << ": " << endl;
		DIR *dirp = opendir(processdir.c_str());
		if (dirp == NULL)
		{
			perror("Error with opendir()");
			exit(1);
		}
		while ((direntp = readdir(dirp)))
		{
			if (direntp < 0)
			{
				perror("Error with readdir()");
				exit(1);
			}
			string s(direntp->d_name);
			filenames.push_back(s);
		}
		sort(filenames.begin(), filenames.end(), scompare);
		displayls(filenames, flags, processdir, rdir);
		if (closedir(dirp) == -1)
		{
			perror("Error with closedir()");
			exit(1);
		}
		filenames.clear();

		if (!rdir.empty()) cout << endl;
	}
}

