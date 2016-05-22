#ifndef _MY_FOLDER_H_
#define _MY_FOLDER_H_

#include "myFatTable.h"

#define MYFOLDERSIZE 4096
class myFolder
{
protected:

	HANDLE vol;
	LARGE_INTEGER offsetDataArea;
	unsigned int currentCluster;
	int clustersize;
	int maxEntry;
	int currentEntry;
	myFatTable* fat;
	char* data;
public:
	//clustersize in bytes
	myFolder(HANDLE vol, myFatTable* fat, unsigned int startingCluster, LARGE_INTEGER dataArea, int clusterSize);
	~myFolder();

	bool isEmptyEntry(char* entry);
	char* getEntryData(int entryID);
	//false: if this is the last cluster of folder, otherwise true
	bool getNextCluster();
	bool getNextEntry(char* entryDat, std::wstring& name);
};

#endif