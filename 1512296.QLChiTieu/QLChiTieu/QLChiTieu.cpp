// QLChiTieu.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "QLChiTieu.h"
#include <windowsX.h>
#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib")

//#include <gdiplus.h>
//#pragma comment(lib, "gdiplus.lib")
//using namespace Gdiplus;

#include <fstream>
#pragma warning(disable:4996)
#define MAX_LOADSTRING 100


//#include "Shobjidl.h"
//#include "Shlobj.h"
//#include "shellapi.h"
//#include "Shlwapi.h"


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
    LoadStringW(hInstance, IDC_QLCHITIEU, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_QLCHITIEU));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QLCHITIEU));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_3DFACE+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_QLCHITIEU);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 985 + 20, 513 + 60, nullptr, nullptr, hInstance, nullptr);

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
struct MucChi {
	wchar_t loaiChiTieu[50];
	wchar_t noiDung[255];
	int soTien;
}; //kiểu dữ liệu mục chi

int DocFile(int &n, HWND ListViewt, unsigned long int pay[7]) //Đọc file nhị phân và ghi thông tin ra list view 
{
	MucChi dl[1000];
	
	FILE *in = fopen("data.txt", "rb");
	if (!in)
	{
		return 0;
	}
	else
	{
		fseek(in, 0, SEEK_END); //Nhay ve cuoi file, di chuyen di 0 vi tri
		n = (ftell(in) + 1) / sizeof(MucChi); //ftell(); tra ve vi tri hien tai cua con tro
										  // SEEK_CUR: di chuyen bat dau tu vi tri hien tai cua con tro, chi dung trong fseek()
		fseek(in, 0, SEEK_SET); //Nhay ve dau file, di chuyen di 0 vi tri
		fread(dl, sizeof(MucChi), n, in); //đọc hết vô mảng
		fclose(in);
		for (int i = 0; i < n; i++)
		{
			LVITEM item;
			item.mask = LVIF_TEXT;
			item.pszText = dl[i].loaiChiTieu;
			item.iItem = i;
			item.iSubItem = 0;
			ListView_InsertItem(ListViewt, &item);
			ListView_SetItemText(ListViewt, i, 1, dl[i].noiDung);
			WCHAR money[15];
			swprintf(money, L"%d", dl[i].soTien);
			ListView_SetItemText(ListViewt, i, 2, money);
			if (0 == wcscmp(dl[i].loaiChiTieu, L"Ăn uống")) //bằng nhau//loại 0
				pay[0] += dl[i].soTien;
			else
			{
				if (0 == wcscmp(dl[i].loaiChiTieu, L"Di chuyển")) //bằng nhau//loại 1
					pay[1] += dl[i].soTien;
				else
				{
					if (0 == wcscmp(dl[i].loaiChiTieu, L"Nhà cửa")) //bằng nhau//loại 0
						pay[2] += dl[i].soTien;
					else {
						if (0 == wcscmp(dl[i].loaiChiTieu, L"Xe cộ")) //bằng nhau//loại 0
							pay[3] += dl[i].soTien;
						else {
							if (0 == wcscmp(dl[i].loaiChiTieu, L"Nhu yếu phẩm")) //bằng nhau//loại 0
								pay[4] += dl[i].soTien;
							else {
								if (0 == wcscmp(dl[i].loaiChiTieu, L"Dịch vụ")) //bằng nhau//loại 0
									pay[5] += dl[i].soTien;
								else {
									pay[6] += dl[i].soTien;
								}
							}
						}
					}
				}
			}
		}
	}
	return 0;
}
int miniPow(int a, int b)    // power with small number - just demo
{
	int r = 1;
	for (int i = 0; i < b; i++)
	{
		r *= a;
	}
	return r;
}
int WideToInt(wchar_t a[])
{
	int i = 0;
	int len = wcslen(a);
	int coefficient = 0;
	int RawNumber = 0;
	int Number = 0;

	for (int k = 0; k < len; k++)
	{
		coefficient = miniPow(10, len - 1 - k);
		RawNumber = (int)a[k];
		Number = RawNumber - 48;
		i += coefficient * Number;
	}
	return i;
}
int GhiFile(int n, HWND ListViewt) //từ list view lấy thông tin và ghi lại vô file
{
	//*lấy dữ liệu
	MucChi dl1[1000];
	for (int i = 0; i<n; i++)
	{
		//LVITEM itemTemp;
		//itemTemp.mask = LVFIF_TEXT;
		//itemTemp.iItem = i;
		//itemTemp.iSubItem = 0;
		//itemTemp.pszText = bufferT;
		//itemTemp.cchTextMax = 255;
		MucChi temp;
		WCHAR bufferT[20];
		ListView_GetItemText(ListViewt, i, 0, temp.loaiChiTieu, 255);
		ListView_GetItemText(ListViewt, i, 1, temp.noiDung, 255);
		ListView_GetItemText(ListViewt, i, 2, bufferT, 255);
		temp.soTien = WideToInt(bufferT);
		dl1[i] = temp;
		/*	MessageBox(MB_OK, temp.loaiChiTieu, 0, 0);
		MessageBox(MB_OK, temp.noiDung, 0, 0);
		MessageBox(MB_OK, bufferT, 0, 0);*/
	}
	//*ghi vô file
	FILE *out;
	out = fopen("data.txt", "wb");
	fwrite(dl1, sizeof(MucChi), n, out);
	fclose(out);


	return 0;
}


