#include "stdafx.h"
#include "mDisk.h"

#define MAXSECTOR 1000

mDisk::mDisk()
{
}

mDisk::mDisk(std::wstring Volume)
{
	vol = CreateFile(Volume.c_str(), GENERIC_READ, FILE_SHARE_READ, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	this->disk = new char[MAXSECTOR * 512];
	this->ending = -1;
	this->starting = 0;
	//ReadFile(vol, disk, 512, 0, 0);
}


mDisk::~mDisk()
{
	if (this->disk != NULL)
		delete[] this->disk;
	CloseHandle(vol);
}

/*
void mDisk::readSector(char* data, long long startingSector, int nBytes, int& numberOfBytesRead)
{
	

	//testing read 1 sector 
	if (nBytes <= 0)
		return;
	DWORD nOfRead = 0;
	//if (startingSector > this->ending)
	//{
	LARGE_INTEGER offset;
	offset.QuadPart = startingSector;
	offset.QuadPart *= 512;
	SetFilePointerEx(this->vol, offset, 0, FILE_BEGIN);

	//int nSector = nBytes / 512 + int(nBytes % 512 > 0);
	//int nRead = 512;
		
	ReadFile(this->vol, this->disk, 512, &nOfRead, 0);
		
	this->starting = startingSector;
	this->ending = startingSector;
	//}
	
	memcpy(data, this->disk, nOfRead);
	numberOfBytesRead += nOfRead;
	
	this->readSector(data + 512, startingSector + 1, nBytes - 512, numberOfBytesRead);
}
*/

void mDisk::readSector(char* data, long long startingSector, int nBytes, int& numberOfBytesRead)
{
	numberOfBytesRead = 0;
	int nBytesLeft = nBytes;
	//pointer to the starting position of data field
	char* pointer = data;
	int numberOfBytesToRead;

	while (nBytesLeft > 0)
	{
		if (startingSector > this->ending || startingSector < this->starting)
		{
			LARGE_INTEGER offset;
			offset.QuadPart = startingSector;
			offset.QuadPart *= 512;
			SetFilePointerEx(this->vol, offset, 0, FILE_BEGIN);

			DWORD nOfRead;
			ReadFile(this->vol, this->disk, (DWORD)MAXSECTOR*512, &nOfRead, 0);

			this->starting = startingSector;
			this->ending = this->starting + (nOfRead / 512 + int(nOfRead % 512 > 0)) - 1;
		}

		if (this->ending < startingSector)
			return;
		//point to the position of startingSector between this->starting and this->ending
		char* startingPointer = this->disk + (startingSector - this->starting) * 512;
		int numberOfBytesToCopy = min((this->ending - startingSector + 1) * 512, nBytesLeft);
		memcpy(pointer, startingPointer, numberOfBytesToCopy);
		
		numberOfBytesRead += numberOfBytesToCopy;
		nBytesLeft -= numberOfBytesToCopy;
		//shift the pointer to the next position of data field to copy
		pointer += numberOfBytesToCopy;
		//shift the starting sector to the next position in disk
		startingSector += numberOfBytesToCopy / 512 + int(numberOfBytesToCopy % 512 > 0);
	}
	
}
