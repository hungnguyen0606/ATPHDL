#include "stdafx.h"
#include "VolumeInfo.h"
#include "myFolder.h"
#include "myFatTable.h"
#include "BootSector.h"
using namespace std;
//vol: handle to volume, clusterID: cluster you want to read, clusterSize: cluster's size in bytes
//offset: offset to first cluster of volume in bytes
void readCluster(HANDLE vol, char* data, unsigned int clusterID, DWORD clusterSize, LARGE_INTEGER offset)
{
	LARGE_INTEGER clusID, clusSize;
	clusID.QuadPart = clusterID - 2;
	clusSize.QuadPart = clusterSize;
	offset.QuadPart += clusID.QuadPart*clusSize.QuadPart;
	SetFilePointerEx(vol, offset, 0, FILE_BEGIN);
	
	ReadFile(vol, data, clusterSize, 0, 0);
}

void readSector(HANDLE vol, char* data, LARGE_INTEGER sectorID, DWORD sectorSize)
{
	LARGE_INTEGER offset;
	offset.QuadPart = sectorSize;
	offset.QuadPart *= sectorID.QuadPart;
	SetFilePointerEx(vol, offset, 0, FILE_BEGIN);
	
	ReadFile(vol, data, sectorSize, 0, 0);
}

void getPathVolume(const TCHAR* vol, TCHAR* ret)
{
	swprintf(ret, L"\\\\.\\%s", vol);
	//swprintf(ret, "\\\\.\\%s:", vol);
}

void splitPath(TCHAR *path, vector<wstring>& pathList)
{
	pathList.clear();

	int last = 0;
	int n = lstrlen(path);
	for (int i = 0; i < n; ++i)
		if (path[i] == L'\\' || path[i] == L'/')
		{
			if (i > last)
				pathList.push_back(wstring(&path[last], &path[i]));
			last = i + 1;
		}

	pathList.push_back(wstring(&path[last], &path[n]));
}

unsigned int findNextCluster(HANDLE vol,  VolumeInfo volInfo, 
	myFatTable mfat, unsigned int currentCluster, wstring name, unsigned int& filesize)
{
	unsigned int ret;
	myFolder mfolder(vol, &mfat, currentCluster, volInfo.offsetDataArea,
		int(volInfo.bytesPerSector)*volInfo.sectorsPerCluster);
	char entry[100];
	wstring tempName;
	while (mfolder.getNextEntry(entry, tempName))
	{
		if (tempName == name)
			break;
	}
	ret = unsigned int(*(unsigned __int16*)(entry + 26)) + (unsigned int(*(unsigned __int16*)(entry + 20)) << 16);
	filesize = *(unsigned int*)(entry + 28);
	return ret;
}