int nX, nY; //Tâm của cái hình tròn
DWORD dwRadius; //Bán kính của cái hình tròn
float xStartAngle; //Góc bắt đầy quay
float xSweepAngle; //Góc kết thúc quay, đơn vị là độ
//Khi vẽ, hình tròn vẽ từ góc bắt đầu quay theo chiều ngược kim đồng hồ nếu góc kết thúc là dương và ngược lại

//Thông tin hình cung tròn làm chú thích
int nX_note, nY_note; //Tâm của cái hình tròn
DWORD dwRadius_note; //Bán kính của cái hình tròn
float xStartAngle_note; //Góc bắt đầy quay
float xSweepAngle_note; //Góc kết thúc quay, đơn vị là độ


unsigned long int pay[7] = { 0, 0, 0, 0, 0, 0, 0 };//số tiền tổng của từng khoảng, khoảng cuối cùng là loại "khác"
float ratio[7];
unsigned long int paySum; //số tiền chi ra tất cả;
int sizeLCT, sizeND, sizeST; //size của ô
WCHAR *bufferLCT = NULL;
WCHAR *bufferND = NULL;
WCHAR *bufferST = NULL;
int sotien;

int n = 0; //số lượng mục chi

HFONT hFont;
HWND hwnd, loaiChiTieu, noiDung, soTien, tongCong;
HWND TLAnUong, TLDiChuyen, TLNhaCua, TLXeCo, TLNhuYeuPham, TLDichVu, TLKhac;

