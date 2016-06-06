#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
using namespace std;

size_t getToken()
{
	hash<string> str_hash;
	time_t t = time(0);
	struct tm * now = localtime(&t);
	now->tm_sec = 0;
	string token((char*)now, ((char*)now) + sizeof(tm));
	return str_hash(token);
}
int main()
{
	while (true)
	{	
		cout << "Your token is: " << getToken();
		_sleep(1000);
		system("CLS");
	}
	return 0;
}