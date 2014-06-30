#define _CRT_SECURE_NO_WARNINGS

#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include <tchar.h>
#include <shlobj.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#pragma region GLOBAL_VAR

HWND txthwnd = 0;
WCHAR szPath[MAX_PATH];
bool started = false;
size_t timer = 0;

#pragma endregion

#pragma region TIMER

void onStartStop(HWND hwnd)
{
  if (started)
  {
    started = false;
    SetDlgItemText(hwnd, 0x6, L"start timer");
    SetDlgItemText(hwnd, 0x7, L"00:00:00");
    timer = 0;
    EnableWindow(GetDlgItem(hwnd, 0x07), false);
    KillTimer(hwnd, 0x10);
  }
  else
  {
    started = true;
    SetDlgItemText(hwnd, 0x6, L"stop");
    EnableWindow(GetDlgItem(hwnd, 0x07), true);
    SetTimer(hwnd, 0x10, 1000, (TIMERPROC)NULL);
  }
  InvalidateRect(hwnd, NULL, NULL);
}

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

void onTimer(HWND hwnd)
{
  timer++;
  WCHAR timerStr[20];
  wcscpy(timerStr, timeFormat(timer));
  SetDlgItemText(hwnd, 0x7, timerStr);
}

#pragma endregion

#pragma region COMMAND_MSGS

void checkEnable(HWND hwnd)
{
  if (IsDlgButtonChecked(hwnd, 0x01)) EnableWindow(GetDlgItem(hwnd, 0x02), true);
  else EnableWindow(GetDlgItem(hwnd, 0x02), false);
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
    SetDlgItemText(hwnd, 0x05, szPath);
    IMalloc* imalloc = 0;
    if (SUCCEEDED(SHGetMalloc(&imalloc)))
    {
      imalloc->Free(pidl);
      imalloc->Release();
    }
  }
}

#pragma endregion

#pragma region PAINT

void onPaint(HDC hdc)
{
  Graphics graphics(hdc);
  graphics.SetSmoothingMode(SmoothingModeHighQuality);
  if (started)
  {
    SolidBrush brush(Color::White);
    graphics.FillRectangle(&brush, 427, 127, 30, 30);
    brush.SetColor(Color::LimeGreen);
    graphics.FillEllipse(&brush, 430, 130, 24, 24);
  }
  else
  {
    SolidBrush brush(Color::White);
    graphics.FillRectangle(&brush, 427, 127, 30, 30);
    brush.SetColor(Color::Red);
    graphics.FillEllipse(&brush, 430, 130, 24, 24);
  }
}

#pragma endregion

#pragma region CREATION

void onCreate(HWND hwnd)
{
  CreateWindowEx(0, L"Button", L"enable file selection", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 20, 40, 150, 25,
    hwnd, (HMENU)0x1, 0, 0);
  CheckDlgButton(hwnd, 0x1, BST_CHECKED);
  CreateWindowEx(0, L"Button", L"select file path", WS_CHILD | WS_VISIBLE, 35, 70, 120, 25,
    hwnd, (HMENU)0x2, 0, 0);
  CreateWindowEx(0, L"Button", L"file operations", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 10, 171, 100,
    hwnd, (HMENU)0x3, 0, 0);
  CreateWindowEx(0, L"Static", L"file path", WS_CHILD | WS_VISIBLE, 195, 44, 70, 25,
    hwnd, (HMENU)0x4, 0, 0);
  GetCurrentDirectory(MAX_PATH, szPath);
  CreateWindowEx(WS_EX_CLIENTEDGE, L"Edit", szPath, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_READONLY, 195, 70, MAX_PATH + 10, 25,
    hwnd, (HMENU)0x5, GetModuleHandle(NULL), NULL);
  CreateWindowEx(0, L"Button", L"start timer", WS_CHILD | WS_VISIBLE, 35, 130, 90, 25,
    hwnd, (HMENU)0x6, 0, 0);
  CreateWindowEx(0, L"Static", L"00:00:00", WS_CHILD | WS_VISIBLE, 140, 135, 90, 25,
    hwnd, (HMENU)0x7, 0, 0);
  EnableWindow(GetDlgItem(hwnd, 0x07), false);
}

LRESULT onCtlColorStatic(HDC hdc, HWND hwnd)
{
  SetBkColor(hdc, 0xFFFFFF);
  return (LRESULT)GetStockObject(WHITE_BRUSH);
}

#pragma endregion

#pragma region WINDOWS_PROCEDURE

LRESULT CALLBACK WindProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT result;
  HDC hdc;
  PAINTSTRUCT ps;

  switch (uMsg)
  {
    case WM_CREATE:
      onCreate(hwnd);
      break;
    case WM_CTLCOLORSTATIC:
      if(result = onCtlColorStatic((HDC)wParam, (HWND)lParam)) return result;
      break;
    case WM_COMMAND:
      switch (LOWORD(wParam))
      {
        case 0x1:
          checkEnable(hwnd);
          break;
        case 0x2:
          onSelectFilePath(hwnd);
          break;
        case 0x6:
          onStartStop(hwnd);
          break;
      }
      break;
    case WM_TIMER:
      switch (wParam)
      {
        case 0x10:
          onTimer(hwnd);
          return 0;
      }
      break;
    case WM_PAINT:
      hdc = BeginPaint(hwnd, &ps);
      onPaint(hdc);
      EndPaint(hwnd, &ps);
      return 0;
    case WM_CLOSE:
      PostQuitMessage(0);
      return 0;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

#pragma endregion

#pragma region WIN_MAIN

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
  const LPCTSTR WNDNAME = L"Win32 interfaceLayout";
  GdiplusStartupInput gdiplusStartupInput;
  ULONG_PTR gdiplusToken = NULL;

  GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

  WNDCLASSEX wnd = { sizeof(WNDCLASSEX), 0, WindProc, 0, 0, hInst, NULL, LoadCursor(0, IDC_ARROW),
                     (HBRUSH) GetStockObject(WHITE_BRUSH), NULL, WNDNAME, NULL };

  if (RegisterClassEx(&wnd))
  {
    HWND hwnd = CreateWindowEx(0, WNDNAME, L"File Selector Example", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 
                                CW_USEDEFAULT, CW_USEDEFAULT, 500, 220, NULL, NULL, hInst, NULL);
    if (IsWindow(hwnd))
    {
      ShowWindow(hwnd, nCmdShow);

      MSG msg = {};
      while (GetMessage(&msg, NULL, 0, 0))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
  }

  GdiplusShutdown(gdiplusToken);
  return 0;
}

#pragma endregion