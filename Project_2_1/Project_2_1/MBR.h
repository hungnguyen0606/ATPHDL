#ifndef _MBR_
#define _MBR_
#include "mPartitionTable.h"

#pragma pack(push, 1)
struct mMBR
{
	unsigned char BootstrapCodeArea[446];
	mPARTITIONTABLE pTable;
	unsigned char BootSignature[2];
};
#pragma pack(pop)
#endif