// Project_2_1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Project_2_1.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Dialog1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void getVolumeName(HWND hDlg, char* ret);
void getPhysicalNumber(HWND hDlg, int& ret);
void makeImage(LPVOID para);
void saveFile(HWND hDlg, char* ret);
void showLog();

struct srcdest
{
	std::string src, dest;
	HWND hDlg;
};

HWND waitingbox;
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PROJECT_2_1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT_2_1));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT_2_1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PROJECT_2_1);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   //ShowWindow(hWnd, nCmdShow);
   //UpdateWindow(hWnd);

   return TRUE;
}

unsigned long long file_size(FILE* fin)
{
	unsigned long long size = 0;
	char buff[8 * 512];
	int tt = 8 * 512;
	int cc;
	while (cc = fread(buff, 512, 8, fin))
	{
		size += cc;
	}
		

	return size;

}
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	
	
	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:
	{
		waitingbox = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG_WAITING), hWnd, About);
		ShowWindow(waitingbox, SW_HIDE);
		UpdateWindow(waitingbox);

		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dialog1);
		break;

	}
		
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Dialog1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			PostQuitMessage(0);
			return (INT_PTR)TRUE;
		}
		switch (LOWORD(wParam))
		{
		case IDC_VOLUME_INFO:
		{
			mBootSector mbs;
			//HWND val = GetDlgItem(hDlg, IDC_EDIT_VOLUME);
			//TCHAR temp[1024];
			//char ctemp[1024];
			/*GetDlgItemText(hDlg, IDC_EDIT_VOLUME, temp, 1024);

			for (int i = 0; i < lstrlen(temp); ++i)
				ctemp[i] = temp[i];
			ctemp[lstrlen(temp)] = 0;*/
			char vol[1024];
			getVolumeName(hDlg, vol);
			if (readBootSector(vol, mbs))
			{
				writeBootSectorToLog(mbs);
				CreateThread(NULL, 1024, (LPTHREAD_START_ROUTINE)showLog, 0, 0, NULL);
			}
			
			else MessageBox(hDlg, L"Cannot open volume", L"Error", MB_OK);
			//system("notepad log.txt");
			break;
		}
		case IDC_DRIVE_INFO:
		{
			mMBR mbr;
			//HWND val = GetDlgItem(hDlg, IDC_EDIT_PHYSICAL_DRIVE);
			int driveNumber;// = GetDlgItemInt(hDlg, IDC_EDIT_PHYSICAL_DRIVE, 0, false);//get
			getPhysicalNumber(hDlg, driveNumber);

			if (readMBR(driveNumber, mbr))
			{
				writeMBRtoLog(mbr);
				CreateThread(NULL, 1024, (LPTHREAD_START_ROUTINE)showLog, 0, 0, NULL);
			}
			else
				MessageBox(hDlg, L"Cannot open drive", L"Error", MB_OK);
			//system("notepad log.txt");

			break;
		}

		case IDC_IMAGE_PHYDRIVE:
		{
			int num;
			char path[1024], dest[1024];
			getPhysicalNumber(hDlg, num);
			getPathPhysicalDrive(num, path);
			srcdest* temp = new srcdest;
			temp->src = std::string(path);
			saveFile(hDlg, dest);
			temp->dest = std::string(dest);

			CreateThread(NULL, 1024, (LPTHREAD_START_ROUTINE)makeImage, temp, 0, NULL);
			//CreateImage(path, "temp.img", NULL);
			break;
		}
		case IDC_IMAGE_VOLUME:
		{
			char vol[1024];
			char path[1024], dest[1024];
			getVolumeName(hDlg, vol);
			getPathVolume(vol, path);
			srcdest* temp = new srcdest;
			temp->src = std::string(path);
			saveFile(hDlg, dest);
			temp->dest = std::string(dest);

			CreateThread(NULL, 1024, (LPTHREAD_START_ROUTINE)makeImage, temp, 0, NULL);

			//CreateImage(path, "temp.img", NULL);
			break;
		}
		default: 
			return (INT_PTR)TRUE;
		}
		break;
	}

	return (INT_PTR)FALSE;
}

void getVolumeName(HWND hDlg, char* ret)
{
	TCHAR temp[1024];
	char ctemp[1024];
	GetDlgItemText(hDlg, IDC_EDIT_VOLUME, temp, 1024);

	for (int i = 0; i < lstrlen(temp); ++i)
		ret[i] = temp[i];
	ret[lstrlen(temp)] = 0;
}

void getPhysicalNumber(HWND hDlg, int& ret)
{
	ret = GetDlgItemInt(hDlg, IDC_EDIT_PHYSICAL_DRIVE, 0, false);
}
void makeImage(LPVOID para)
{
	srcdest* temp = ((srcdest*)para);
	ShowWindow(waitingbox, SW_SHOW);
	
	HWND progressbar = GetDlgItem(waitingbox, IDC_PROGRESS1);

	SendMessage(progressbar, PBM_SETMARQUEE, 1, 0);
	UpdateWindow(progressbar);
	CreateImage(temp->src.c_str(), temp->dest.c_str(), progressbar);
	SendMessage(progressbar, PBM_SETMARQUEE, 0, 0);

	ShowWindow(waitingbox, SW_HIDE);
	delete temp;
}
void showLog()
{
	WinExec("notepad log.txt", SW_SHOW);
	//system("notepad log.txt");
	//exithrea
}

void saveFile(HWND hDlg, char* ret)
{
	TCHAR szFilter[] = TEXT("BMP file\0*.BMP\0");
	TCHAR szFile[1024];
	OPENFILENAME ofn; 

	szFile[0] = '\0';
	
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hDlg; 
	ofn.lpstrFilter = szFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = szFile; 
	ofn.nMaxFile = sizeof(szFile);
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	BOOL a;
	a = GetSaveFileName(&ofn);
	if (a)
	{
		for (int i = 0; i < lstrlen(szFile); ++i)
			ret[i] = szFile[i];
		ret[lstrlen(szFile)] = 0;
	}
}