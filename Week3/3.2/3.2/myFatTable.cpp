#include "stdafx.h"
#include "myFatTable.h"


myFatTable::myFatTable(HANDLE Volume, LARGE_INTEGER offset, int entrySize)
{
	this->vol = Volume;
	this->offset = offset;
	this->entrySize = entrySize;
	this->entriesPerDat = DATASIZE / this->entrySize;
	this->curStartingCluster = 1;
	this->curEndingCluster = 0;


}


myFatTable::~myFatTable()
{
}

unsigned int myFatTable::operator[](unsigned int id)
{
	if (curStartingCluster > curEndingCluster || curStartingCluster > id || curEndingCluster < id)
	{
		LARGE_INTEGER offCluster;
		offCluster.QuadPart = id;
		offCluster.QuadPart *= this->entrySize;
		offCluster.QuadPart = (offCluster.QuadPart / DATASIZE * DATASIZE);
		offCluster.QuadPart += this->offset.QuadPart;

		SetFilePointerEx(this->vol, offCluster, 0, FILE_BEGIN);
		ReadFile(vol, this->data, DATASIZE, 0, 0);
		curStartingCluster = id - id % this->entriesPerDat;
		curEndingCluster = curStartingCluster + entriesPerDat - 1;
		return this->operator[](id);
	}
	else
	{
		if (entrySize == 4)
			return *((unsigned int*)&data[id%this->entriesPerDat*this->entrySize]);
		return (unsigned int)(*((unsigned __int16*)&data[id%this->entriesPerDat*this->entrySize]));
	}
}
