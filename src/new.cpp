#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <string>
#include <stack>
#include <time.h>
#include <pwd.h>
#include <grp.h>

using namespace std;

string filePermission(mode_t file)
{
    string file_permiss;
    if(S_ISDIR(file)){file_permiss.push_back('d');}
    else if(S_ISBLK(file)){file_permiss.push_back('b');}
    else if(S_ISCHR(file)){file_permiss.push_back('c');}
    else if(S_ISLNK(file)){file_permiss.push_back('l');}
    else if(S_ISFIFO(file)) {file_permiss.push_back('p');}
    else if(S_ISSOCK(file)) {file_permiss.push_back('s');}
    if(file & S_IRUSR) {file_permiss.push_back('r');}
    else{file_permiss.push_back('-');}
    if(file & S_IWUSR){file_permiss.push_back('w');}
    else{file_permiss.push_back('-');}
    if(file & S_IXUSR){file_permiss.push_back('x');}
    else{file_permiss.push_back('-');}
    if(file & S_IRGRP){file_permiss.push_back('r');}
    else {file_permiss.push_back('-');}
    if(file & S_IWGRP){file_permiss.push_back('w');}
    else {file_permiss.push_back('-');}
    if(file & S_IXGRP) {file_permiss.push_back('x');}
    else {file_permiss.push_back('-');}
    if(file & S_IROTH) {file_permiss.push_back('r');}
    else {file_permiss.push_back('-');}
    if(file & S_IWOTH) {file_permiss.push_back('w');}
    else {file_permiss.push_back('-');}
    if(file & S_IXOTH) {file_permiss.push_back('x');}
    else {file_permiss.push_back('-');}
    return file_permiss;   
}
bool alphabetical(string c1, string c2)
{
    for(unsigned int i = 0; i < c1.size(); i++)
    {
        c1.at(i) = tolower(c1.at(i));
    }
    for(unsigned int i = 0; i < c2.size(); i++)
    {
        c2.at(i) = tolower(c2.at(i));
    }
    return c1.compare(c2) < 0;
}
string displayColorText(mode_t m, string filename)
{
	string color;
	string bg = "49";
	if (S_ISDIR(m)) color = "34"; 
	else if (m & S_IXUSR) color =  "32";
	else if (S_ISBLK(m)) color = "33";
	else if (S_ISCHR(m)) color = "35";
	else if (S_ISLNK(m)) color = "31";
	else if (S_ISFIFO(m)) color = "36";
	else if (S_ISSOCK(m)) color = "92";
	if (filename.at(0) == '.') bg = "100";
	return "\033[1;" + bg + ";" + color + "m" + filename + "\033[0;00m";
}


void execute(vector<string> filenames, const vector<bool> flags, string currentpath, stack<string> &final_change)
{
	struct stat info;
	int totalblocks = 0;
	for (unsigned i = 0; i < filenames.size(); ++i)
	{
		if (filenames[i].at(0) != '.' || flags[0])
		{
			string updatedpath(currentpath + "/" + filenames[i]);
			if (stat(updatedpath.c_str(), &info) != 0)
			{
				perror("Error with stat()");
				exit(1);
			}
			if (flags[2] && S_ISDIR(info.st_mode) && filenames[i] != "." && filenames[i] != "..") 
			{
				final_change.push(updatedpath);
			}
			if (flags[1])
			{
				cout << filePermission(info.st_mode) << "\t";
				cout << info.st_nlink << "\t";
				struct passwd userinfo = *getpwuid(info.st_uid);
				if (&userinfo == NULL)
				{
					perror("Error with getpwuid()");
					exit(1);
				}
				cout << userinfo.pw_name << "\t";
				struct group groupinfo = *getgrgid(info.st_gid);
				if (&groupinfo == NULL)
				{
					perror("Error with getgrgid()");
					exit(1);
				}
				cout << groupinfo.gr_name << "\t";
				cout << info.st_size << "\t";
				char* timeinfo = ctime(&info.st_mtime);
				if (timeinfo == NULL)
				{
					perror("Error with ctime()");
					exit(1);
				}
				string displaytime(timeinfo);
				cout << displaytime.substr(4, 12) << "\t";
				totalblocks += info.st_blocks;
			}
            if(!flags[1] || !flags[2])
            { 
		        cout << displayColorText(info.st_mode, filenames[i]) << "  ";
            }
	        else
            {
                cout << displayColorText(info.st_mode, filenames[i]) << endl;
            }
		}
	}
	if (flags[1]) 
    {
        cout << "Total blocks: " << totalblocks / 2 << endl;
    }
    if(!flags[1] || !flags[2]) 
    {
        cout << endl;
    }
}
void string_parse(char *prompt, vector<bool> &flag_hold, vector<string> &pathnames)
{
    if(prompt[0] == '-')
    {
        if(prompt[1] == '\0')
        {cout << "Error: please input in more arguments" << endl;}
        for(int x = 1; prompt[x] != '\0'; x++)
        {
            if(prompt[x] == 'a')
            {flag_hold[0] = true;}
            else if(prompt[x] == 'l')
            {flag_hold[1] = true;}
            else if(prompt[x] == 'R')
            {flag_hold[2] = true;}
        }
    }
    else
    {
        string convert(prompt);
        pathnames.push_back(convert);
    }
}

vector<string> paths;
vector<string> filenames;
stack<string> final_change;
string directory_name;
int main(int argc, char *argv[])
{
    vector<bool> flags(false);      //initalizes all vector to be false
    for(unsigned int x = 0; x < 3; x++){flags.push_back(false);}
	if (argc > 1)
	{
		for (int i = 1; i < argc; ++i)
		{
            string_parse(argv[i], flags, paths);
		}
	}	
	if(paths.size() == 0){paths.push_back(".");}	
	dirent *readfile;
	for (int i = paths.size() - 1; i >= 0; --i) 
    {
        final_change.push(paths[i]);
    }
	while (!final_change.empty())
	{
		directory_name = final_change.top();
		final_change.pop();
		DIR *dirp = opendir(directory_name.c_str());
		if (dirp == NULL)
		{
			perror("Error with opendir()");
			exit(1);
		}
		while((readfile = readdir(dirp)))
		{
			if (readfile < 0)
			{
				perror("Error with readdir()");
				exit(1);
			}
			string s(readfile->d_name);
			filenames.push_back(s);
		}
		sort(filenames.begin(), filenames.end(), alphabetical);//sorts directories and files in vector alphabetcially
		execute(filenames, flags, directory_name, final_change);
		if (closedir(dirp) == -1)
		{
			perror("Error with closedir()");
			exit(1);
		}
		filenames.clear();
		if (!final_change.empty()) 
        {   
            cout << endl;
        }
	}
}
