#ifndef _BOOT_SECTOR_
#define _BOOT_SECTOR_
#include <string>
#pragma pack(push, 1)

struct mBootSector
{
protected:
	unsigned char data[512];
};

struct mBoot32
{
	unsigned char jumpCode[3];
	unsigned char OEM[8];
	unsigned __int16 bytesPerSector;
	unsigned char sectorsPerCluster;
	unsigned __int16 sectorsOnBoot;
	unsigned char nFat;
	unsigned char dump0[4];
	unsigned char deviceType;
	unsigned char dump1[2];
	unsigned __int16 sectorsPerTrack;
	unsigned __int16 nReader;
	unsigned __int32 offset;
	unsigned __int32 volumeSize;
	unsigned __int32 fatSize;
	unsigned __int16 flag;
	unsigned __int16 fatVersion;
	unsigned __int32 rdetStartingCluster;
	unsigned __int16 subInfo;
	unsigned __int16 backupSector;
	unsigned char dump2[12];
	unsigned char physicFlag;
	unsigned char dump3;
	unsigned char os;
	unsigned __int32 serialNumber;
	unsigned char volumeLable[11];
	unsigned char fatType[8];
	unsigned char code[420];
	unsigned char ending[2];

	bool isFat32()
	{
		std::string ver(fatType, fatType + 5);
		return ver == "FAT32";
	}
};

struct mBoot16
{
	unsigned char jumpCode[3];
	unsigned char OEM[8];
	unsigned __int16 bytesPerSector;
	unsigned char sectorsPerCluster;
	unsigned __int16 sectorsOnBoot;
	unsigned char nFat;
	unsigned __int16 nEntry;
	unsigned __int16 nSectorVol;
	unsigned char volType;
	unsigned __int16 sectorPerFat;
	unsigned __int16 sectorsPerTrack;
	unsigned __int16 nReader;
	unsigned __int32 offset;
	unsigned __int32 volumeSize;
	unsigned char physicalSign;
	unsigned char dump0;
	unsigned char os;
	unsigned __int32 serialNumber;
	unsigned char volumeLabel[11];
	unsigned char fatVer[8];
	unsigned char bootcode[448];
	unsigned char ending[2];

	bool isFat16()
	{
		std::string ver(fatVer, fatVer + 5);
		return ver == "FAT16";
	}
	bool isFat12()
	{
		std::string ver(fatVer, fatVer + 5);
		return ver == "FAT12";
	}
};

struct mBootNTFS
{
	unsigned char jumpCode[3];
	unsigned char OEMID[8];

	unsigned __int16 bytesPerSector;
	unsigned char sectorsPerCluster;
	unsigned __int16 reversedSectors;
	unsigned char dump0[5];
	unsigned char mediaDes[1];
	unsigned __int16 dump1;
	unsigned __int16 sectorsPerTrack;
	unsigned __int16 readers;
	unsigned __int32 hiddenSectors;
	unsigned char dump2[8];
	unsigned __int64 totalSector;
	unsigned char dump3[24];
	unsigned __int64 serialNumberVol;
	unsigned __int32 checkSum;

	unsigned char bootstrapCode[426];
	unsigned __int16 ending;

	bool isNTFS()
	{
		std::string ver(OEMID, OEMID + 4);
		return ver == "NTFS";
	}
};
#pragma pack(pop)
#endif