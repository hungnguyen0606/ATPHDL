#ifndef _MY_SDK_
#define _MY_SDK_

#include "BootSector.h"
bool readBootSector(char* Volume, mBootSector& mbs);
void writeBootSectorToLog(mBootSector mbs);


void getPathPhysicalDrive(int num, char* ret);
void getPathVolume(char* vol, char* ret);

#endif 