// Project4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "mDisk.h"

//must be multiple of 512, size of a sector
#define BLOCKSIZE 10240

using namespace std;

bool checkBitmap(char* data, long long& nBytes)
{
	if (data[0] != 'B' || data[1] != 'M')
		return false;

	//if this is a bitmap file, return the file size
	nBytes = *(unsigned int*)(data + 2);
	return true;
}

bool checkWav(char* data, long long& nBytes)
{
	if (string(data, data + 4) != "RIFF" && string(data + 8, data + 12) != "WAVE")
		return false;
	
	nBytes = *((unsigned int*)(data + 4));
	nBytes += 8;
	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	wstring volume;
	int recoveryTypes;
	cout << "Please enter the volume name: ";
	wcin >> volume;
	cout << "Please enter file'types you want to recover: ";
	cin >> recoveryTypes;
	cout << "Searching...";

	mDisk disk(volume);
	char* xxx = new char[1111];
	int temp = 0;
	//disk.readSector(xxx, 0, 1024, temp);

	//set bit 0 to find bitmap, set bit 1 to find ...
	
	//number of file recovered by the program
	int rCount = 0;

	int numberOfBytesRead = 1;
	bool isEnding = false;
	//current reading sector on disk
	long long currentSector = 0;
	//data read on disk
	char* data = new char[BLOCKSIZE];

	for (;!isEnding;)
	{
		/*cout << currentSector << endl;
		if (currentSector == 102290)
		{
			int xxx = 3;
			xxx += 10;
		}*/
		numberOfBytesRead = 0;
		disk.readSector(data, currentSector, 512, numberOfBytesRead);
		isEnding = numberOfBytesRead <= 0;
		
		wstring name;
		long long fileSize = 0;
		
		//check if there is a file to recover
		if ((recoveryTypes & 1) && checkBitmap(data, fileSize))
		{
			//set file name
			wstringstream f;
			f << rCount << ".bmp";
			name = f.str();
			++rCount;
		}
		else
		if ((recoveryTypes & 2) && checkWav(data, fileSize))
		{
			wstringstream f;
			f << rCount << ".wav";
			name = f.str();
			++rCount;
		}

		//check if find a file to recover
		if (fileSize == 0)
		{
			++currentSector;
			continue;
		}
			
		//recover file
		HANDLE output = CreateFile(name.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, 0,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		
		while (fileSize > 0)
		{
			int numberOfBytesRead = 0;

			//set number of bytes to read from disk
			int numberOfBytesToRead = min(BLOCKSIZE, fileSize);
			disk.readSector(data, currentSector, numberOfBytesToRead, numberOfBytesRead);
			
			//Check if finish reading disk
			isEnding = numberOfBytesToRead > numberOfBytesRead;
			if (isEnding)
				break;

			//Write data to file
			fileSize -= numberOfBytesRead;
			WriteFile(output, data, (DWORD)numberOfBytesRead, 0, 0);

			//change current sector read on disk
			currentSector += numberOfBytesRead / 512 + int(numberOfBytesRead % 512 > 0);

		}
		CloseHandle(output);	
	}

	cout << "Finish.\n" << rCount << " file(s) have been recovered.";
	system("pause");
	return 0;
}

