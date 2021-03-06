#ifndef UNICODE
#define UNICODE
#endif 

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include "resource.h"
#include "edk.h"
#include "EEGREAD.h"
#include <Windows.h>
#include <tchar.h>
#include <shlobj.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")


size_t Userclock;
WCHAR szPath[MAX_PATH];

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

WCHAR* timeFormat(size_t time)
{
  WCHAR secs[4];
  WCHAR mins[4];
  WCHAR hrs[4];
  WCHAR fin[24];
  WCHAR phold[8];
  wcscpy(phold, L"0");

  _itot(time % 60, secs, 10); // secs
  _itot((time / 60) % 60, mins, 10); // mins
  _itot(time / 3600, hrs, 10); // hrs

  if (wcslen(hrs) == 1) wcscat(phold, hrs);
  else wcscpy(phold, hrs);
  wcscpy(fin, phold); // 00
  wcscat(fin, L":");  // 00:
  wcscpy(phold, L"0");
  if (wcslen(mins) == 1) wcscat(phold, mins);
  else wcscpy(phold, mins);
  wcscat(fin, phold); // 00:00
  wcscat(fin, L":");  // 00:00:
  wcscpy(phold, L"0");
  if (wcslen(secs) == 1) wcscat(phold, secs);
  else wcscpy(phold, secs);
  wcscat(fin, phold); // 00:00:00

  return fin;
}


void onTimer(HWND hwnd, size_t& timer)
{
  timer++;
  WCHAR timerStr[20];
  wcscpy(timerStr, timeFormat(timer));
  SetDlgItemText(hwnd, 211, timerStr);
}

void startTime(HWND hwnd, size_t& timer)
{
    SetTimer(hwnd, 0x10, 1000, (TIMERPROC)NULL);
    InvalidateRect(hwnd, NULL, NULL);
}

void stopTime(HWND hwnd, size_t& timer)
{
  
    SetDlgItemText(hwnd, 211, L"00:00:00");
    timer = 0;
    KillTimer(hwnd, 0x10);
    InvalidateRect(hwnd, NULL, NULL);
}

void onSelectFilePath(HWND hwnd)
{
  BROWSEINFO rBI = { 0 };
  rBI.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
  rBI.lpszTitle = L"Select Folder for Output Files";

  LPITEMIDLIST pidl = SHBrowseForFolder(&rBI);
  if (pidl != NULL) 
  {
    BOOL fRet = SHGetPathFromIDList(pidl, szPath);
    SetDlgItemText(hwnd, 401, szPath);
    IMalloc* imalloc = 0;
    if (SUCCEEDED(SHGetMalloc(&imalloc)))
    {
      imalloc->Free(pidl);
      imalloc->Release();
    }
  }
}

void greyAll(HWND hwnd)
{
  EnableWindow(GetDlgItem(hwnd, 500), false);
  EnableWindow(GetDlgItem(hwnd, 400), false);
  EnableWindow(GetDlgItem(hwnd, 401), false);
  EnableWindow(GetDlgItem(hwnd, 403), false);
  EnableWindow(GetDlgItem(hwnd, 402), false);
  EnableWindow(GetDlgItem(hwnd, 510), false);
  EnableWindow(GetDlgItem(hwnd, 3), false);
  EnableWindow(GetDlgItem(hwnd, 4), false);
  EnableWindow(GetDlgItem(hwnd, 5), false);
  EnableWindow(GetDlgItem(hwnd, 6), false);
  EnableWindow(GetDlgItem(hwnd, 7), false);
  EnableWindow(GetDlgItem(hwnd, 8), false);
  EnableWindow(GetDlgItem(hwnd, 9), false);
  EnableWindow(GetDlgItem(hwnd, 10), false);
  EnableWindow(GetDlgItem(hwnd, 11), false);
  EnableWindow(GetDlgItem(hwnd, 12), false);
  EnableWindow(GetDlgItem(hwnd, 13), false);
  EnableWindow(GetDlgItem(hwnd, 14), false);
  EnableWindow(GetDlgItem(hwnd, 15), false);
  EnableWindow(GetDlgItem(hwnd, 16), false);
  EnableWindow(GetDlgItem(hwnd, 0), false);
  EnableWindow(GetDlgItem(hwnd, 1), false);
}

