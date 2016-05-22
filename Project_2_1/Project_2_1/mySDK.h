#ifndef _MY_SDK_
#define _MY_SDK_

bool readMBR(int PhysicalDriveNum, mMBR& mbr);
void writeMBRtoLog(mMBR mbr);
bool readBootSector(char* Volume, mBootSector& mbs);
void writeBootSectorToLog(mBootSector mbs);
bool CreateImage(const char* pathSrc, const char* pathDes, HWND progressbar);

void getPathPhysicalDrive(int num, char* ret);
void getPathVolume(char* vol, char* ret);

#endif 