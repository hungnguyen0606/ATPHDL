#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <iterator>
#include "Bitmap.h"
using namespace std;
#define BITHIDE 3
bool hideData(string imagePath, string dataPath)
{
	ifstream ifs(imagePath, ios::binary);
	if (!ifs.good())
		return false;
	ifstream ifsDat(dataPath, ios::binary);
	if (!ifsDat.good())
		return false;

	string bmpdata((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
	string message((istreambuf_iterator<char>(ifsDat)), (istreambuf_iterator<char>()));
	ifs.close();
	ifsDat.close();


	Bitmap mbm(bmpdata);
	int h = mbm.Height();
	int w = mbm.Width();
	if ((h - 1)*w < message.length())
		return false;

	int len = message.length();
	int id = 0;
	for (int i = 1; i < h, id < len; ++i)
	{
		for (int j = 0; j < w, id < len; ++j)
		{
			unsigned char* pix = mbm(i, j);
			unsigned char c = message[id++];
			pix[0] = pix[0] ^ (pix[0] & 15) ^ (c & 15);
			pix[1] = pix[1] ^ (pix[1] & 7) ^ ((c >> 4) & 7);
			pix[2] = pix[2] ^ (pix[2] & 1) ^ ((c >> 7) & 1);

		}
	}
	*(int*)mbm(0, 0) = message.length();
	ofstream ofs(imagePath, ios::binary);
	ofs << string((char*)mbm.data, ((char*)mbm.data) + bmpdata.length());
	ofs.close();
	return true;
}

bool getData(string imagePath, string dataPath)
{
	ifstream ifs(imagePath, ios::binary);
	if (!ifs.good())
		return false;
	ofstream ofs(dataPath, ios::binary);
	if (!ofs.good())
		return false;
	Bitmap bm(string((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>())));
	ifs.close();

	int len = *(int*)bm(0, 0);
	string message("");

	int w = bm.Width();
	int h = bm.Height();


	for (int i = 1, id = 0; i < h, id < len; ++i)
	for (int j = 0; j < w, id < len; ++j)
	{
		unsigned char c = 0;
		unsigned char* pix = bm(i, j);
		c ^= (pix[0] & 15) ^ ((pix[1] & 7) << 4) ^ ((pix[2] & 1) << 7);
		message.push_back(char(c));
		++id;
	}
	
	ofs << message;
	ofs.close();
}
int main(int argc, char** argv)
{
	//hideData("test.bmp", "holla.jpg");
	//getData("test.bmp", "hh.jpg");
	if (argc != 4)
	{
		cout << "Wrong format.";
		return 0;
	}
	if (strcmp(argv[3], "hide") == 0)
	{
		cout << "Hiding data... ";
		if (hideData(argv[1], argv[2]))
			cout << "Successfully";
		else
			cout << "Failed";
	}
	else
	{
		cout << "Restoring data... ";
		if (getData(argv[1], argv[2]))
			cout << "Successfully";
		else
			cout << "Failed";
	}
	return 0;
}