// PainGDI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "PainGDI.h"
//#include"Commctrl.h"
#include <iostream>
#include <vector>
using namespace std;
#include <windowsX.h>
#include "Shobjidl.h"
#include "Shlobj.h"
#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib")
#include<math.h>



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
    LoadStringW(hInstance, IDC_PAINGDI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAINGDI));

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

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAINGDI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PAINGDI);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
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
class CShape {
public:
	virtual void Draw(HDC hdc) = 0;
	virtual CShape* Create(int a, int b, int c, int d) = 0;
};

class CLine : public CShape {
private:
	int x1;
	int y1;
	int x2;
	int y2;
public:
	void Draw(HDC hdc) {
		MoveToEx(hdc, x1, y1, NULL);
		LineTo(hdc, x2, y2);
	}

	CLine(int a, int b, int c, int d) {
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
	}

	CShape* Create(int a, int b, int c, int d) {
		return new CLine(a, b, c, d);
	}

	CLine() {}
};

class CRectangle : public CShape {
private:
	int x1;
	int y1;
	int x2;
	int y2;
public:
	void Draw(HDC hdc) {
		Rectangle(hdc, x1, y1, x2, y2);
	}

	CRectangle(int a, int b, int c, int d) {
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
	}

	CShape* Create(int a, int b, int c, int d) {
		return new CRectangle(a, b, c, d);
	}

	CRectangle() {}
};

class CEllipse : public CShape {
private:
	int x1;
	int y1;
	int x2;
	int y2;
public:
	void Draw(HDC hdc) {
		Ellipse(hdc, x1, y1, x2, y2);
	}

	CEllipse(int a, int b, int c, int d) {
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
	}

	CShape* Create(int a, int b, int c, int d) {
		return new CEllipse(a, b, c, d);
	}

	CEllipse() {}
};






HWND hStatusBar;

int startX;
int startY;
int lastX;
int lastY;
bool isDrawing = false;

vector<CShape*> shapes;
//vector<CShape*> prototypes;

int checkType = 0; //kiểu của hình vẽ // == 0 là line, 1 là rectangle, 2 là ellipse, 3 là square, 4 là circle

int RGBx = 214;
int RGBy = 45;
int RGBz = 218;


HDC          hdcMem;
HBITMAP      hbmMem;
HANDLE       hOld;



