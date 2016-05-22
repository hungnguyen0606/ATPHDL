#pragma once
#include <string>
class mDisk
{
private:
	HANDLE vol;
	char* disk;

	//starting and ending sector
	long long starting, ending; 
public:
	mDisk();
	mDisk(std::wstring Volume);
	~mDisk();

	
	void readSector(char* data, long long startingSector, int nBytes, int& numberOfBytesRead);
	
};

