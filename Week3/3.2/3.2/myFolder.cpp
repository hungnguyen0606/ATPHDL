#include "stdafx.h"
#include "myFolder.h"
#include "mySDK.h"


myFolder::myFolder(HANDLE vol, myFatTable* fat, unsigned int startingCluster, LARGE_INTEGER dataArea, int clustersize)
{
	data = new char[clustersize];
	this->clustersize = clustersize;
	this->maxEntry = this->clustersize / 32;
	this->currentEntry = 0;
	this->currentCluster = startingCluster;
	this->offsetDataArea = dataArea;
	this->vol = vol;
	this->fat = fat;
	readCluster(this->vol, this->data, currentCluster, this->clustersize, this->offsetDataArea);
}


myFolder::~myFolder()
{
	delete[]data;
}

char* myFolder::getEntryData(int entryID)
{
	return &this->data[entryID * 32];
}
bool myFolder::isEmptyEntry(char* entry)
{
	for (int i = 0; i < 32; ++i)
		if (entry[i])
			return false;
	
	return true;
}
bool myFolder::getNextCluster()
{
	currentEntry = 0;
	//nhớ điều chỉnh offset và cluster size
	LARGE_INTEGER offset;
	if (fat->isEOF((*fat)[currentCluster]))
		return false;
	unsigned int nextCluster =(*fat)[currentCluster];
	readCluster(this->vol, this->data, nextCluster, clustersize, this->offsetDataArea);
	currentCluster = nextCluster;
	return true;
}
bool myFolder::getNextEntry(char* entryDat, std::wstring& name)
{
	if (currentEntry >= maxEntry)
		if (!getNextCluster())
			return false;

	char* temp = getEntryData(currentEntry);
	while (temp[0] == (char)0xE5)
	{
		++currentEntry;
		if (currentEntry >= maxEntry)
			if (!getNextCluster())
				return false;
		temp = getEntryData(currentEntry);
	}

	if (isEmptyEntry(temp))
		return false;
	name = L"";
	char x[13];
	
	//long file name
	if (temp[11] == 0x0F)
	{
		int n = temp[0] ^ 0x40;
		//TCHAR x = *((TCHAR*)temp[30]);
		/*for (int i = 30; i >= 28; i -= 2)
			name = *((TCHAR*)&temp[i]) + name;
		for (int i = 24; i >= 14; i -= 2)
			name = *((TCHAR*)&temp[i]) + name;
		for (int i = 9; i >= 1; i -= 2)
			name = *((TCHAR*)&temp[i]) + name;*/
		
		name = std::wstring((TCHAR*)(temp + 1), (TCHAR*)(temp + 1) + 5)
			+ std::wstring((TCHAR*)(temp + 14), (TCHAR*)(temp + 14) + 6)
			+ std::wstring((TCHAR*)(temp + 28), (TCHAR*)(temp + 28) + 2);
		name.resize(lstrlen(name.c_str()));
		--n;
		while (n--)
		{
			++currentEntry;
			if (currentEntry >= maxEntry)
				if (!getNextCluster())
					return false;
			temp = getEntryData(currentEntry);
			name = std::wstring((TCHAR*)(temp + 1), (TCHAR*)(temp + 1) + 5) 
				+ std::wstring((TCHAR*)(temp + 14), (TCHAR*)(temp + 14) + 6)
				+ std::wstring((TCHAR*)(temp + 28), (TCHAR*)(temp + 28) + 2) + name;
			
		}
		++currentEntry;
		if (currentEntry >= maxEntry)
			if (!getNextCluster())
				return false;
		temp = getEntryData(currentEntry);
		
		std::memcpy(entryDat, temp, 32);
	}
	//short file name
	else
	{
		std::wstring n1(L""), n2(L"");
		for (int i = 0; i < 8; ++i)
		{
			if (temp[i] == (char)0x20)
				break;
			n1 += temp[i];
		}
		for (int i = 8; i < 11; ++i)
		{
			if (temp[i] == (char)0x20)
				break;
			n2 += temp[i];
		}
		if (n2.length())
			name = n1 + L"." + n2;
		else
			name = n1;
		std::memcpy(entryDat, temp, 32);
		//std::wstring n1(x);
	}
	++currentEntry;
	return true;
}
//
//bool myFolder::getNextEntry(char* entryDat, std::wstring& name)
//{
//	if (currentEntry >= maxEntry)
//	if (!getNextCluster())
//		return false;
//
//	char* temp = getEntryData(currentEntry);
//	while (temp[0] == (char)0xE5)
//	{
//		++currentEntry;
//		if (currentEntry >= maxEntry)
//		if (!getNextCluster())
//			return false;
//		temp = getEntryData(currentEntry);
//	}
//
//	if (isEmptyEntry(temp))
//		return false;
//	name = L"";
//	char x[13];
//
//	//long file name
//	if (temp[11] == 0x0F)
//	{
//		int n = temp[0] ^ 0x40;
//		
//
//		name = std::wstring((TCHAR*)(temp + 1), (TCHAR*)(temp + 1) + 5)
//			+ std::wstring((TCHAR*)(temp + 14), (TCHAR*)(temp + 14) + 6)
//			+ std::wstring((TCHAR*)(temp + 28), (TCHAR*)(temp + 28) + 2);
//		name.resize(lstrlen(name.c_str()));
//		--n;
//		while (n--)
//		{
//			++currentEntry;
//			if (currentEntry >= maxEntry)
//			if (!getNextCluster())
//				return false;
//			temp = getEntryData(currentEntry);
//			name = std::wstring((TCHAR*)(temp + 1), (TCHAR*)(temp + 1) + 5)
//				+ std::wstring((TCHAR*)(temp + 14), (TCHAR*)(temp + 14) + 6)
//				+ std::wstring((TCHAR*)(temp + 28), (TCHAR*)(temp + 28) + 2) + name;
//
//		}
//		++currentEntry;
//		if (currentEntry >= maxEntry)
//		if (!getNextCluster())
//			return false;
//		temp = getEntryData(currentEntry);
//
//		std::memcpy(entryDat, temp, 32);
//	}
//	//short file name
//	else
//	{
//		std::wstring n1(L""), n2(L"");
//		for (int i = 0; i < 8; ++i)
//		{
//			if (temp[i] == (char)0x20)
//				break;
//			n1 += temp[i];
//		}
//		for (int i = 8; i < 11; ++i)
//		{
//			if (temp[i] == (char)0x20)
//				break;
//			n2 += temp[i];
//		}
//		if (n2.length())
//			name = n1 + L"." + n2;
//		else
//			name = n1;
//		std::memcpy(entryDat, temp, 32);
//		//std::wstring n1(x);
//	}
//	++currentEntry;
//	return true;
//}
