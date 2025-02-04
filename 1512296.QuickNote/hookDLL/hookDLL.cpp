// hookDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include <Windowsx.h>
#include<string>
#include <stdio.h>
#include<iostream>
#define  ID_ADDNOTE						10004
//#define WM_LBUTTONDBLCLK                0x0203
#define EXPORT  __declspec(dllexport)
HHOOK hHook = NULL;
HINSTANCE hinstLib;
LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0) // không xử lý message 
	{
		HWND hWnd = FindWindow(L"QUICKNOTE", L"quicknote");
		if (GetAsyncKeyState(VK_LWIN) < 0 &&
			GetAsyncKeyState(VK_SPACE) < 0)
		{
			SendMessage(hWnd, WM_COMMAND, ID_ADDNOTE, 0);
		}
	}
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}
EXPORT void removeHook(HWND hWnd)
{
	if (hHook == NULL) return;
	UnhookWindowsHookEx(hHook);
	hHook = NULL;
}
EXPORT void installHook(HWND hWnd)
{
	if (hHook != NULL) return;
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardHookProc, hinstLib, 0);
}



