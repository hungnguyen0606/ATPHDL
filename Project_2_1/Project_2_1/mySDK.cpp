#include "stdafx.h"
#include "mySDK.h"

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

bool readMBR(int PhysicalDriveNum, mMBR& mbr)
{
	char path[1024];
	//sprintf(path, "\\\\.\\PhysicalDrive%d", PhysicalDriveNum);
	getPathPhysicalDrive(PhysicalDriveNum, path);

	FILE* f = fopen(path, "rb+");
	if (f == NULL)
		return false;

	fread(&mbr, sizeof(mbr), 1, f);
	fclose(f);
	return true;
}

void writeMBRtoLog(mMBR mbr)
{
	int nPartition = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (mbr.pTable.lEntry[i].isAvailable())
			++nPartition;
	}

	FILE* f = fopen("log.txt", "w");
	fprintf(f, "Total Primary Partition: %d\n", nPartition);
	fprintf(f, "----------------------------------------------\n");
	for (int i = 0, cc = 0; i < 4; ++i)
	{
		if (!mbr.pTable.lEntry[i].isAvailable())
			continue;
		
		mPARTITIONENTRY &temp = mbr.pTable.lEntry[i];

		fprintf(f, "Partition %d\n", ++cc);
		fprintf(f, "Active Partition: %s\n", ((temp.isActive) ? "Yes" : "No"));
		fprintf(f, "Starting head: %02X\n", temp.startHead);
		fprintf(f, "Starting sector: %02X\n", ((int)temp.startSectorCylinder[0] & 63));
		fprintf(f, "Starting cylinder: %02X\n", ((int)temp.startSectorCylinder[0] >> 6) + ((int)temp.startSectorCylinder[1]<<2));
		fprintf(f, "Partition type: %d\n", temp.fileSystem);
		fprintf(f, "Ending head: %02X\n", temp.endHead);
		fprintf(f, "Starting sector: %02X\n", ((int)temp.endSectorCylinder[0] & 63));
		fprintf(f, "Starting cylinder: %02X\n", ((int)temp.endSectorCylinder[0] >> 6) + ((int)temp.endSectorCylinder[1] << 2));
		fprintf(f, "Total sectors: %d\n", temp.nSecPerPartition);
		fprintf(f, "----------------------------------------------\n");
	}

	fprintf(f, "Hex value of MBR\n");
	for (int i = 0; i < sizeof(mbr); ++i)
	{
		fprintf(f, "%02X ", ((unsigned char*)&mbr)[i]);
		if ((i & 15) == 15)
			fprintf(f, "\n");
	}
	fclose(f);
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

	if (strncmp((char*)((mBoot32*)&mbs)->fatType, "FAT32", 5) == 0)
	{
		char temp[20];
		mBoot32 &mb = *((mBoot32*)&mbs);

		strncpy(temp, (char*)mb.OEM, 8);
		temp[8] = 0;
		fprintf(f, "OEM: %s\n", temp);

		fprintf(f, "Bytes per Sector: %d\n", mb.bytesPerSector);
		fprintf(f, "Sectors per Cluster: %d\n", mb.sectorsPerCluster);
		fprintf(f, "Sectors per Track: %d\n", mb.sectorsPerTrack);
		fprintf(f, "Number of head: %d\n", mb.nReader);
		fprintf(f, "Number of Fat: %d\n", mb.nFat);
		fprintf(f, "Fat size (sectors): %d\n", mb.fatSize);
		fprintf(f, "Volume size (sectors): %d\n", mb.volumeSize);
		fprintf(f, "Logical drive number: %02X\n", mb.physicFlag);
		fprintf(f, "Sector Number of the BackupBoot Sector: %04X\n", mb.backupSector);
		fprintf(f, "RDET's starting cluster: %08X\n", mb.rdetStartingCluster);

		//sprintf_s(temp, 11, "%s", mb.volumeLable);
		strncpy(temp, (char*)mb.volumeLable, 11);
		temp[11] = 0;
		fprintf(f, "Volume label: %s\n", temp);
		fprintf(f, "----------------------------------------------\n");
		fprintf(f, "Hex value of Boot Sector\n");
		for (int i = 0; i < sizeof(mb); ++i)
		{
			fprintf(f, "%02X ", ((unsigned char*)&mb)[i]);
			if ((i & 15) == 15)
				fprintf(f, "\n");
		}
	}
	else
		MessageBox(0, L"This is not FAT32", L"Error", MB_OK);
	fclose(f);
}
//
//bool CreateImage(const char* pathSrc, const char* pathDes, HWND progressbar)
//{
//	FILE* fin = fopen(pathSrc, "rb+");
//	FILE* fout = fopen(pathDes, "wb");
//	if (fin == NULL || fout == NULL)
//	{
//		return false;
//	}
//	
//	char buff[512 * 8];
//	int size = 512;
//	int n = 8;
//	int cc = 0;
//	while (cc = fread(buff, size, n, fin))
//	{
//		fwrite(buff, size, cc, fout);
//		SendMessage(progressbar, PBM_STEPIT, 0, 0);
//		//SendMessage(progressbar, PBM_SETMARQUEE, 1, 0);
//		UpdateWindow(progressbar);
//	}
//		
//	fclose(fout);
//	fclose(fin);
//	return true;
//}
bool CreateImage(const char* pathSrc, const char* pathDes, HWND progressbar)
{
	LARGE_INTEGER zero;
	zero.QuadPart = 0;
	//FILE* fin = fopen(pathSrc, "rb+");
	//FILE* fout = fopen(pathDes, "ab");
	TCHAR temp[1024];
	char buff[512*8];
	charToTchar(pathSrc, temp);
	HANDLE fin = CreateFile(temp, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	charToTchar(pathDes, temp);
	HANDLE fout = CreateFile(temp, GENERIC_WRITE, FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	
	if (fout == INVALID_HANDLE_VALUE)
	{
		fout = CreateFile(temp, GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
	}

	LARGE_INTEGER lastsector, bytesmove;
	lastsector.QuadPart = 0;
	charToTchar("loglog.log", temp);
	HANDLE flog = CreateFile(temp, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (flog == INVALID_HANDLE_VALUE)
	{
		flog = CreateFile(temp, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
	}
	else
	{

		DWORD bytesread = 0;
		SetFilePointerEx(flog, zero, 0, FILE_BEGIN);
		ReadFile(flog, &lastsector, sizeof(lastsector), &bytesread, 0);
		if (bytesread == 0)
			lastsector.QuadPart = 0;
	}

	bytesmove.QuadPart = lastsector.QuadPart * 512;
	bool suc1 = SetFilePointerEx(fin, bytesmove, 0, FILE_BEGIN);
	bool suc2 = SetFilePointerEx(fout, bytesmove, 0, FILE_BEGIN);

	DWORD bytesread;
	DWORD blocksize = 8 * 512;
	
	do
	{
		ReadFile(fin, buff, blocksize, &bytesread, 0);
		WriteFile(fout, buff, bytesread, 0, 0);

		lastsector.QuadPart += 8;
		SetFilePointerEx(flog, zero, 0, FILE_BEGIN);
		WriteFile(flog, &lastsector, sizeof(lastsector), 0, 0);
		SendMessage(progressbar, PBM_STEPIT, 0, 0);
		UpdateWindow(progressbar);
	} while (bytesread == blocksize);

	lastsector.QuadPart = 0;
	SetFilePointerEx(flog, zero, 0, FILE_BEGIN);
	WriteFile(flog, &lastsector, sizeof(lastsector), 0, 0);

	CloseHandle(fin);
	CloseHandle(fout);
	CloseHandle(flog);
	return true;
}
//
//bool CreateImage(const char* pathSrc, const char* pathDes, HWND progressbar)
//{
//	TCHAR temp[1024];
//	char ctemp[1024], buff[1024], outpath[1024];
//	charToTchar(pathSrc, temp);
//	HANDLE fin = CreateFile(temp, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
//	charToTchar(pathDes, temp);
//	HANDLE fout = CreateFile(temp, GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
//	HANDLE flog = CreateFile(L"CreateIMG.log", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
//
//	
//
//	unsigned long long lastsector = 0;
//	bool isSignal = false;
//	//check last log file
//	
//	LARGE_INTEGER offset;
//	if (flog == INVALID_HANDLE_VALUE)
//	{
//		flog = CreateFile(L"CreateIMG.log", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
//		LARGE_INTEGER sz;
//		sz.QuadPart = 0;
//		SetFilePointerEx(flog, sz, 0, FILE_BEGIN);
//		
//		//1: there's an process that has been interrupted. 0:normal 
//		isSignal = 1;
//		WriteFile(flog, &isSignal, 1, 0, 0);
//		//
//		int n = strlen(pathSrc) + 1;
//		WriteFile(flog, &n, sizeof(n), 0, 0);
//		WriteFile(flog, pathSrc, n, 0, 0);
//
//		n = strlen(pathDes) + 1;
//		WriteFile(flog, &n, sizeof(n), 0, 0);
//		WriteFile(flog, pathDes, n, 0, 0);
//
//		/*LARGE_INTEGER dis;
//		dis.QuadPart = 0;
//		SetFilePointerEx(flog, dis, &offset, FILE_CURRENT);*/
//	}
//	else
//	{
//		ReadFile(flog, &isSignal, 1, 0, 0);
//		if (isSignal)
//		{
//			int n;
//
//			//finput
//			ReadFile(flog, &n, sizeof(n), 0, 0);
//			ReadFile(flog, ctemp, n, 0, 0);
//			
//			charToTchar(ctemp, temp);
//			HANDLE fin = CreateFile(temp, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
//
//			//foutput
//			ReadFile(flog, &n, sizeof(n), 0, 0);
//			ReadFile(flog, ctemp, n, 0, 0);
//
//			charToTchar(ctemp, temp);
//			HANDLE fout = CreateFile(temp, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
//			//f
//		}
//		else
//		{
//			LARGE_INTEGER dis;
//			dis.QuadPart = 0;
//			SetFilePointerEx(flog, dis, NULL, FILE_BEGIN);
//
//			isSignal = 1;
//			WriteFile(flog, &isSignal, 1, 0, 0);
//			//
//			int n = strlen(pathSrc) + 1;
//			WriteFile(flog, &n, sizeof(n), 0, 0);
//			WriteFile(flog, pathSrc, n, 0, 0);
//
//			n = strlen(pathDes) + 1;
//			WriteFile(flog, &n, sizeof(n), 0, 0);
//			WriteFile(flog, pathDes, n, 0, 0);
//
//			
//			
//		}
//	}
//
//	//check if fies are opened normally
//	if ((fin == INVALID_HANDLE_VALUE || fout == INVALID_HANDLE_VALUE) && isSignal == false)
//	{
//		return false;
//	}
//	
//	LARGE_INTEGER dis;
//	dis.QuadPart = 0;
//	SetFilePointerEx(flog, dis, &offset, FILE_CURRENT);
//	
//	CloseHandle(flog);
//	CloseHandle(fout);
//	flog = INVALID_HANDLE_VALUE;
//	
//	
//	DWORD ssread = 512 * 8;
//	DWORD byteread = 0;
//	
//	do
//	{
//		byteread = 0;
//		ReadFile(fin, buff, ssread, &byteread, 0);
//
//		WriteFile(fout, buff, byteread, 0, 0);
//
//		//update last sector read
//		lastsector += 8;
//		
//		flog = CreateFile(L"CreateIMG.log",GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
//		SetFilePointerEx(flog, offset, 0, FILE_BEGIN);
//		WriteFile(flog, &lastsector, sizeof(lastsector), 0, 0);
//		CloseHandle(flog);
//
//
//	} while (byteread == ssread);
//	
//
//	CloseHandle(fin);
//	CloseHandle(fout);
//
//
//
//}