// Notepad.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Notepad.h"
#include <richedit.h>
#include <Winuser.h>
#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include <string>
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_NOTEPAD, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NOTEPAD));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
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
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NOTEPAD));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_3DLIGHT+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_NOTEPAD);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
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
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW /*| WS_HSCROLL | WS_VSCROLL*/,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

//HWND hwnd;
HFONT hFont;

HWND editText;
int size;
RECT rect;
int cx, cy;


WCHAR* bufferOUT = NULL;
std::wofstream fo;
std::ifstream fi;
std::string bufTemp, bufIn, fileName;

//const int BUFFER_SIZE = 255;
//WCHAR* buffer = NULL;
int mes;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (GetWindowRect(hWnd, &rect))
	{
		cx = rect.right - rect.left;
		cy = rect.bottom - rect.top;
	}
    switch (message)
    {


	case WM_CREATE:
	{
		LoadLibrary(L"riched32.dll");
		//Lấy font hệ thống
		LOGFONT lf;
		GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
		hFont = CreateFont(lf.lfHeight-4, lf.lfWidth,
			lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
			lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
			lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
			lf.lfPitchAndFamily, lf.lfFaceName);
		
		editText = CreateWindowEx(WS_EX_ACCEPTFILES, L"edit", L"",
			WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL |WS_HSCROLL | WS_VSCROLL, 
			0, 1, cx-9, cy-47,
			hWnd, NULL, hInst, NULL);

		SendMessage(editText, WM_SETFONT, WPARAM(hFont), TRUE);


		break;
	}
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case ID_FILE_SAVE:
			{
				size = GetWindowTextLength(editText);
				bufferOUT = new WCHAR[size + 1];
				GetWindowText(editText, bufferOUT, size + 1);
				//fileName = "C:\Users\Public\Desktop\fileOut.txt";
				fo.open("fileOut.txt");
				fo << bufferOUT;
				fo.close();
				SetWindowText(hWnd, L"fileOut.txt - Notepad");
				MessageBox(hWnd, L"The file is saved with name ""fileOut.txt""", L"Notification", MB_OK);

				break;
			}
			case ID_FILE_OPEN:
			{
				mes = MessageBox(hWnd, L"Make sure you have saved these files.\r\nIf saved, select OK.", L"Warning", MB_OKCANCEL | MB_ICONQUESTION);
				switch (mes)
				{
				case IDCANCEL:
					break;
				case IDOK:
				{
					
					fi.open("fileIn.txt");	//mở file để đọc

					if (fi.fail())
					{
						MessageBox(hWnd, L"Failed to open file ""fileIn.txt""!", L"Notification", MB_OK | MB_ICONEXCLAMATION);
						break;
					}
					
					while (!fi.eof()) //đọc từng dòng của file
					{
						std::getline(fi, bufTemp);
						bufIn += bufTemp; //nối vào string
						bufIn += "\r\n"; //thêm ký tự xuống dòng
					}
					fi.close();
					std::wstring wide_string = std::wstring(bufIn.begin(), bufIn.end()); //chuyển string sang thành wstring

					const wchar_t* bufferI = wide_string.c_str(); //chuyển wstring sang thành WCHAR
					
					SetWindowText(editText, bufferI);
					bufIn = ""; //Hủy các ký tự đã lấy vào vùng nhớ của file vừa mở - sử dụng lại cho các file sau

					MessageBox(hWnd, L"The file named ""fileIn.txt"" was opened.", L"Notification", MB_OK);
					SetWindowText(hWnd, L"fileIn.txt - Notepad");
					break;
				}
				}
				break;
			}
			case ID_FILE_NEW:
			{
				mes = MessageBox(hWnd, L"Make sure you have saved these files.\r\nIf saved, select OK.", L"Warning", MB_OKCANCEL | MB_ICONQUESTION);
				switch (mes)
				{
				case IDCANCEL:
					break;
				case IDOK:
					SetWindowText(hWnd, L"Untitled - Notepad");
					SetWindowText(editText, L"");
					break;
				}
			}
			case ID_EDIT_COPY:
				SendMessage(editText, WM_COPY, 0, 0);
				break;
			case ID_EDIT_CUT:
				SendMessage(editText, WM_CUT, 0, 0);
				break;
			case ID_EDIT_PASTE:
				SendMessage(editText, WM_PASTE, 0, 0);
				break;
			case ID_EDIT_DELETE:
				SendMessage(editText, WM_CLEAR, 0, 0);
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
			if (!bufferOUT) delete[] bufferOUT;
        }
        break;
	case WM_SIZE:

		SetWindowPos(editText, 0, 0, 0, cx-9, cy-47, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		
		/*buffer = new WCHAR[BUFFER_SIZE];
		wsprintf(buffer, L"Kich thuoc moi: cx = %d, cy = %d", cx, cy);
		CreateWindow(L"EDIT", buffer, WS_CHILD | WS_VISIBLE | WS_BORDER, 160, 30, 500, 18, hWnd, NULL, hInst, NULL);*/

		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
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
