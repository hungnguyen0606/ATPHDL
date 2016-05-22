#ifndef _MY_SDK_H_
#define _MY_SDK_H_
#include "stdafx.h"

void readSector(HANDLE vol, char* data, LARGE_INTEGER sectorID, DWORD sectorSize);
void readCluster(HANDLE vol, char* data, unsigned int clusterID, DWORD clusterSize, LARGE_INTEGER offset);
void splitPath(TCHAR *path, std::vector<std::wstring>& pathList);
void CopyContent(TCHAR *path, TCHAR* dest);
#endif