void unGreyAll(HWND hwnd)
{
  EnableWindow(GetDlgItem(hwnd, 500), true);
  EnableWindow(GetDlgItem(hwnd, 400), true);
  EnableWindow(GetDlgItem(hwnd, 403), true);
  EnableWindow(GetDlgItem(hwnd, 401), true);
  EnableWindow(GetDlgItem(hwnd, 510), true);
  EnableWindow(GetDlgItem(hwnd, 3), true);
  EnableWindow(GetDlgItem(hwnd, 4), true);
  EnableWindow(GetDlgItem(hwnd, 5), true);
  EnableWindow(GetDlgItem(hwnd, 6), true);
  EnableWindow(GetDlgItem(hwnd, 7), true);
  EnableWindow(GetDlgItem(hwnd, 8), true);
  EnableWindow(GetDlgItem(hwnd, 9), true);
  EnableWindow(GetDlgItem(hwnd, 10), true);
  EnableWindow(GetDlgItem(hwnd, 11), true);
  EnableWindow(GetDlgItem(hwnd, 12), true);
  EnableWindow(GetDlgItem(hwnd, 13), true);
  EnableWindow(GetDlgItem(hwnd, 14), true);
  EnableWindow(GetDlgItem(hwnd, 15), true);
  EnableWindow(GetDlgItem(hwnd, 16), true);
  EnableWindow(GetDlgItem(hwnd, 0), true);
  if(IsDlgButtonChecked(hwnd, 1)) EnableWindow(GetDlgItem(hwnd, 402), true);
  EnableWindow(GetDlgItem(hwnd, 1), true);
}