HWND ListView;
int x = 20;
int y = 10;
int z = 590;
int t = 570;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
	{
		//Lấy font hệ thống
		SetWindowText(hWnd, L"Quản lý chi tiêu");
		LOGFONT lf;
		GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
		hFont = CreateFont(lf.lfHeight - 2, lf.lfWidth,
			lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
			lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
			lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
			lf.lfPitchAndFamily, lf.lfFaceName);

		/*hwnd = CreateWindow(L"BUTTON", L"Hello", WS_CHILD | WS_GROUP | BS_GROUPBOX, 500, 50, 100, 100, hWnd, NULL, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);*/
		
		//Thêm một loại chi tiêu
		hwnd = CreateWindowEx(0, L"BUTTON", L"Thêm một loại chi tiêu", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			x, y, t-x, 90, hWnd, NULL, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
		hwnd = CreateWindow(L"STATIC", L"Loại chi tiêu:", WS_CHILD | WS_VISIBLE | SS_LEFT, 42, 35, 130, 20, hWnd, NULL, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

		hwnd = CreateWindow(L"STATIC", L"Nội dung:", WS_CHILD | WS_VISIBLE | SS_LEFT, 184, 35, 130, 20, hWnd, NULL, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

		hwnd = CreateWindow(L"STATIC", L"Số tiền (VNĐ):", WS_CHILD | WS_VISIBLE | SS_LEFT, 325, 35, 130, 20, hWnd, NULL, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

		loaiChiTieu = CreateWindow(WC_COMBOBOX, TEXT(""),
			CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | CBS_AUTOHSCROLL | CBS_DISABLENOSCROLL | WS_BORDER,
			42, 60, 120, 22, hWnd, HMENU(ID_CBCHITIEU), hInst,
			NULL);

		ComboBox_AddString(loaiChiTieu, L"Ăn uống");
		ComboBox_AddString(loaiChiTieu, L"Di chuyển");
		ComboBox_AddString(loaiChiTieu, L"Nhà cửa");
		ComboBox_AddString(loaiChiTieu, L"Xe cộ");
		ComboBox_AddString(loaiChiTieu, L"Nhu yếu phẩm");
		ComboBox_AddString(loaiChiTieu, L"Dịch vụ");
		SendMessage(loaiChiTieu, WM_SETFONT, WPARAM(hFont), TRUE);

		noiDung = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 184, 60, 120, 24, hWnd, NULL, hInst, NULL);
		SendMessage(noiDung, WM_SETFONT, WPARAM(hFont), TRUE);

		soTien = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER, 325, 60, 120, 24, hWnd, NULL, hInst, NULL);
		SendMessage(soTien, WM_SETFONT, WPARAM(hFont), TRUE);

		hwnd = CreateWindow(L"BUTTON", L"Thêm", WS_VISIBLE | WS_CHILD , 470, 60, 80, 25, hWnd, (HMENU)IDC_ADD, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);	

		//tạo list view
		INITCOMMONCONTROLSEX icex;
		// Ensure that the common control DLL is loaded. 
		icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES;
		InitCommonControlsEx(&icex);
		//Toàn bộ danh sách các chi tiêu
		hwnd = CreateWindowEx(0, L"BUTTON", L"Toàn bộ danh sách các chi tiêu", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			x, y + 98, t - x, 300, hWnd, NULL, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

		ListView = CreateWindow(WC_LISTVIEWW, L"",
			WS_CHILD | WS_VISIBLE | LVS_REPORT | WS_VSCROLL | WS_TABSTOP | WS_BORDER,
			42, 140, 505, 250,
			hWnd, (HMENU)IDC_LISTVIEW, hInst, NULL);

		LVCOLUMN col1;
		col1.mask = LVCF_TEXT | LVCF_WIDTH;
		col1.pszText = L"Loại chi tiêu";
		col1.cx = 150;
		ListView_InsertColumn(ListView, 0, &col1);

		LVCOLUMN col2;
		col2.mask = LVCF_TEXT | LVCF_WIDTH;
		col2.pszText = L"Nội dung";
		col2.cx = 200;
		ListView_InsertColumn(ListView, 1, &col2);

		LVCOLUMN col3;
		col3.mask = LVCF_TEXT | LVCF_WIDTH;
		col3.pszText = L"Số tiền (VNĐ)";
		col3.cx = 155;
		ListView_InsertColumn(ListView, 2, &col3);
		
		DocFile(n, ListView, pay);

		//Thông tin thống kê
		hwnd = CreateWindowEx(0, L"BUTTON", L"Thông tin thống kê", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			x, y + 405, t - x, 80, hWnd, NULL, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
		hwnd = CreateWindow(L"STATIC", L"Tổng cộng:", WS_CHILD | WS_VISIBLE | SS_LEFT, 230, 450, 70, 20, hWnd, NULL, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
		tongCong = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY | ES_RIGHT, 320, 450, 120, 22, hWnd, NULL, hInst, NULL);
		SendMessage(tongCong, WM_SETFONT, WPARAM(hFont), TRUE);
		hwnd = CreateWindow(L"STATIC", L"VNĐ", WS_CHILD | WS_VISIBLE | SS_LEFT, 450, 450, 30, 20, hWnd, NULL, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
		//tính tổng và show lên text box
		paySum = pay[0] + pay[1] + pay[2] + pay[3] + pay[4] + pay[5] + pay[6];
		WCHAR Sumtemp[20];
		swprintf(Sumtemp, L"%d", paySum);
		SetWindowText(tongCong, Sumtemp);

		//Chú thích
		hwnd = CreateWindowEx(0, L"BUTTON", L"Biểu đồ thống kê", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			z, y, 375, 495 - y, hWnd, NULL, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
		hwnd = CreateWindow(L"STATIC", L"Chú thích:", WS_CHILD | WS_VISIBLE | SS_LEFT, 605, 245 + 35, 60, 20, hWnd, NULL, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
		if (0 == paySum) { ratio[0] = 0; ratio[1] = 0;
		ratio[2] = 0;
		ratio[3] = 0;
		ratio[4] = 0;
		ratio[5] = 0;
		ratio[6] = 0;
		}
		else {
			ratio[0] = 100 * ((float)pay[0] / (float)paySum);
			ratio[1] = 100 * ((float)pay[1] / (float)paySum);
			ratio[2] = 100 * ((float)pay[2] / (float)paySum);
			ratio[3] = 100 * ((float)pay[3] / (float)paySum);
			ratio[4] = 100 * ((float)pay[4] / (float)paySum);
			ratio[5] = 100 * ((float)pay[5] / (float)paySum);
			ratio[6] = 100 - (ratio[0] + ratio[1] + ratio[2] + ratio[3] + ratio[4] + ratio[5]);
		}
		WCHAR temp_TLAnUong[30], temp_TLDiChuyen[30], temp_TLNhaCua[30], temp_TLXeCo[30], temp_TLNhuYeuPham[30], temp_TLDichVu[30], temp_TLKhac[30];

		swprintf(temp_TLAnUong, L"Ăn uống (%.2f%%)", ratio[0]);
		swprintf(temp_TLDiChuyen, L"Di chuyển (%.2f%%)", ratio[1]);
		swprintf(temp_TLNhaCua, L"Nhà cửa (%.2f%%)", ratio[2]);
		swprintf(temp_TLXeCo, L"Xe cộ (%.2f%%)", ratio[3]);
		swprintf(temp_TLNhuYeuPham, L"Nhu yếu phẩm (%.2f%%)", ratio[4]);
		swprintf(temp_TLDichVu, L"Dịch vụ (%.2f%%)", ratio[5]);
		swprintf(temp_TLKhac, L"Khác (%.2f%%)", ratio[6]);

		TLAnUong = CreateWindow(L"STATIC", temp_TLAnUong, WS_CHILD | WS_VISIBLE | SS_LEFT, 652, 290 + 25, 110 +5, 20, hWnd, NULL, hInst, NULL);
		SendMessage(TLAnUong, WM_SETFONT, WPARAM(hFont), TRUE);
		TLDiChuyen = CreateWindow(L"STATIC", temp_TLDiChuyen, WS_CHILD | WS_VISIBLE | SS_LEFT, 652, 335 + 25,110 +5, 20, hWnd, NULL, hInst, NULL);
		SendMessage(TLDiChuyen, WM_SETFONT, WPARAM(hFont), TRUE);
		TLNhaCua = CreateWindow(L"STATIC", temp_TLNhaCua, WS_CHILD | WS_VISIBLE | SS_LEFT, 652, 380 + 25, 115, 20, hWnd, NULL, hInst, NULL);
		SendMessage(TLNhaCua, WM_SETFONT, WPARAM(hFont), TRUE);
		TLXeCo = CreateWindow(L"STATIC", temp_TLXeCo, WS_CHILD | WS_VISIBLE | SS_LEFT, 652, 425 + 25, 90 + 5, 20, hWnd, NULL, hInst, NULL);
		SendMessage(TLXeCo, WM_SETFONT, WPARAM(hFont), TRUE);
		TLNhuYeuPham = CreateWindow(L"STATIC", temp_TLNhuYeuPham, WS_CHILD | WS_VISIBLE | SS_LEFT, 815, 290 + 25, 140 + 8, 20, hWnd, NULL, hInst, NULL);
		SendMessage(TLNhuYeuPham, WM_SETFONT, WPARAM(hFont), TRUE);
		TLDichVu = CreateWindow(L"STATIC", temp_TLDichVu, WS_CHILD | WS_VISIBLE | SS_LEFT, 815, 335 + 25, 110 + 5, 20, hWnd, NULL, hInst, NULL);
		SendMessage(TLDichVu, WM_SETFONT, WPARAM(hFont), TRUE);
		TLKhac = CreateWindow(L"STATIC", temp_TLKhac, WS_CHILD | WS_VISIBLE | SS_LEFT, 815, 380 + 25, 90 + 5, 20, hWnd, NULL, hInst, NULL);
		SendMessage(TLKhac, WM_SETFONT, WPARAM(hFont), TRUE);


		/*WCHAR tem[20];
		swprintf(tem, L"n = %d", n);
		SetWindowText(hWnd, tem);*/

	}break;
	/*case WM_MOUSEMOVE:
	{
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);

		WCHAR Tbuffer[100];
		wsprintf(Tbuffer, L"  %d, %d", x, y);
		SetWindowText(hWnd, Tbuffer);
	}break;*/
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
			
			int notifycode = HIWORD(wParam);
			HWND combobox = (HWND)lParam;

            // Parse the menu selections:
            switch (wmId)
            {

            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
			case IDC_ADD: //thêm vào danh sách (list view, không thêm vào file), và cập nhật biểu đồ
			{
				//lấy dữ liệu từ mấy cái ô đó rồi thêm vô list view.
				sizeLCT = GetWindowTextLength(loaiChiTieu);
				sizeND = GetWindowTextLength(noiDung);
				sizeST = GetWindowTextLength(soTien);

				bufferLCT = new WCHAR[sizeLCT + 1];
				bufferND = new WCHAR[sizeND + 1];
				bufferST = new WCHAR[sizeST + 1];
				
				GetWindowText(loaiChiTieu, bufferLCT, sizeLCT + 1);
				GetWindowText(noiDung, bufferND, sizeND + 1);
				GetWindowText(soTien, bufferST, sizeST + 1);
				//WCHAR buf[50] = &bufferLCT;

				LVITEM item;
				item.mask = LVIF_TEXT;
				item.pszText = bufferLCT;
				item.iItem = n;
				item.iSubItem = 0;
				ListView_InsertItem(ListView, &item);
				ListView_SetItemText(ListView, n, 1, bufferND);
				ListView_SetItemText(ListView, n, 2, bufferST);
				n++;

				int tien = _wtoi(bufferST);

				//set giá trị thống kê
				int i = ComboBox_GetCurSel(loaiChiTieu); //thứ tự mà combo box đang được chọn, -1 nếu tự nhập.
				switch (i)
				{
				case -1: pay[6] += tien; break;
				case 0: pay[0] += tien; break;
				case 1: pay[1] += tien; break;
				case 2: pay[2] += tien; break;
				case 3: pay[3] += tien; break;
				case 4: pay[4] += tien; break;
				case 5: pay[5] += tien; break;
				}
				//tính tổng và show lên text box
				paySum = pay[0] + pay[1] + pay[2] + pay[3] + pay[4] + pay[5] + pay[6];
				WCHAR Sumtemp[20];
				swprintf(Sumtemp, L"%d", paySum);
				SetWindowText(tongCong, Sumtemp);

				//chia tỉ lệ phần trăm
				ratio[0] = 100 * ((float)pay[0] / (float)paySum);
				ratio[1] = 100 * ((float)pay[1] / (float)paySum);
				ratio[2] = 100 * ((float)pay[2] / (float)paySum);
				ratio[3] = 100 * ((float)pay[3] / (float)paySum);
				ratio[4] = 100 * ((float)pay[4] / (float)paySum);
				ratio[5] = 100 * ((float)pay[5] / (float)paySum);
				ratio[6] = 100 - (ratio[0] + ratio[1] + ratio[2] + ratio[3] + ratio[4] + ratio[5]);

				WCHAR temp_TLAnUong[30], temp_TLDiChuyen[30], temp_TLNhaCua[30], temp_TLXeCo[30], temp_TLNhuYeuPham[30], temp_TLDichVu[30], temp_TLKhac[30];
				swprintf(temp_TLAnUong, L"Ăn uống (%.2f%%)", ratio[0]);
				swprintf(temp_TLDiChuyen, L"Di chuyển (%.2f%%)", ratio[1]);
				swprintf(temp_TLNhaCua, L"Nhà cửa (%.2f%%)", ratio[2]);
				swprintf(temp_TLXeCo, L"Xe cộ (%.2f%%)", ratio[3]);
				swprintf(temp_TLNhuYeuPham, L"Nhu yếu phẩm (%.2f%%)", ratio[4]);
				swprintf(temp_TLDichVu, L"Dịch vụ (%.2f%%)", ratio[5]);
				swprintf(temp_TLKhac, L"Khác (%.2f%%)", ratio[6]);
				SetWindowText(TLAnUong, temp_TLAnUong);
				SetWindowText(TLDiChuyen, temp_TLDiChuyen);
				SetWindowText(TLDichVu, temp_TLDichVu);
				SetWindowText(TLKhac, temp_TLKhac);
				SetWindowText(TLNhaCua, temp_TLNhaCua);
				SetWindowText(TLNhuYeuPham, temp_TLNhuYeuPham);
				SetWindowText(TLXeCo, temp_TLXeCo);

				//set lại các text box thành rỗng
				SetWindowText(loaiChiTieu, L"");
				SetWindowText(noiDung, L"");
				SetWindowText(soTien, L"");
				//gửi lại thông báo WM_PAINT
				InvalidateRect(hWnd, NULL, TRUE);
				UpdateWindow(hWnd);
				//SendMessage(hWnd, WM_PAINT, 0, 0);
			}break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		nX = 775;
		nY = 155;
		dwRadius = (DWORD)110;
		xStartAngle = 180;

		dwRadius_note = (DWORD)28;
		xStartAngle_note = 0;
		xSweepAngle_note = 40;
		if (0 == pay[0] && 0 == pay[1] && 0 == pay[2] && 0 == pay[3] && 0 == pay[4] && 0 == pay[5] && 0 == pay[6])
		{
			xSweepAngle = -360;
			BeginPath(hdc);
			SetDCPenColor(hdc, RGB(160, 160, 160)); //màu bạc
			SetDCBrushColor(hdc, RGB(160, 160, 160)); //màu bạc
			SelectObject(hdc, GetStockObject(DC_BRUSH));
			SelectObject(hdc, GetStockObject(DC_PEN));
			MoveToEx(hdc, nX, nY, (LPPOINT)NULL);
			AngleArc(hdc, nX, nY, dwRadius, xStartAngle, xSweepAngle);
			LineTo(hdc, nX, nY);
			EndPath(hdc);
			StrokeAndFillPath(hdc);
		}
		else
		{
			//paySum = pay[0] + pay[1] + pay[2] + pay[3] + pay[4] + pay[5] + pay[6];
			
			ratio[0] = 100 * ((float)pay[0] / (float)paySum);
			ratio[1] = 100 * ((float)pay[1] / (float)paySum);
			ratio[2] = 100 * ((float)pay[2] / (float)paySum);
			ratio[3] = 100 * ((float)pay[3] / (float)paySum);
			ratio[4] = 100 * ((float)pay[4] / (float)paySum);
			ratio[5] = 100 * ((float)pay[5] / (float)paySum );
			ratio[6] = 100 - (ratio[0] + ratio[1] + ratio[2] + ratio[3] + ratio[4] + ratio[5]);

			//vẽ lần 1 // pay[0]
			//xStartAngle = 152.154;
			xSweepAngle = -ratio[0] * 3.6; //số âm
			BeginPath(hdc);
			SetDCPenColor(hdc, RGB(250, 0, 0)); // màu đỏ
			SetDCBrushColor(hdc, RGB(250, 0, 0));
			SelectObject(hdc, GetStockObject(DC_BRUSH));
			SelectObject(hdc, GetStockObject(DC_PEN));
			MoveToEx(hdc, nX, nY, (LPPOINT)NULL);
			AngleArc(hdc, nX, nY, dwRadius, xStartAngle, xSweepAngle);
			LineTo(hdc, nX, nY);
			EndPath(hdc);
			StrokeAndFillPath(hdc);
			//ReleaseDC(hWnd, hdc);

			//vẽ lần 2 //pay[1]
			xStartAngle = xStartAngle + xSweepAngle;
			xSweepAngle = -ratio[1] * 3.6; //số âm
			BeginPath(hdc);
			SetDCPenColor(hdc, RGB(0, 0, 250)); //blue đậm
			SetDCBrushColor(hdc, RGB(0, 0, 250));
			SelectObject(hdc, GetStockObject(DC_BRUSH));
			SelectObject(hdc, GetStockObject(DC_PEN));
			MoveToEx(hdc, nX, nY, (LPPOINT)NULL);
			AngleArc(hdc, nX, nY, dwRadius, xStartAngle, xSweepAngle);
			LineTo(hdc, nX, nY);
			EndPath(hdc);
			StrokeAndFillPath(hdc);

			//vẽ lần 3 //pay[2]
			xStartAngle = xStartAngle + xSweepAngle;
			xSweepAngle = -ratio[2] * 3.6; //số âm
			BeginPath(hdc);
			SetDCPenColor(hdc, RGB(255, 255, 0)); //yellow
			SetDCBrushColor(hdc, RGB(255, 255, 0));
			SelectObject(hdc, GetStockObject(DC_BRUSH));
			SelectObject(hdc, GetStockObject(DC_PEN));
			MoveToEx(hdc, nX, nY, (LPPOINT)NULL);
			AngleArc(hdc, nX, nY, dwRadius, xStartAngle, xSweepAngle);
			LineTo(hdc, nX, nY);
			EndPath(hdc);
			StrokeAndFillPath(hdc);

			//vẽ lần 4 //pay[3]
			xStartAngle = xStartAngle + xSweepAngle;
			xSweepAngle = -ratio[3] * 3.6; //số âm
			BeginPath(hdc);
			SetDCPenColor(hdc, RGB(0, 255, 0)); //green
			SetDCBrushColor(hdc, RGB(0, 255, 0));
			SelectObject(hdc, GetStockObject(DC_BRUSH));
			SelectObject(hdc, GetStockObject(DC_PEN));
			MoveToEx(hdc, nX, nY, (LPPOINT)NULL);
			AngleArc(hdc, nX, nY, dwRadius, xStartAngle, xSweepAngle);
			LineTo(hdc, nX, nY);
			EndPath(hdc);
			StrokeAndFillPath(hdc);

			//vẽ lần 5 //pay[4]
			xStartAngle = xStartAngle + xSweepAngle;
			xSweepAngle = -ratio[4] * 3.6; //số âm
			BeginPath(hdc);
			SetDCPenColor(hdc, RGB(0, 255, 255)); //xanh nhạt (blue) //méo biết tên màu
			SetDCBrushColor(hdc, RGB(0, 255, 255));
			SelectObject(hdc, GetStockObject(DC_BRUSH));
			SelectObject(hdc, GetStockObject(DC_PEN));
			MoveToEx(hdc, nX, nY, (LPPOINT)NULL);
			AngleArc(hdc, nX, nY, dwRadius, xStartAngle, xSweepAngle);
			LineTo(hdc, nX, nY);
			EndPath(hdc);
			StrokeAndFillPath(hdc);
			

			//vẽ lần 6 //pay[5]
			xStartAngle = xStartAngle + xSweepAngle;
			xSweepAngle = -ratio[5] * 3.6; //số âm
			BeginPath(hdc);
			SetDCPenColor(hdc, RGB(204, 0, 204)); //tím mộng mơ
			SetDCBrushColor(hdc, RGB(204, 0, 204));
			SelectObject(hdc, GetStockObject(DC_BRUSH));
			SelectObject(hdc, GetStockObject(DC_PEN));
			MoveToEx(hdc, nX, nY, (LPPOINT)NULL);
			AngleArc(hdc, nX, nY, dwRadius, xStartAngle, xSweepAngle);
			LineTo(hdc, nX, nY);
			EndPath(hdc);
			StrokeAndFillPath(hdc);

			//vẽ lần 7 //pay[6]
			xStartAngle = xStartAngle + xSweepAngle;
			xSweepAngle = -ratio[6] * 3.6; //số âm
			BeginPath(hdc);
			SetDCPenColor(hdc, RGB(255, 153, 204));
			SetDCBrushColor(hdc, RGB(255, 153, 204));
			SelectObject(hdc, GetStockObject(DC_BRUSH));
			SelectObject(hdc, GetStockObject(DC_PEN));
			MoveToEx(hdc, nX, nY, (LPPOINT)NULL);
			AngleArc(hdc, nX, nY, dwRadius, xStartAngle, xSweepAngle);
			LineTo(hdc, nX, nY);
			EndPath(hdc);
			StrokeAndFillPath(hdc);

			
		}

		//Vẽ chú thích
		nX_note = 613;
		nY_note = 305 + 25;
		BeginPath(hdc);
		SetDCPenColor(hdc, RGB(250, 0, 0)); // màu đỏ //chú thích 1
		SetDCBrushColor(hdc, RGB(250, 0, 0));
		SelectObject(hdc, GetStockObject(DC_BRUSH));
		SelectObject(hdc, GetStockObject(DC_PEN));
		MoveToEx(hdc, nX_note, nY_note, (LPPOINT)NULL);
		AngleArc(hdc, nX_note, nY_note, dwRadius_note, xStartAngle_note, xSweepAngle_note);
		LineTo(hdc, nX_note, nY_note);
		EndPath(hdc);
		StrokeAndFillPath(hdc);

		nX_note = 613;
		nY_note = 350 + 25;
		BeginPath(hdc);
		SetDCPenColor(hdc, RGB(0, 0, 250)); // blue đậm //chú thích 2
		SetDCBrushColor(hdc, RGB(0, 0, 250));
		SelectObject(hdc, GetStockObject(DC_BRUSH));
		SelectObject(hdc, GetStockObject(DC_PEN));
		MoveToEx(hdc, nX_note, nY_note, (LPPOINT)NULL);
		AngleArc(hdc, nX_note, nY_note, dwRadius_note, xStartAngle_note, xSweepAngle_note);
		LineTo(hdc, nX_note, nY_note);
		EndPath(hdc);
		StrokeAndFillPath(hdc);

		nX_note = 613;
		nY_note = 395 + 25;
		BeginPath(hdc);
		SetDCPenColor(hdc, RGB(255, 255, 0)); // yellow //chú thích 3
		SetDCBrushColor(hdc, RGB(255, 255, 0));
		SelectObject(hdc, GetStockObject(DC_BRUSH));
		SelectObject(hdc, GetStockObject(DC_PEN));
		MoveToEx(hdc, nX_note, nY_note, (LPPOINT)NULL);
		AngleArc(hdc, nX_note, nY_note, dwRadius_note, xStartAngle_note, xSweepAngle_note);
		LineTo(hdc, nX_note, nY_note);
		EndPath(hdc);
		StrokeAndFillPath(hdc);

		nX_note = 613;
		nY_note = 440 +25;
		BeginPath(hdc);
		SetDCPenColor(hdc, RGB(0, 255, 0)); // green //chú thích 4
		SetDCBrushColor(hdc, RGB(0, 255, 0));
		SelectObject(hdc, GetStockObject(DC_BRUSH));
		SelectObject(hdc, GetStockObject(DC_PEN));
		MoveToEx(hdc, nX_note, nY_note, (LPPOINT)NULL);
		AngleArc(hdc, nX_note, nY_note, dwRadius_note, xStartAngle_note, xSweepAngle_note);
		LineTo(hdc, nX_note, nY_note);
		EndPath(hdc);
		StrokeAndFillPath(hdc);

		nX_note = 776;
		nY_note = 305 + 25;
		BeginPath(hdc);
		SetDCPenColor(hdc, RGB(0, 255, 255)); // Xanh nhạt //chú thích 5
		SetDCBrushColor(hdc, RGB(0, 255, 255));
		SelectObject(hdc, GetStockObject(DC_BRUSH));
		SelectObject(hdc, GetStockObject(DC_PEN));
		MoveToEx(hdc, nX_note, nY_note, (LPPOINT)NULL);
		AngleArc(hdc, nX_note, nY_note, dwRadius_note, xStartAngle_note, xSweepAngle_note);
		LineTo(hdc, nX_note, nY_note);
		EndPath(hdc);
		StrokeAndFillPath(hdc);

		nX_note = 776;
		nY_note = 350 + 25;
		BeginPath(hdc);
		SetDCPenColor(hdc, RGB(204, 0, 204)); // tím mộng mơ //chú thích 6
		SetDCBrushColor(hdc, RGB(204, 0, 204));
		SelectObject(hdc, GetStockObject(DC_BRUSH));
		SelectObject(hdc, GetStockObject(DC_PEN));
		MoveToEx(hdc, nX_note, nY_note, (LPPOINT)NULL);
		AngleArc(hdc, nX_note, nY_note, dwRadius_note, xStartAngle_note, xSweepAngle_note);
		LineTo(hdc, nX_note, nY_note);
		EndPath(hdc);
		StrokeAndFillPath(hdc);

		nX_note = 776;
		nY_note = 395 + 25;
		BeginPath(hdc);
		SetDCPenColor(hdc, RGB(255, 153, 204)); // màu đỏ //chú thích 7
		SetDCBrushColor(hdc, RGB(255, 153, 204));
		SelectObject(hdc, GetStockObject(DC_BRUSH));
		SelectObject(hdc, GetStockObject(DC_PEN));
		MoveToEx(hdc, nX_note, nY_note, (LPPOINT)NULL);
		AngleArc(hdc, nX_note, nY_note, dwRadius_note, xStartAngle_note, xSweepAngle_note);
		LineTo(hdc, nX_note, nY_note);
		EndPath(hdc);
		StrokeAndFillPath(hdc);

			// TODO: Add any drawing code that uses hdc here...
		ReleaseDC(hWnd, hdc);
        EndPaint(hWnd, &ps);
			
        }
        break;
    case WM_DESTROY:
		//Lấy dữ liệu trên list view rồi ghi lại qua file
		GhiFile(n, ListView);
		

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