int setType(int &startX, int &startY, int &lastX, int &lastY)
{
	int x = abs(lastX - startX); //chiều ngang
	int y = abs(lastY - startY); // chiều dài
	if (x > y) //chiều ngang lớn hơn --> quy đổi chiều ngang thành chiều dài, tức ..
	{
		if (lastX>startX)
		lastX -= (x - y);
		else lastX += (x - y);
	}
	else //Chiều dài lớn hơn --> quy đổi chiều dài thành ngang
	{
		if (lastY>startY)
		lastY -= (y - x);
		else lastY += (y - x);
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	HMENU hMenu = GetMenu(hWnd); // Lấy menu của màn hình chính
	UINT state;
	bool isChecked;


    switch (message)
    {
	case WM_LBUTTONDOWN:
	{
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		startX = x;
		startY = y;
		isDrawing = true;
	}
	break;
	case WM_MOUSEMOVE: {
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);

		WCHAR buffer[100];
		wsprintf(buffer, L"  %d, %d", x, y);
		SendMessage(hStatusBar, SB_SETTEXT, 2, (LPARAM)buffer);
		if (isDrawing) {
			lastX = x;
			lastY = y;
			SendMessage(hStatusBar, SB_SETTEXT, 1, (LPARAM)buffer);
			InvalidateRect(hWnd, NULL, TRUE);
		}
	}break;

	case WM_LBUTTONUP: 
	{
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		
		switch (checkType)
		{
		case 0: shapes.push_back(new CLine(startX, startY, lastX, lastY)); break;
		case 1: shapes.push_back(new CRectangle(startX, startY, lastX, lastY)); break;
		case 2: shapes.push_back(new CEllipse(startX, startY, lastX, lastY)); break;
		case 3:
		{
			setType(startX, startY, lastX, lastY);
			shapes.push_back(new CRectangle(startX, startY, lastX, lastY)); break;
		}break;
		case 4:
		{
			setType(startX, startY, lastX, lastY);
			shapes.push_back(new CEllipse(startX, startY, lastX, lastY)); break;
		}break;
		default:
			break;
		}
		isDrawing = false;
		InvalidateRect(hWnd, NULL, TRUE);
	}break;

	case WM_KEYDOWN:
	{
		if (wParam == VK_SHIFT)
		{
			if (checkType == 1)
			{
				SendMessage(hStatusBar, SB_SETTEXT, 0, (LPARAM)L"Square");
				checkType = 3; break;
			}
			else if (checkType == 2)
			{
				SendMessage(hStatusBar, SB_SETTEXT, 0, (LPARAM)L"Circle");
				checkType = 4; break;
			}
		}

	} break;
	case WM_KEYUP:
	{
		if (wParam == VK_SHIFT)
		{
			if (3 == checkType)
			{
				SendMessage(hStatusBar, SB_SETTEXT, 0, (LPARAM)L"Rectangle");
				checkType = 1;
			}
			else if (4 == checkType)
			{
				SendMessage(hStatusBar, SB_SETTEXT, 0, (LPARAM)L"Ellipse");
				checkType = 2;
			}
		}
	}break;

	case WM_CREATE:
	{
		int sizes[] = { 300, 400, 500, 1320 };
		InitCommonControls();
		hStatusBar = CreateWindow(STATUSCLASSNAME, NULL,
			WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
			0, 0, 0, 0, hWnd, NULL, hInst, NULL);

		SendMessage(hStatusBar, SB_SETPARTS, 4, (LPARAM)&sizes);
		SendMessage(hStatusBar, SB_SETTEXT, 1, (LPARAM)L"  0, 0");
		SendMessage(hStatusBar, SB_SETTEXT, 0, (LPARAM)L"Line");
	} break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case ID_TYPE_RECTANGLE:
			{
				CheckMenuItem(hMenu, ID_TYPE_RECTANGLE, MF_CHECKED);
				CheckMenuItem(hMenu, ID_TYPE_LINE, MF_UNCHECKED);
				CheckMenuItem(hMenu, ID_TYPE_ELLIPSE, MF_UNCHECKED);
				SendMessage(hStatusBar, SB_SETTEXT, 0, (LPARAM)L"Rectangle");
				checkType = 1;
			}	break;
			case ID_TYPE_LINE:
			{
				CheckMenuItem(hMenu, ID_TYPE_RECTANGLE, MF_UNCHECKED);
				CheckMenuItem(hMenu, ID_TYPE_LINE, MF_CHECKED);
				CheckMenuItem(hMenu, ID_TYPE_ELLIPSE, MF_UNCHECKED);
				SendMessage(hStatusBar, SB_SETTEXT, 0, (LPARAM)L"Line");
				checkType = 0;
			}	break;
			case ID_TYPE_ELLIPSE:
			{
				CheckMenuItem(hMenu, ID_TYPE_RECTANGLE, MF_UNCHECKED);
				CheckMenuItem(hMenu, ID_TYPE_LINE, MF_UNCHECKED);
				CheckMenuItem(hMenu, ID_TYPE_ELLIPSE, MF_CHECKED);
				SendMessage(hStatusBar, SB_SETTEXT, 0, (LPARAM)L"Ellipse");
				checkType = 2;
			}	break;
			
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	/*case WM_ERASEBKGND:
		return 1;*/

    case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		RECT rect;
		GetClientRect(hWnd, &rect);
		// Create an off-screen DC for double-buffering
		hdcMem = CreateCompatibleDC(hdc);
		hbmMem = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);

		hOld = SelectObject(hdcMem, hbmMem);
		FillRect(hdcMem, &rect, NULL);
		// Draw into hdcMem
		
		SetDCPenColor(hdcMem, RGB(RGBx, RGBy, RGBz));
		SelectObject(hdcMem, GetStockObject(NULL_BRUSH));
		SelectObject(hdcMem, GetStockObject(DC_PEN));
		for (int i = 0; i < shapes.size(); i++) {
			shapes[i]->Draw(hdcMem);
		}
		if (isDrawing)
		{
			switch (checkType)
			{
			case 0:
			{
				MoveToEx(hdcMem, startX, startY, NULL);
				LineTo(hdcMem, lastX, lastY);
			}break;
			case 1:
				Rectangle(hdcMem, startX, startY, lastX, lastY); break;
			case 2:
				Ellipse(hdcMem, startX, startY, lastX, lastY); break;
			case 3:
			{
				setType(startX, startY, lastX, lastY);
				Rectangle(hdcMem, startX, startY, lastX, lastY);
			}break;
			case 4:
			{
				setType(startX, startY, lastX, lastY);
				Ellipse(hdcMem, startX, startY, lastX, lastY);
			}break;
			default:
				break;
			}
		}
		
		// Transfer the off-screen DC to the screen
		BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hdcMem, 0, 0, SRCCOPY);
		// Free-up the off-screen DC
		SelectObject(hdcMem, hOld);
		DeleteObject(hbmMem);
		DeleteDC(hdcMem);

		EndPaint(hWnd, &ps);
	}
        break;
	case WM_SIZE:
	{
		SendMessage(hStatusBar, WM_SIZE, true, true);

		break;
	}
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
