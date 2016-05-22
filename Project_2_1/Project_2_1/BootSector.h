#ifndef _BOOT_SECTOR_
#define _BOOT_SECTOR_

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
};

struct mBoot16
{

};
#pragma pack(pop)
#endif