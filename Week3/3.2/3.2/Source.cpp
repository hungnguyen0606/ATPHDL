#include "stdafx.h"
#include <iostream>
#include <cstdio>
#include "mySDK.h"

using namespace std;
TCHAR sr[2000], dt[2000];
int main()
{
	wstring source, dest;
	cout << "Enter your source file:\n";
	getline(wcin, source);
	cout << "Enter your destination (including filename) to save file:\n";
	getline(wcin, dest);
	cout << "Copying...\n";
	lstrcpy(sr, source.c_str());
	lstrcpy(dt, dest.c_str());
	CopyContent(sr, dt);
	cout << "Finish.\n";
	//cout << a;
	//wscanf(L"%[^\n]")
	return 0;
}