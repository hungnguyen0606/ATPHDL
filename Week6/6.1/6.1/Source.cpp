#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include <string>
#include <iterator>
#include <fstream>
using namespace std;

#define BLOCKSIZE 8*512

typedef long long int64;

char block[BLOCKSIZE];

int split(char** argv)
{
	ifstream ifs(argv[1], std::ios::binary);
	if (!ifs.good())
	{
		cout << "Cannot open file.";
		return 0;
	}
	ofstream ofs(argv[2], ios::binary);
	//FILE* g = fopen("\\\\.\\I:", "w+r");
	//ofs.write(block, 1024);
	if (!ofs.good())
	{
		cout << "Cannot find your usb.";
		return 0;
	}



	string newName = string(argv[1], argv[1] + strlen(argv[1])) + string("_HUNG");
	ofstream ret(newName, std::ios::binary);
	if (!ret.good())
	{
		cout << "Something's wrong here. Sorry.";
		return 0;
	}

	while (!(ifs.fail() && ifs.eof()))
	{
		ifs.read(block, BLOCKSIZE);
		int lastRead = ifs.gcount();
		ret.write(block, lastRead / 2);
		int left = lastRead - lastRead / 2;
		ofs.write(block + lastRead / 2, left);
	}

	ret.close();
	ofs.close();
	ifs.close();

	remove(argv[1]);
	rename(newName.c_str(), argv[1]);
}

int concat(char** argv)
{
	ifstream ifs(argv[1], std::ios::binary);
	if (!ifs.good())
	{
		cout << "Cannot open file.";
		return 0;
	}
	ifstream ifs2(argv[2], ios::binary);
	//FILE* g = fopen("\\\\.\\I:", "w+r");
	//ofs.write(block, 1024);
	if (!ifs2.good())
	{
		cout << "Cannot find your usb.";
		return 0;
	}

	string newName = string(argv[1], argv[1] + strlen(argv[1])) + string("_HUNG");
	ofstream ret(newName, std::ios::binary);
	if (!ret.good())
	{
		cout << "Something's wrong here. Sorry.";
		return 0;
	}

	while (!(ifs.fail() && ifs.eof()))
	{
		ifs.read(block, BLOCKSIZE / 2);
		ifs2.read(block + ifs.gcount(), BLOCKSIZE - ifs.gcount());
		int x = ifs.gcount() + ifs2.gcount();
		ret.write(block, ifs.gcount() + ifs2.gcount());
	}
	ifs.close();
	ifs2.close();
	ret.close();

	remove(argv[1]);
	remove(argv[2]);
	rename(newName.c_str(), argv[1]);
}
int main(int argc, char** argv)
{
	if (argc != 4)
	{
		cout << "Wrong format";
		return 0;
	}
	
	if (strcmp(argv[3], "split") == 0)
		split(argv);
	else
		concat(argv);


	
	return 0;
}