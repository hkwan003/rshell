#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main()
{
	string command;
	cout << "$ ";
	getline(cin,command);
	cout << command << endl;
	vector<char > tokenizer(2*command.size());
	for(int x = 0; x < command.size(); x++)
	{
		tokenizer[x] = command.at(x);
	}
    cout << "output size of string: " << command.size() << endl;
    cout << "output size of vector: " << tokenizer.size() << endl;















}	