void selectButton(HWND hwnd)
{
  HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | 
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog *pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, 
                IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem *pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {
                        SetDlgItemText(hwnd, 401, pszFilePath);
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
}

void createChannelButtons(HWND hwnd)
{
	CreateWindow(TEXT("static"), TEXT("Channels: "), WS_VISIBLE | WS_CHILD, 50, 30, 80, 25, hwnd, (HMENU)200, NULL, NULL);

	CreateWindow(TEXT("Button"), TEXT("All"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 50, 80, 25, hwnd, (HMENU)0, NULL, NULL);

	CreateWindow(TEXT("Button"), TEXT("AF3"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 70, 80, 25, hwnd, (HMENU)3, NULL, NULL);

	CreateWindow(TEXT("Button"), TEXT("F7"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 90, 80, 25, hwnd, (HMENU)4, NULL, NULL);

	CreateWindow(TEXT("Button"), TEXT("F3"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 110, 80, 25, hwnd, (HMENU)5, NULL, NULL);

	CreateWindow(TEXT("Button"), TEXT("FC5"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 130, 80, 25, hwnd, (HMENU)6, NULL, NULL);

	CreateWindow(TEXT("Button"), TEXT("T7"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 150, 80, 25, hwnd, (HMENU)7, NULL, NULL);

	CreateWindow(TEXT("Button"), TEXT("P7"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 170, 80, 25, hwnd, (HMENU)8, NULL, NULL);

	CreateWindow(TEXT("Button"), TEXT("O1"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 190, 80, 25, hwnd, (HMENU)9, NULL, NULL);

	CreateWindow(TEXT("Button"), TEXT("O2"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 210, 80, 25, hwnd, (HMENU)10, NULL, NULL);

	CreateWindow(TEXT("Button"), TEXT("P8"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 230, 80, 25, hwnd, (HMENU)11, NULL, NULL);

	CreateWindow(TEXT("Button"), TEXT("T8"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 250, 80, 25, hwnd, (HMENU)12, NULL, NULL);

	CreateWindow(TEXT("Button"), TEXT("FC6"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 270, 80, 25, hwnd, (HMENU)13, NULL, NULL);

	CreateWindow(TEXT("Button"), TEXT("F4"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 290, 80, 25, hwnd, (HMENU)14, NULL, NULL);

	CreateWindow(TEXT("Button"), TEXT("F8"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 310, 80, 25, hwnd, (HMENU)15, NULL, NULL);

	CreateWindow(TEXT("Button"), TEXT("AF4"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 330, 80, 25, hwnd, (HMENU)16, NULL, NULL);
}

void isChannelChecked(int boxID, HWND hwnd, headset& h)
{
	bool checked = IsDlgButtonChecked(hwnd, boxID);
	if (checked)
	{
		CheckDlgButton(hwnd, boxID, BST_UNCHECKED);
		switch (boxID)
		{
			case 1:
				EnableWindow(GetDlgItem(hwnd, 402), false);
				break;
			case 0:
				for (int i = 3; i <= 16; i++)
				{
					CheckDlgButton(hwnd, i, BST_UNCHECKED);
          remove_channel(h, i);
				}
			break;
			default:
				remove_channel(h, boxID);
				break;
		}
	}
	else
	{
		CheckDlgButton(hwnd, boxID, BST_CHECKED);
		switch (boxID)
		{
			case 1:
				EnableWindow(GetDlgItem(hwnd, 402), true);
			break;
			case 0:
				for (int i = 3; i <= 16; i++)
				{
					CheckDlgButton(hwnd, i, BST_CHECKED);
          add_channel(h, i);
				}
			break;
			default:
				add_channel(h, boxID);
				break;
		}
	}
}

void createHeadsetStatus(HWND hwnd)
{
	CreateWindow(TEXT("static"), TEXT("Headset Status: "), WS_VISIBLE | WS_CHILD, 165, 30, 115, 25, hwnd, (HMENU)201, NULL, NULL);
	CreateWindow(TEXT("static"), TEXT("Stopped"), WS_VISIBLE | WS_CHILD, 280, 30, 115, 25, hwnd, (HMENU)202, NULL, NULL);
}

void createFileOutput(HWND hwnd)
{
	CreateWindow(TEXT("static"), TEXT("Text File Name:"), WS_VISIBLE | WS_CHILD, 135, 70, 105, 25, hwnd, (HMENU)203, NULL, NULL);
  CreateWindow(TEXT("static"), TEXT("FFT File Name:"), WS_VISIBLE | WS_CHILD, 135, 90, 105, 25, hwnd, (HMENU)203, NULL, NULL);
	CreateWindow(TEXT("static"), TEXT("File Path:"), WS_VISIBLE | WS_CHILD, 165, 110, 75, 25, hwnd, (HMENU)203, NULL, NULL);
	CreateWindow(TEXT("Button"), TEXT("Select"), WS_VISIBLE | WS_CHILD, 390, 110, 80, 21, hwnd, (HMENU)510, NULL, NULL);
  CreateWindow(TEXT("static"), TEXT(".csv"), WS_VISIBLE | WS_CHILD, 390, 70, 40, 25, hwnd, (HMENU)209, NULL, NULL);
  CreateWindow(TEXT("static"), TEXT(".csv"), WS_VISIBLE | WS_CHILD, 390, 90, 40, 25, hwnd, (HMENU)203, NULL, NULL);
}

void createTextBoxes(HINSTANCE hInstance, HWND hwnd)
{
	HWND txtfileName = CreateWindow(L"EDIT", L"RawData", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL | ES_WANTRETURN, 240, 70, 140, 20, hwnd, (HMENU)400, hInstance, 0);
  HWND csvfileName = CreateWindow(L"EDIT", L"FFTData", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL | ES_WANTRETURN, 240, 90, 140, 20, hwnd, (HMENU)403, hInstance, 0);
	HWND filePath = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL | ES_WANTRETURN, 240, 110, 140, 20, hwnd, (HMENU)401, hInstance, 0);
	HWND timeBox = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL | ES_WANTRETURN, 330, 142, 60, 20, hwnd, (HMENU)402, hInstance, 0);
	EnableWindow(GetDlgItem(hwnd, 402), false);
}

void createRecordStopButton(HWND hwnd)
{
	CreateWindow(TEXT("Button"), TEXT("RECORD"), WS_VISIBLE | WS_CHILD, 165, 140, 80, 25, hwnd, (HMENU)500, NULL, NULL);
    //EnableWindow(GetDlgItem(hwnd, 500), false);
	CreateWindow(TEXT("Button"), TEXT("Timed"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 255, 140, 65, 25, hwnd, (HMENU)1, NULL, NULL);
	CreateWindow(TEXT("static"), TEXT("Seconds"), WS_VISIBLE | WS_CHILD, 400, 143, 80, 25, hwnd, (HMENU)200, NULL, NULL);
	CreateWindow(TEXT("Button"), TEXT("STOP"), WS_VISIBLE | WS_CHILD, 165, 240, 80, 25, hwnd, (HMENU)501, NULL, NULL);
}

void createRunTime(HWND hwnd)
{
	CreateWindow(TEXT("static"), TEXT("Recording Time: "), WS_VISIBLE | WS_CHILD, 165, 190, 115, 25, hwnd, (HMENU)210, NULL, NULL);
	CreateWindow(TEXT("static"), TEXT("00:00:00"), WS_VISIBLE | WS_CHILD, 285, 190, 75, 25, hwnd, (HMENU)211, NULL, NULL);
	CreateWindow(TEXT("static"), TEXT("/"), WS_VISIBLE | WS_CHILD, 365, 190, 10, 25, hwnd, (HMENU)212, NULL, NULL);
	CreateWindow(TEXT("static"), TEXT("00:00:00"), WS_VISIBLE | WS_CHILD, 380, 190, 75, 25, hwnd, (HMENU)213, NULL, NULL);
}

void createAll(HWND hwnd)
{
  createChannelButtons(hwnd);
		createHeadsetStatus(hwnd);
		createFileOutput(hwnd);
		createRecordStopButton(hwnd);
		createRunTime(hwnd);
}

void onPaint(HWND hwnd, HDC hdc)
{
		Graphics graphics(hdc);
		//graphics.SetSmoothingMode(SmoothingModeHighQuality);
		SolidBrush brush(Color::Green);
		graphics.FillEllipse(&brush, 100, 100, 100, 100);
    MessageBox(hwnd, L"text", L"text", MB_OK);

}

LRESULT onCtlColorStatic(HDC hdc, HWND hwnd)
{
  SetBkColor(hdc, 0xFFFFFF);
  return (LRESULT)GetStockObject(WHITE_BRUSH);
}


inline PRPACK GetAppState(HWND hwnd)
{
    LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
    PRPACK package = reinterpret_cast<PRPACK>(ptr);
    return package;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
  PRPACK package = new PACK(false, new headset(), NULL, new raw_buffer_queue());
  
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.lpszClassName = CLASS_NAME;

	HMENU hmenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));

	RegisterClass(&wc);

	// Create the window.

	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"EmotivUserApplication",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 440,

		NULL,       // Parent window    
		hmenu,       // Menu
		hInstance,  // Instance handle
		package      // Additional application data
		);

	createTextBoxes(hInstance, hwnd);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  wchar_t fileName[100], filePath[100];
  WCHAR userTime[20];
  CREATESTRUCT *pCreate;
  bool stop = false;
  DWORD ThreadID;
  PRPACK package;
  int userIntTime;
  PAINTSTRUCT ps;
  HDC hdc;
  LRESULT result;

  if(uMsg != WM_CREATE)
  {
    package = GetAppState(hwnd);
  }

	switch (uMsg)
	{
	  case WM_DESTROY:
		  PostQuitMessage(0);
		  return 0;

    case WM_PAINT:
		  hdc = BeginPaint(hwnd, &ps);
		  onPaint(hwnd, hdc);
		  EndPaint(hwnd, &ps);
      return 0;

	  case WM_CREATE:
		  createAll(hwnd);
      pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
      package = reinterpret_cast<PRPACK>(pCreate->lpCreateParams);
      package->hwnd = hwnd;
      SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)package);
		  break;

	  case WM_COMMAND:
		  if(LOWORD(wParam) >-1 && LOWORD(wParam) <17) isChannelChecked(LOWORD(wParam), hwnd, *(package->head));
		  switch (LOWORD(wParam))
		  {
			  case ID_FILE_CLOSE40001:
					  PostQuitMessage(0);
				  break;
			  case ID_HELP_ABOUT:
				  MessageBox(NULL, L"EmotivUserApplication\n Version 1\n EEGalabECU", L"About", MB_OK);
				  break;
        case 500:
          SetDlgItemText(hwnd, 202, L"Recording");
          GetDlgItemText(hwnd, 402, userTime, 100);
          wcscpy(userTime, timeFormat(Userclock));
          SetDlgItemText(hwnd, 213, userTime);

          package->timer = 0;
          startTime(package->hwnd, package->timer);
          greyAll(hwnd);
          package->stop = false;
          CreateThread(NULL, 0, processRawData, package, 0, &ThreadID);
          break;

        case 501:
          package->stop = true;
          SetDlgItemText(hwnd, 202, L"Stopped");
          SetDlgItemText(hwnd, 211, L"00:00:00");
          SetDlgItemText(hwnd, 212, L"/");
          SetDlgItemText(hwnd, 213, L"00:00:00");
          unGreyAll(hwnd);
          stopTime(package->hwnd, package->timer);
          break;

        case 510:
          onSelectFilePath(hwnd);
          break;
        case 400: case 401:
        GetWindowText(GetDlgItem(hwnd, 400), fileName, 100);
        GetWindowText(GetDlgItem(hwnd, 401), filePath, 100); 
        break;
		  }
		  break;

    case WM_TIMER:
    
      switch (wParam)
      {
        case 0x10:
          onTimer(package->hwnd, package->timer);
          return 0;
      }
      break;

	  case WM_CTLCOLORSTATIC:
        if(result = onCtlColorStatic((HDC)wParam, (HWND)lParam)) return result;
        break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}