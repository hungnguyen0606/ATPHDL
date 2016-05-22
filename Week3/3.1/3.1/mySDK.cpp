#include "stdafx.h"
#include "mySDK.h"
#include <iostream>

using namespace std;

void charToTchar(const char* src, TCHAR* ret)
{
	for (int i = 0; i < strlen(src); ++i)
	{
		ret[i] = src[i];
	}
	ret[strlen(src)] = 0;
}
void getPathPhysicalDrive(int num, char* ret)
{
	sprintf(ret, "\\\\.\\PhysicalDrive%d", num);
}
void getPathVolume(char* vol, char* ret)
{
	sprintf(ret, "\\\\.\\%s:", vol);
}
//
bool readBootSector(char* Volume, mBootSector& mbs)
{
	char path[1024];
	//sprintf(path, "\\\\.\\%s:", Volume);
	getPathVolume(Volume, path);

	FILE* f = fopen(path, "rb+");
	if (f == NULL)
		return false;

	fread(&mbs, sizeof(mbs), 1, f);
	fclose(f);
	return true;
}

void writeBootSectorToLog(mBootSector mbs)
{
	//mBoot32 *mb = ;
	FILE* f = fopen("log.txt", "w");
	mBoot32 &mb = *((mBoot32*)&mbs);
	if (((mBoot32*)&mbs)->isFat32())
	{
		mBoot32 &boot = *((mBoot32*)&mbs);
		cout << "Fat32\n";
		cout << "Bytes per sector: " << boot.bytesPerSector << endl;
		cout << "Sectors per Cluster: " << boot.sectorsPerCluster << endl;
		cout << "Sectors on Boot: " << boot.sectorsOnBoot << endl;
		cout << "Number of fat table: " << (int) boot.nFat << endl;
		cout << "Fat size (in sector): " << boot.fatSize << endl;
		cout << "Volume size (in sector): " << boot.volumeSize << endl;
		cout << "Starting cluster of RDET: " << boot.rdetStartingCluster << endl;
		cout << "Backup boot sector: " << boot.backupSector << endl;

	}
	else
	if (((mBoot16*)&mbs)->isFat16())
	{
		mBoot16 &boot = *((mBoot16*)&mbs);
		cout << "Fat16\n";
		cout << "Bytes per sector: " << boot.bytesPerSector << endl;
		cout << "Sectors per Cluster: " << (int)boot.sectorsPerCluster << endl;
		cout << "Sectors on Boot: " << boot.sectorsOnBoot << endl;
		cout << "Number of fat table: " << (int) boot.nFat << endl;
		cout << "Fat size (in sector): " << boot.sectorPerFat << endl;
		cout << "Volume size (in sector): " << boot.volumeSize << endl;
		cout << "Number of entries in rdet: " << boot.nEntry << endl;
		//cout << "Backup boot sector: " << boot.backupSector << endl;
	}
	else
	if (((mBootNTFS*)&mbs)->isNTFS())
	{
		mBootNTFS &boot = *((mBootNTFS*)&mbs);
		cout << "NTFS\n";
		cout << "Bytes per sector: " << boot.bytesPerSector << endl;
		cout << "Sectors per Cluster: " << (int)boot.sectorsPerCluster << endl;
		cout << "Hidden sectors: " << boot.hiddenSectors << endl;
		cout << "Total sector " << boot.totalSector<< endl;
		
	}
	fclose(f);
}
