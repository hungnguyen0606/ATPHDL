#ifndef _MY_FAT_TABLE_H_
#define _MY_FAT_TABLE_H_

#include "stdafx.h"

//must be multiple of 4 and 2 (which is the entry's size)
class myFat16;
class myFat32;
#define DATASIZE 1024

class myFat16
{
public:
	static bool isFree(int cluster)
	{
		return cluster == (int)0;
	}
	static bool isBad(int cluster)
	{
		return cluster == (int)0xFFF7;
	}
	static bool isEOF(int cluster)
	{
		return cluster == (int)0xFFFF;
	}
	static bool isUsed(int cluster)
	{
		return !(isFree(cluster) || isBad(cluster) || isEOF(cluster));
	}
};

class myFat32
{
public:
	static bool isFree(int cluster)
	{
		return cluster == (int)0;
	}
	static bool isBad(int cluster)
	{
		return cluster == (int)0x0FFFFFF7;
	}
	static bool isEOF(int cluster)
	{
		return cluster == (int)0x0FFFFFFF;
	}
	static bool isUsed(int cluster)
	{
		return !(isFree(cluster) || isBad(cluster) || isEOF(cluster));
	}
};

class myFatTable
{
protected:
	
	HANDLE vol;
	//offset to first sector of fat table
	LARGE_INTEGER offset;
	//4 (or 2) bytes if Fat32 (fat 16)
	int entrySize;
	int entriesPerDat;
	//current starting sector on data;
	unsigned int curStartingCluster;
	//current ending sector on data;
	unsigned int curEndingCluster;
	//data
	char data[DATASIZE];
public:
	myFatTable(HANDLE Volume, LARGE_INTEGER offset, int entrySize);
	~myFatTable();

	bool isFree(int cluster)
	{
		return entrySize == 2 ? myFat16::isFree(cluster) : myFat32::isFree(cluster);
	}
	bool isBad(int cluster)
	{
		return entrySize == 2 ? myFat16::isBad(cluster) : myFat32::isBad(cluster);
	}
	bool isEOF(int cluster)
	{
		return entrySize == 2 ? myFat16::isEOF(cluster) : myFat32::isEOF(cluster);
	}
	bool isUsed(int cluster)
	{
		return entrySize == 2 ? myFat16::isUsed(cluster) : myFat32::isUsed(cluster);
	}

	unsigned int operator[](unsigned int id);
};

#endif