void getVolumeInfo(char* data, VolumeInfo& volInfo)
{
	
	

	
	if (((mBoot32*)data)->isFat32())
	{
		mBoot32 &boot = *((mBoot32*)data);
		volInfo.bytesPerSector = boot.bytesPerSector;
		volInfo.sectorsPerCluster = boot.sectorsPerCluster;
		volInfo.setFat32(true);

		volInfo.offsetFatTable.QuadPart = boot.sectorsOnBoot;
		volInfo.offsetFatTable.QuadPart *= 512;

		volInfo.offsetDataArea.QuadPart = boot.fatSize;
		volInfo.offsetDataArea.QuadPart *= 512 * 2;
		volInfo.offsetDataArea.QuadPart += volInfo.offsetFatTable.QuadPart;

	}
	else
	{
		mBoot16 &boot = *((mBoot16*)data);
		volInfo.bytesPerSector = boot.bytesPerSector;
		volInfo.sectorsPerCluster = boot.sectorsPerCluster;
		
		volInfo.setFat32(false);

		volInfo.offsetFatTable.QuadPart = boot.sectorsOnBoot;
		volInfo.offsetFatTable.QuadPart *= 512;

		
		volInfo.offsetRdet.QuadPart = boot.sectorPerFat;
		volInfo.offsetRdet.QuadPart *= boot.nFat*512;
		volInfo.offsetRdet.QuadPart += volInfo.offsetFatTable.QuadPart;
		//size of rdet in byte
		unsigned int sRdet = (boot.nEntry * 32 / 512 + unsigned int(boot.nEntry * 32 % 512 > 0)) * 512;
		volInfo.offsetDataArea.QuadPart = volInfo.offsetRdet.QuadPart + sRdet;
		
	}
}
unsigned int findRdetEntry(HANDLE vol, VolumeInfo volInfo, std::wstring filename, unsigned int& remainingFileSize);
void CopyContent(TCHAR *path, TCHAR *dest)
{
	vector<wstring> fList;
	splitPath(path, fList);

	//open volume
	TCHAR temp[1024];
	getPathVolume(fList[0].c_str(), temp);
	HANDLE vol = CreateFile(temp, GENERIC_READ, FILE_SHARE_READ, 0, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//
	char dataBoot[1024];
	unsigned int currentCluster;
	VolumeInfo volInfo;

	LARGE_INTEGER dis;
	dis.QuadPart = 0;
	SetFilePointerEx(vol, dis, 0, FILE_BEGIN);
	ReadFile(vol, dataBoot, 512, 0, 0);
	getVolumeInfo(dataBoot, volInfo);

	myFatTable mfat(vol, volInfo.offsetFatTable, volInfo.fatEntrySize);
	bool isFat16;
	//
	unsigned int  remainingFileSize = 0;
	int starting;
	if (volInfo.isFat32)
	{
		currentCluster = ((mBoot32*)(dataBoot))->rdetStartingCluster;
		starting = 1;
	}
	else
	{
		currentCluster = findRdetEntry(vol, volInfo, fList[1], remainingFileSize);
		starting = 2;
	}
	//Finding starting cluster of file
	
	for (int i = starting; i < fList.size(); ++i)
	{
		currentCluster = findNextCluster(vol, volInfo, mfat, currentCluster, fList[i], remainingFileSize);
	}
	//starting to copy content
	HANDLE output = CreateFile(dest, GENERIC_WRITE, FILE_SHARE_WRITE, 0, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	unsigned int clusSize = unsigned int(volInfo.bytesPerSector)*volInfo.sectorsPerCluster;
	char *buff = new char[clusSize];
	LARGE_INTEGER offset, tempSize;
	
	while (remainingFileSize > 0)
	{
		readCluster(vol, buff, currentCluster, clusSize, volInfo.offsetDataArea);
		DWORD nbyteread = clusSize;
		
		if (clusSize > (remainingFileSize))
			nbyteread = DWORD(remainingFileSize);
		
		remainingFileSize -= nbyteread;

		WriteFile(output, buff, nbyteread, 0, 0);

		if (mfat.isEOF(currentCluster))
			break;
		currentCluster = mfat[currentCluster];
	}
	CloseHandle(output);
	delete[]buff;
}

unsigned findRdetEntry(HANDLE vol, VolumeInfo volInfo, std::wstring filename,  unsigned int& remainingFileSize)
{
	unsigned int retCluster = 0;
	//if (currentEntry >= maxEntry)
	int maxEntry = volInfo.bytesPerSector / 32;
	int currentEntry = 0;
	LARGE_INTEGER currentSector, maxRdetSector;
	maxRdetSector.QuadPart = volInfo.offsetDataArea.QuadPart / volInfo.bytesPerSector;
	currentSector.QuadPart = volInfo.offsetRdet.QuadPart / volInfo.bytesPerSector;
	char data[512];
	readSector(vol, data, currentSector, volInfo.bytesPerSector);
	int nEntryPerSector = volInfo.bytesPerSector/32;
	while (currentSector.QuadPart < maxRdetSector.QuadPart)
	{
		std::wstring name = L"";
		if (currentEntry >= maxEntry)
		{
			currentSector.QuadPart += 1;
			if (currentSector.QuadPart >= maxRdetSector.QuadPart)
				return false;
			readSector(vol, data, currentSector, volInfo.bytesPerSector);
			currentEntry = 0;
		}
		char* temp = &data[32 * currentEntry];
		while (temp[0] == (char)0xE5)
		{
			++currentEntry;
			if (currentEntry >= maxEntry)
			{
				currentSector.QuadPart += 1;
				if (currentSector.QuadPart >= maxRdetSector.QuadPart)
					return false;
				readSector(vol, data, currentSector, volInfo.bytesPerSector);
				currentEntry = 0;
			}
			temp = &data[32 * currentEntry];
		}

		if (temp[0] == 0)
			return false;
		name = L"";
		char x[13];

		//long file name
		if (temp[11] == 0x0F)
		{
			int n = temp[0] ^ 0x40;


			name = std::wstring((TCHAR*)(temp + 1), (TCHAR*)(temp + 1) + 5)
				+ std::wstring((TCHAR*)(temp + 14), (TCHAR*)(temp + 14) + 6)
				+ std::wstring((TCHAR*)(temp + 28), (TCHAR*)(temp + 28) + 2);
			name.resize(lstrlen(name.c_str()));
			--n;
			while (n--)
			{
				++currentEntry;
				if (currentEntry >= maxEntry)
				if (currentEntry >= maxEntry)
				{
					currentSector.QuadPart += 1;
					if (currentSector.QuadPart >= maxRdetSector.QuadPart)
						return false;
					readSector(vol, data, currentSector, volInfo.bytesPerSector);
					currentEntry = 0;
				}
				temp = &data[currentEntry * 32];
				name = std::wstring((TCHAR*)(temp + 1), (TCHAR*)(temp + 1) + 5)
					+ std::wstring((TCHAR*)(temp + 14), (TCHAR*)(temp + 14) + 6)
					+ std::wstring((TCHAR*)(temp + 28), (TCHAR*)(temp + 28) + 2) + name;

			}
			++currentEntry;
			if (currentEntry >= maxEntry)
			{
				currentSector.QuadPart += 1;
				if (currentSector.QuadPart >= maxRdetSector.QuadPart)
					return false;
				readSector(vol, data, currentSector, volInfo.bytesPerSector);
				currentEntry = 0;
			}
			temp = &data[currentEntry * 32];
			if (name == filename)
			{
				remainingFileSize = *(unsigned int*)(temp + 28);
				return unsigned int(*(__int16*)(temp + 26)) + (unsigned int(*(__int16*)(temp + 20)) << 16);
			}
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
			if (name == filename)
			{
				remainingFileSize = *(unsigned int*)(temp + 28);
				return unsigned int(*(__int16*)(temp + 26)) + (unsigned int(*(__int16*)(temp + 20)) << 16);
			}
				
			//std::wstring n1(x);
		}
		++currentEntry;
	}

	
	return retCluster;
}