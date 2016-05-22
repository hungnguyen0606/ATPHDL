#ifndef _VOLUME_INFO_H_
#define _VOLUME_INFO_H_
class VolumeInfo
{
public:
	bool isFat32;
	int fatEntrySize;
	unsigned __int16 bytesPerSector;
	unsigned char sectorsPerCluster;
	LARGE_INTEGER offsetFatTable;
	LARGE_INTEGER offsetDataArea;

	//only available for fat 16
	LARGE_INTEGER offsetRdet; 
	int nEntryRdet;
	//

public:
	void setFat32(bool is32) { isFat32 = is32; fatEntrySize = (is32) ? 4 : 2; }
	VolumeInfo();
	~VolumeInfo();
};

#endif