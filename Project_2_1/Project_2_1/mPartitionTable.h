#ifndef _M_PARTITION_TABLE
#define _M_PARTITION_TABLE

#pragma pack(push, 1)
struct mPARTITIONENTRY
{
	bool isActive;
	unsigned char startHead;
	unsigned char startSectorCylinder[2];
	unsigned char fileSystem;
	unsigned char endHead;
	unsigned char endSectorCylinder[2];
	unsigned int startLBA;
	unsigned int nSecPerPartition;
	//__int16 ending;
	mPARTITIONENTRY()
	{
		endHead = 0;
		startHead = 1;
		nSecPerPartition = 0;
	}

	bool isAvailable()
	{
		return nSecPerPartition != 0;
	}
};

struct mPARTITIONTABLE
{
	mPARTITIONENTRY lEntry[4];

	mPARTITIONTABLE()
	{
			
	}
};

#pragma pop(pop)

#endif