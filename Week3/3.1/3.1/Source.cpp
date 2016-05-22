#include <iostream>
#include "BootSector.h"
#include "mySDK.h"
using namespace std;

char vol[16];
int main()
{
		
	cout << "Enter your volume name (ex: H, A, D, C, E, ...): ";
	cin >> vol;
	mBootSector mbs;
	readBootSector(vol, mbs);
	writeBootSectorToLog(mbs);
	
	return 0;
}