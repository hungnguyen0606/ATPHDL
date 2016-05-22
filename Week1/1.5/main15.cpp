#include <cstdio>
#include <cstring>
#include <iostream>
#include <utility>
#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

void encrypt(const char* pass, const char* file)
{
	FILE* f = fopen(file, "r+b");
	
	if (f == NULL)
		return;
	
	int n = strlen(pass);
	int lastlen = n + 1;

	char* buff = new char[n + 1];
	memset(buff, 0, n);
	while (lastlen >= n)
	{
		fpos_t temp;
		fgetpos(f, &temp);
		lastlen = fread(buff, 1, n, f);
		
		for (int i = 0; i < lastlen; ++i)
			buff[i] = buff[i] ^ pass[i];
		
		fsetpos(f, &temp);
		fwrite(buff, 1, lastlen, f);
		fseek(f, 0, SEEK_CUR);
	}

	delete[]buff;
	fclose(f);

}
int main()
{
	string pass, file;
	cout << "Password: ";
	cin >> pass;
	cout << "File to (encrypt/decrypt): ";
	cin >> file;
	encrypt(pass.c_str(), file.c_str());
	
	return 0;
}
