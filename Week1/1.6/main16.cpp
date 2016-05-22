#include <cstdio>
#include <cstring>
#include <iostream>
#include <utility>
#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

#define ENSIZE 1024
void encrypt(const char* pass, const char* file)
{
	
	FILE* f = fopen(file, "r+b");

	if (f == NULL)
		return;

	int n = strlen(pass);
	int lastlen;

	//n = 1024;
	char* buff = new char[ENSIZE];
	lastlen = fread(buff, 1, 1024, f);
	for (int i = 0; i < lastlen; ++i)
		buff[i] = buff[i] ^ pass[i%n];

	fseek(f, 0, SEEK_SET);
	fwrite(buff, 1, lastlen, f);

	delete[]buff;
	fclose(f);

}
int main()
{
	string pass, file;
	cout << "Password: ";
	cin >> pass;
	cout << "Volume to (encrypt/decrypt): ";
	cin >> file;
	file = "\\\\.\\" + file + ":";
	encrypt(pass.c_str(), file.c_str());

	return 0;
}
