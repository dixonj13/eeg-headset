#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include "resource.h"
#include "channelMap.h"
#include "FFTs.h"
#include "headset.h"
#include "rawBuffer.h"
#include "rawBufferQueue.h"
#include "timeUtil.h"
#include "edk.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void createChannelButtons(HWND hwnd)
{
	CreateWindow(TEXT("static"), TEXT("Channels: "), WS_VISIBLE | WS_CHILD, 50, 30, 80, 25, hwnd, (HMENU)200, NULL, NULL);

	CreateWindow(TEXT("Button"), TEXT("All"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 50, 80, 25, hwnd, (HMENU)100, NULL, NULL);
	CheckDlgButton(hwnd, 100, BST_CHECKED);

	CreateWindow(TEXT("Button"), TEXT("AF3"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 70, 80, 25, hwnd, (HMENU)3, NULL, NULL);
	CheckDlgButton(hwnd, 3, BST_CHECKED);

	CreateWindow(TEXT("Button"), TEXT("F7"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 90, 80, 25, hwnd, (HMENU)4, NULL, NULL);
	CheckDlgButton(hwnd, 4, BST_CHECKED);

	CreateWindow(TEXT("Button"), TEXT("F3"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 110, 80, 25, hwnd, (HMENU)5, NULL, NULL);
	CheckDlgButton(hwnd, 5, BST_CHECKED);

	CreateWindow(TEXT("Button"), TEXT("FC5"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 130, 80, 25, hwnd, (HMENU)6, NULL, NULL);
	CheckDlgButton(hwnd, 6, BST_CHECKED);

	CreateWindow(TEXT("Button"), TEXT("T7"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 150, 80, 25, hwnd, (HMENU)7, NULL, NULL);
	CheckDlgButton(hwnd, 7, BST_CHECKED);

	CreateWindow(TEXT("Button"), TEXT("P7"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 170, 80, 25, hwnd, (HMENU)8, NULL, NULL);
	CheckDlgButton(hwnd, 8, BST_CHECKED);

	CreateWindow(TEXT("Button"), TEXT("O1"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 190, 80, 25, hwnd, (HMENU)9, NULL, NULL);
	CheckDlgButton(hwnd, 9, BST_CHECKED);

	CreateWindow(TEXT("Button"), TEXT("O2"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 210, 80, 25, hwnd, (HMENU)10, NULL, NULL);
	CheckDlgButton(hwnd, 10, BST_CHECKED);

	CreateWindow(TEXT("Button"), TEXT("P8"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 230, 80, 25, hwnd, (HMENU)11, NULL, NULL);
	CheckDlgButton(hwnd, 11, BST_CHECKED);

	CreateWindow(TEXT("Button"), TEXT("T8"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 250, 80, 25, hwnd, (HMENU)12, NULL, NULL);
	CheckDlgButton(hwnd, 12, BST_CHECKED);

	CreateWindow(TEXT("Button"), TEXT("FC6"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 270, 80, 25, hwnd, (HMENU)13, NULL, NULL);
	CheckDlgButton(hwnd, 13, BST_CHECKED);

	CreateWindow(TEXT("Button"), TEXT("F4"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 290, 80, 25, hwnd, (HMENU)14, NULL, NULL);
	CheckDlgButton(hwnd, 14, BST_CHECKED);

	CreateWindow(TEXT("Button"), TEXT("F8"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 310, 80, 25, hwnd, (HMENU)15, NULL, NULL);
	CheckDlgButton(hwnd, 15, BST_CHECKED);

	CreateWindow(TEXT("Button"), TEXT("AF4"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 330, 80, 25, hwnd, (HMENU)16, NULL, NULL);
	CheckDlgButton(hwnd, 16, BST_CHECKED);
}

void isChannelChecked(int boxID, HWND hwnd)
{
	bool checked = IsDlgButtonChecked(hwnd, boxID);
	if (checked)
	{
		CheckDlgButton(hwnd, boxID, BST_UNCHECKED);
		//Remove Channel
		switch (boxID)
		{
			case 115:
				EnableWindow(GetDlgItem(hwnd, 402), false);
				break;
			case 100:
				for (int i = 3; i <= 16; i++)
				{
					CheckDlgButton(hwnd, i, BST_UNCHECKED);
				}
			break;
			default:
				//channelAdd(h, boxID);
				break;
		}
	}
	else
	{
		CheckDlgButton(hwnd, boxID, BST_CHECKED);
		switch (boxID)
		{
			case 115:
				EnableWindow(GetDlgItem(hwnd, 402), true);
			break;
			case 100:
				for (int i = 3; i <= 16; i++)
				{
					CheckDlgButton(hwnd, i, BST_CHECKED);
				}
			break;
			default:
				//channelRemove(h, boxID);
				break;
		}
	}
}

void createHeadsetStatus(HWND hwnd)
{
	CreateWindow(TEXT("static"), TEXT("Headset Status: "), WS_VISIBLE | WS_CHILD, 165, 30, 115, 25, hwnd, (HMENU)201, NULL, NULL);
	CreateWindow(TEXT("static"), TEXT("STATUS......"), WS_VISIBLE | WS_CHILD, 280, 30, 115, 25, hwnd, (HMENU)202, NULL, NULL);
}

void createFileOutput(HWND hwnd)
{
	CreateWindow(TEXT("static"), TEXT("File Name:"), WS_VISIBLE | WS_CHILD, 165, 70, 75, 25, hwnd, (HMENU)203, NULL, NULL);
	CreateWindow(TEXT("static"), TEXT("File Path:"), WS_VISIBLE | WS_CHILD, 165, 90, 75, 25, hwnd, (HMENU)203, NULL, NULL);
	CreateWindow(TEXT("Button"), TEXT("Select"), WS_VISIBLE | WS_CHILD, 390, 90, 80, 21, hwnd, (HMENU)510, NULL, NULL);
}

void createTextBoxes(HINSTANCE hInstance, HWND hwnd)
{
	HWND fileName = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL | ES_WANTRETURN, 240, 70, 140, 20, hwnd, (HMENU)400, hInstance, 0);
	HWND filePath = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL | ES_WANTRETURN, 240, 90, 140, 20, hwnd, (HMENU)401, hInstance, 0);
	HWND timeBox = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL | ES_WANTRETURN, 330, 142, 60, 20, hwnd, (HMENU)402, hInstance, 0);
	EnableWindow(GetDlgItem(hwnd, 402), false);
}

void createRecordStopButton(HWND hwnd)
{
	CreateWindow(TEXT("Button"), TEXT("RECORD"), WS_VISIBLE | WS_CHILD, 165, 140, 80, 25, hwnd, (HMENU)500, NULL, NULL);
	CreateWindow(TEXT("Button"), TEXT("Timed"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 255, 140, 65, 25, hwnd, (HMENU)115, NULL, NULL);
	CreateWindow(TEXT("Button"), TEXT("STOP"), WS_VISIBLE | WS_CHILD, 165, 240, 80, 25, hwnd, (HMENU)501, NULL, NULL);
}

void createRunTime(HWND hwnd)
{
	CreateWindow(TEXT("static"), TEXT("Recording Time: "), WS_VISIBLE | WS_CHILD, 165, 190, 115, 25, hwnd, (HMENU)210, NULL, NULL);
	CreateWindow(TEXT("static"), TEXT("TIME"), WS_VISIBLE | WS_CHILD, 285, 190, 60, 25, hwnd, (HMENU)211, NULL, NULL);
	CreateWindow(TEXT("static"), TEXT("/"), WS_VISIBLE | WS_CHILD, 335, 190, 10, 25, hwnd, (HMENU)212, NULL, NULL);
	CreateWindow(TEXT("static"), TEXT("TIME"), WS_VISIBLE | WS_CHILD, 345, 190, 60, 25, hwnd, (HMENU)213, NULL, NULL);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
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
		NULL       // Additional application data
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
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CREATE:
		createChannelButtons(hwnd);
		createHeadsetStatus(hwnd);
		createFileOutput(hwnd);
		createRecordStopButton(hwnd);
		createRunTime(hwnd);
		break;

	case WM_COMMAND:
		isChannelChecked(LOWORD(wParam), hwnd);
		switch (LOWORD(wParam))
		{
			case ID_FILE_CLOSE40001:
					PostQuitMessage(0);
				break;
			case ID_HELP_ABOUT:
				MessageBox(NULL, L"EmotivUserApplication\n Version 1\n EEGalabECU", L"About", MB_OK);
				break;
		}
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_BTNFACE + 1));

		EndPaint(hwnd, &ps);
	}
		return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}