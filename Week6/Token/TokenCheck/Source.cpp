#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
using namespace std;

string getToken()
{
	hash<string> str_hash;
	time_t t = time(0);
	struct tm * now = localtime(&t);
	now->tm_sec = 0;
	string token((char*)now, ((char*)now) + sizeof(tm));
	stringstream f;
	f << str_hash(token);
	return f.str();
}

int main()
{
	while (true)
	{
		cout << "Enter your token: ";
		string token;
		cin >> token;
		if (token == getToken())
			break;
		cout << "Wrong token. Please re-enter your token.\n";	
	}
	cout << "Right token. Exit... ";
	_sleep(2000);
	cout << "Bye bye.";
	return 0;
}