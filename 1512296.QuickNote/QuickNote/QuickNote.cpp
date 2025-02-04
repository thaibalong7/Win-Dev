// QuickNote.cpp : Defines the entry point for the application.
/*---Thái Bá Long---*/
/*-----1512296------*/

#include "stdafx.h"
#include "QuickNote.h"

#define MAX_LOADSTRING 100
const wstring endline = L"գիծը"; //ký tự ngăn cách mỗi dòng
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

wstring rootFile = L"root";						// save subfile's name
vector<wstring>listFile;						// list of tag
vector<vector<wstring>> listData;				// list of note
vector<int> listFileUpdate;						// list of position tag have had changed


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
//Khai báo hàm sử lý chính //cũng là cửa sổ thêm note
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//DialogBox để xem các Note
INT_PTR CALLBACK	ViewNoteProc(HWND, UINT, WPARAM, LPARAM );
//SubClass để vẽ biểu đồ
LRESULT CALLBACK	StatisticsProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

//Xử lý Hook toàn cục //nguồn: code demo của ông thầy
void installHook(HWND hwnd);
void removeHook(HWND hwnd);

//Cài đặt NotificationIcon, ẩn chương trình ở vùng thông báo
/*Nguồn: https://www.codeproject.com/Articles/4768/Basic-use-of-Shell-NotifyIcon-in-Win?msg=998579 */
// Forward declarations of functions included in this code module:
NOTIFYICONDATA		niData;	// notify icon data
BOOL				InitInstance(HINSTANCE, int);
BOOL				OnInitDialog(HWND hWnd);
void				ShowContextMenu(HWND hWnd);
ULONGLONG			GetDllVersion(LPCTSTR lpszDllName);

bool writeFileUtf8(vector<wstring>, std::wstring, const int& flag);
void readFileUtf8(wstring, vector<wstring> &listFile);
int pushData2List(vector<wstring>, vector<vector<wstring>> &listData);
////ĐỌc ghi file utf8
//int readFileUtf8(wstring filename, vector<wstring> &list);
//int writeFileUft8(wstring filename, vector<wstring> &list);
//
////Dữ liệu tổng thể của một Tag
//struct Tag {
//	wstring nameTag;
//	int numNote;
//	vector<wstring> listNote;
//};
////tạo một Tag mới
//Tag createTag(wstring nameTag, int numNote, vector<wstring> listNote)
//{
//	Tag temp;
//	temp.nameTag = nameTag;
//	temp.numNote = numNote;
//	temp.listNote = listNote;
//	return temp;
//}


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
    LoadStringW(hInstance, IDC_QUICKNOTE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_QUICKNOTE));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QUICKNOTE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_3DFACE+1);
	wcex.lpszMenuName	= NULL;//MAKEINTRESOURCEW(IDC_QUICKNOTE);
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
//Handle của DialogBox xem Notes
HWND hViewNote;
//Handle chính, thêm Notes
HWND hAddNote;
//Handle của SubClass Statistic
HWND hStatistic;

BOOL OnInitDialog(HWND hWnd)
{
	HMENU hMenu = GetSystemMenu(hWnd, FALSE);
	if (hMenu)
	{
		AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
		AppendMenu(hMenu, MF_STRING, IDM_ABOUT, _T("About"));
	}
	HICON hIcon = (HICON)LoadImage(hInst,
		MAKEINTRESOURCE(IDI_QUICKNOTE),
		IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE);
	SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	return TRUE;
}

// Name says it all
void ShowContextMenu(HWND hWnd)
{
	POINT pt;
	GetCursorPos(&pt);
	HMENU hMenu = CreatePopupMenu();
	if (hMenu)
	{
		InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_VIEWNOTE, _T("View Note"));
		InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_STATISTIC, _T("View Statistic"));
		InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_EXIT, _T("Exit"));

		// note:	must set window to the foreground or the
		//			menu won't disappear when it should
		SetForegroundWindow(hWnd);

		TrackPopupMenu(hMenu, TPM_BOTTOMALIGN,
			pt.x, pt.y, 0, hWnd, NULL);
		DestroyMenu(hMenu);
	}
}

// Get dll version number
ULONGLONG GetDllVersion(LPCTSTR lpszDllName)
{
	ULONGLONG ullVersion = 0;
	HINSTANCE hinstDll;
	hinstDll = LoadLibrary(lpszDllName);
	if (hinstDll)
	{
		DLLGETVERSIONPROC pDllGetVersion;
		pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hinstDll, "DllGetVersion");
		if (pDllGetVersion)
		{
			DLLVERSIONINFO dvi;
			HRESULT hr;
			ZeroMemory(&dvi, sizeof(dvi));
			dvi.cbSize = sizeof(dvi);
			hr = (*pDllGetVersion)(&dvi);
			if (SUCCEEDED(hr))
				ullVersion = MAKEDLLVERULL(dvi.dwMajorVersion, dvi.dwMinorVersion, 0, 0);
		}
		FreeLibrary(hinstDll);
	}
	return ullVersion;
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hAddNote = CreateWindowW(szWindowClass, szTitle, 
	   WS_OVERLAPPEDWINDOW, 
	   200, 130, 450, 425, nullptr, nullptr, hInstance, nullptr);
   hStatistic = CreateWindowEx(0, szWindowClass, L"Quick Note - Statistic",
	   WS_OVERLAPPEDWINDOW, 
	   150, 100, 1000, 560, nullptr, nullptr, hInst, nullptr);
   if (!hAddNote)
   {
      return FALSE;
   }

   // Fill the NOTIFYICONDATA structure and call Shell_NotifyIcon

   // zero the structure - note:	Some Windows funtions require this but
   //								I can't be bothered which ones do and
   //								which ones don't.
   ZeroMemory(&niData, sizeof(NOTIFYICONDATA));

   // get Shell32 version number and set the size of the structure
   //		note:	the MSDN documentation about this is a little
   //				dubious and I'm not at all sure if the method
   //				bellow is correct
   ULONGLONG ullVersion = GetDllVersion(_T("Shell32.dll"));
   if (ullVersion >= MAKEDLLVERULL(5, 0, 0, 0))
	   niData.cbSize = sizeof(NOTIFYICONDATA);
   else niData.cbSize = NOTIFYICONDATA_V2_SIZE;

   // the ID number can be anything you choose
   niData.uID = TRAYICONID;

   // state which structure members are valid
   niData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;

   // load the icon
   niData.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_QUICKNOTE),
	   IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),
	   LR_DEFAULTCOLOR);

   // the window to send messages to and the message to send
   //		note:	the message value should be in the
   //				range of WM_APP through 0xBFFF
   niData.hWnd = hAddNote;
   niData.uCallbackMessage = SWM_TRAYMSG;

   // tooltip message
   lstrcpyn(niData.szTip, _T("Quick Note by Long"), sizeof(niData.szTip) / sizeof(TCHAR));

   Shell_NotifyIcon(NIM_ADD, &niData);

   // free icon handle
   if (niData.hIcon && DestroyIcon(niData.hIcon))
	   niData.hIcon = NULL;

   // call ShowWindow here to make the dialog initially visible


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

INT_PTR CALLBACK ViewNoteProc(HWND hViewNote, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		// Add items to list. 
		HWND listTags = GetDlgItem(hViewNote, IDC_TAGLIST);
		int sizeFile = listFile.size();
		for (int i = 0; i < sizeFile; i++)
		{
			int pos = (int)SendMessage(listTags, LB_ADDSTRING, 0,
				(LPARAM)listFile[i].c_str());
			// Set the array index of the player as item data.
			// This enables us to retrieve the item from the array
			// even after the items are sorted by the list box.
			SendMessage(listTags, LB_SETITEMDATA, pos, (LPARAM)i);
		}
		// Set input focus to the list box.
		SetFocus(listTags);
		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(hViewNote, LOWORD(wParam));
			return TRUE;
		case IDC_TAGLIST:
		{
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
			{
				HWND hwndList = GetDlgItem(hViewNote, IDC_TAGLIST);
				// Get selected index.
				int lbItem = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
				// Get item data.
				if (lbItem <= listFile.size())
				{

					int i = (int)SendMessage(hwndList, LB_GETITEMDATA, lbItem, 0);
					HWND listNotes = GetDlgItem(hViewNote, IDC_NOTELIST);
					int count = SendMessage(listNotes, LB_GETCOUNT, 0, 0);
					// xoa cac item co san trong list box
					SendMessage(listNotes, LB_RESETCONTENT, 0, i);
					int sizeData = listData[i].size();
					for (int j = 0; j < sizeData; j++)
					{
						int pos = (int)SendMessage(listNotes, LB_ADDSTRING, 0,
							(LPARAM)listData[i][j].c_str());
						// Set the array index of the player as item data.
						// This enables us to retrieve the item from the array
						// even after the items are sorted by the list box.
						SendMessage(listNotes, LB_SETITEMDATA, pos, (LPARAM)i);
					}
					// Set input focus to the list box.
					SetFocus(listNotes);
				}
				return TRUE;
			}
			}
		}
		return TRUE;
		case IDC_NOTELIST:
		{
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
			{
				// get handle
				HWND listNotes = GetDlgItem(hViewNote, IDC_NOTELIST);
				// get index of selected item
				int pos = SendMessage(listNotes, LB_GETCURSEL, 0, 0);
				// get data item lenth 
				int lenth = SendMessage(listNotes, LB_GETTEXTLEN, pos, 0);
				// show the content to message box
				TCHAR* buffer = new TCHAR[lenth + 1];
				SendMessage(listNotes, LB_GETTEXT, pos, (LPARAM)buffer);
				MessageBox(0, buffer, L"Note Content", 0);
			}
			}
		}
		}
	}
	return (INT_PTR)FALSE;
}

#define CHIEUDAIMOTDONVIPHANTRAM 4
int rColor = 255;// , 0, 0, 255, 0, 255 }; màu của cột trừ đi 10
int gColor = 100;// , 255, 0, 255, 255, 0 }; màu của cột cộng thêm 10
int bColor = 50;// , 0, 255, 0, 255, 255 }; màu mỗi cột cộng thêm 10
int  x1Rect = 50;// , 650, 730, 810, 890, 970 }; cộng thêm 80
float yRect = 440;
int  x2Rect = 110;// , 710, 790, 870, 950, 1030 }; mỗi cột rộng 60


LRESULT CALLBACK StatisticsProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);
	HFONT hFont;// get system font
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	hFont = CreateFont(lf.lfHeight - 4, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		// Vẽ biểu đồ ở đây

		// duong ke ngang
		MoveToEx(hdc, 10, 440, NULL);
		LineTo(hdc, 80 + 80 * listFile.size(), 440);
		HWND title = CreateWindowEx(0, L"STATIC", L"tags", WS_CHILD | WS_VISIBLE | SS_CENTER,
			50 + 80 * listFile.size(), 445, 30, 20, hStatistic, NULL, hInst, NULL);
		SendMessage(title, WM_SETFONT, WPARAM(hFont), TRUE);
		// duong ke doc
		MoveToEx(hdc, 30, 460, NULL);
		LineTo(hdc, 30, 30);
		title = CreateWindowEx(0, L"STATIC", L"(%)", WS_CHILD | WS_VISIBLE | SS_CENTER,
			35, 30, 30, 20, hStatistic, NULL, hInst, NULL);
		SendMessage(title, WM_SETFONT, WPARAM(hFont), TRUE);

		int amountTag = listFile.size();
		int amountNote = 0;

		for (int i = 0; i < amountTag; ++i)
			amountNote += listData[i].size();//tính tổng số lượng notes
		wstring wstr1 = L"Tags : ";
		wstr1 += std::to_wstring(amountTag);

		HWND tmp = CreateWindowEx(0, L"STATIC", (LPCWSTR)wstr1.c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER,
			20, 485, 60, 20, hStatistic, NULL, hInst, NULL);
		SendMessage(tmp, WM_SETFONT, WPARAM(hFont), TRUE);
		wstring wstr = L"Notes : ";
		wstr += std::to_wstring(amountNote);
		tmp = CreateWindowEx(0, L"STATIC", (LPCWSTR)wstr.c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER,
			100, 485, 60, 20, hStatistic, NULL, hInst, NULL);
		SendMessage(tmp, WM_SETFONT, WPARAM(hFont), TRUE);
		for (int i = 0; i < amountTag; ++i)
		{

			float percent = listData[i].size()*1. / amountNote * 100;// tính tỉ lệ phần trăm
			float height = percent*CHIEUDAIMOTDONVIPHANTRAM;// chiều cao cột đồ thị						
			SetBkColor(hdc, RGB(255, 0, 0));
			SelectObject(hdc, CreateSolidBrush(RGB(rColor - i * 20, gColor + i * 20, bColor + i * 20)));
			Rectangle(hdc, x1Rect + i * 80, yRect + 1, x2Rect + i * 80, yRect - height);
			HWND title = CreateWindowEx(0, L"STATIC", (LPCWSTR)listFile[i].c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER,
				x1Rect + i * 80, yRect + 5, 60, 20, hStatistic, NULL, hInst, NULL);
			SendMessage(title, WM_SETFONT, WPARAM(hFont), TRUE);

			wstring s = std::to_wstring(percent);
			s.resize(5);
			s += L" %";
			HWND per = CreateWindowEx(0, L"STATIC", (LPCWSTR)s.c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER,
				x1Rect + i * 80, yRect - height - 25, 60, 20, hStatistic, NULL, hInst, NULL);
			SendMessage(per, WM_SETFONT, WPARAM(hFont), TRUE);
		}
		EndPaint(hViewNote, &ps);
	} break;
	default:
		return DefSubclassProc(hWnd, message, wParam, lParam);
	}
	return DefSubclassProc(hWnd, message, wParam, lParam);
}
//lấy font hệ thống
HFONT hFont;
//lấy kích thước cửa sổ
RECT rect;
//dài rộng của cửa sổ
int cx, cy;
//Handle ở AddNote
HWND txtNote, txtTag;

//cờ để các control không được create lần 2 ở SubClass...
bool flag = 1; 
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (GetWindowRect(hWnd, &rect))
	{
		cx = rect.right - rect.left;
		cy = rect.bottom - rect.top;
	}
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);
	//Lấy font hệ thống
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	hFont = CreateFont(lf.lfHeight - 4, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

    switch (message)
    {
	case SWM_TRAYMSG:
		{
				switch (lParam)
				{
						case WM_LBUTTONDBLCLK:
						{
							if ((wParam) != IDC_QUICKNOTE)
								ShowWindow(hAddNote, SW_SHOW);
							else
								ShowWindow(hAddNote, SW_HIDE);
						} break;
						case WM_RBUTTONDOWN:
							ShowContextMenu(hAddNote);
							break;
				}
		} break;
	case WM_CREATE:
		{
		if (1 == flag)
		{
			HWND hwnd = CreateWindowEx(0, L"STATIC", L"Note: ", WS_CHILD | WS_VISIBLE,
				cx - 430, 5, 50, 20, hWnd, NULL, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			txtNote = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | SS_LEFT | SS_LEFT | DT_TABSTOP,
				cx - 430, 30, cx - 55, 250, hWnd, NULL, hInst, NULL);
			SendMessage(txtNote, WM_SETFONT, WPARAM(hFont), TRUE);

			hwnd = CreateWindowEx(0, L"STATIC", L"Tag: ", WS_CHILD | WS_VISIBLE,
				cx - 430, 290, 40, 20, hWnd, NULL, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			txtTag = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | SS_LEFT | SS_LEFT | DT_TABSTOP,
				cx - 390, 290, cx - 95, 20, hWnd, NULL, hInst, NULL);
			SendMessage(txtTag, WM_SETFONT, WPARAM(hFont), TRUE);

			hwnd = CreateWindowEx(0, L"BUTTON", L"SAVE", WS_VISIBLE | WS_CHILD,
				cx - 430, 320, cx - 55, 25, hWnd, (HMENU)IDC_SAVE, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			readFileUtf8(rootFile, listFile);// doc ten cac tag tu rootfile
			pushData2List(listFile, listData);// doc du lieu tu cac file vao list Data	

			flag = 0;
			installHook(hWnd);
		}
		} break;
	case WM_COMMAND:
		   {
				int wmId = LOWORD(wParam);
				// Parse the menu selections:
				switch (wmId)
				{
					//nút SAVE: lưu tag và note
				case IDC_SAVE:
				{
					{
						TCHAR lpstrFileName[MAX_PATH] = TEXT("\0");
						GetWindowText(txtTag, lpstrFileName, MAX_PATH - 1);
						// xac dinh cac tag duoc them vao
						if (lpstrFileName[0] != '\0')
						{
							// xác định các tag được thêm notes và đánh dấu các file chưa tags có thêm note để chỉnh sửa file
							wchar_t* buffer;
							// tách thành từng chuỗi tag riêng biệt
							wchar_t* token = wcstok_s(lpstrFileName, L",", &buffer);
							while (token)
							{
								// tim tags duoc them, da co trong danh sach tags ban dau hay chua
								vector<wstring>::iterator it = find(listFile.begin(), listFile.end(), token);
								// neu da co thi danh dau lai vi tri file trong listFile
								if (it != listFile.end())
								{
									listFileUpdate.push_back(it - listFile.begin());
								}
								else
								{
									listFile.push_back(token);	 // neu chua thi them vao danh sach tag
									vector<wstring> tmp;
									listData.push_back(tmp);					  // dat cho cho tag moi
									listFileUpdate.push_back(listFile.size() - 1);// va danh dau tai 
																				  // vi tri cuoi cung cua listFile
								}
								// tach thanh tung chuoi tag rieng biet
								token = wcstok_s(nullptr, L",", &buffer);
							}
							// thao tac them note vao file chua moi tag
							WORD dwTextLen = 0, bytesWritten = 0;
							TCHAR *wszEditText = NULL;// luu noi dung note
													  //lay do dai cua chuoi note
							dwTextLen = GetWindowTextLength(txtNote);
							if (dwTextLen > 0)
							{
								wszEditText = (TCHAR*)malloc((dwTextLen + 1) * sizeof(TCHAR));
								memset(wszEditText, 0, (dwTextLen + 1) * sizeof(TCHAR));
								//luu noi dung note vao wszEditText
								GetWindowText(txtNote, wszEditText, dwTextLen + 1);
								// ghi vao file
								// luu lai note vao moi file tag
								for (int index : listFileUpdate)
								{
									vector<wstring> tmp2;
									tmp2.push_back(wszEditText);
									listData[index].push_back(wszEditText);
									writeFileUtf8(tmp2, (wstring)listFile[index], 1);// viet xuong file luu moi tag
								}
								// luu lai thong tin ten cac note vao file root									
								writeFileUtf8(listFile, rootFile, 0);
							}
							SetWindowText(txtNote, L"");
							SetWindowText(txtTag, L"");
						}
					}
					break;

				}break;
				case ID_ADDNOTE:
				{
					ShowWindow(hAddNote, SW_SHOW);
					break;
				}
				case SWM_VIEWNOTE:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_VIEW_NOTE), hViewNote, ViewNoteProc);
					break;
				case SWM_STATISTIC:
				{
					SetWindowSubclass(hStatistic, StatisticsProc, ID_VICTIM, NULL);
					ShowWindow(hStatistic, SW_SHOW);
					
				}break;
				case SWM_EXIT: 
					DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
				}
			}
			break;
	case WM_INITDIALOG:
			return OnInitDialog(hWnd);
	case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				// TODO: Add any drawing code that uses hdc here...
				EndPaint(hWnd, &ps);
			}
			break;
	case WM_CLOSE:
			ShowWindow(hWnd, HIDE_WINDOW);
			break;
	case WM_DESTROY:
			removeHook(hWnd);
			PostQuitMessage(0);
			break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.


void installHook(HWND hwnd)
{
	// gọi hàm DLL theo kiểu Run-time
	// Định nghĩa prototype của hàm
	typedef VOID(*MYPROC)(HWND);

	HINSTANCE hinstLib;
	MYPROC ProcAddr;
	// load DLL và lấy handle của DLL module
	hinstLib = LoadLibrary(L"hookDLL.dll");
	// Nếu load thành công, lấy địa chỉ của hàm installhook trong DLL

	if (hinstLib != NULL)
	{
		ProcAddr = (MYPROC)GetProcAddress(hinstLib, "?installHook@@YAXPAUHWND__@@@Z");
		// Nếu lấy được địa chỉ hàm, gọi thực hiện hàm
		if (ProcAddr != NULL)
			ProcAddr(hwnd);
	}
}

void removeHook(HWND hwnd)
{
	// gọi hàm DLL theo kiểu Run-time
	// Định nghĩa prototype của hàm
	typedef VOID(*MYPROC)(HWND);

	HINSTANCE hinstLib;
	MYPROC ProcAddr;
	// load DLL và lấy handle của DLL module
	hinstLib = LoadLibrary(L"hookDLL.dll");
	if (hinstLib != NULL)
	{
		ProcAddr = (MYPROC)GetProcAddress(hinstLib, "?removeHook@@YAXPAUHWND__@@@Z");
		// Nếu lấy được địa chỉ hàm, gọi thực hiện hàm
		if (ProcAddr != NULL)
			ProcAddr(hwnd);
	}
}
//đọc file utf8
bool writeFileUtf8(vector<wstring> input, std::wstring filename, const int& flag)
{
	std::ofstream utf8file;
	if (flag == 1)
		utf8file.open(filename, ios::app);
	if (flag == 0)
		utf8file.open(filename);
	std::wbuffer_convert<std::codecvt_utf8<wchar_t>> converter(utf8file.rdbuf());
	std::wostream out(&converter);
	for (wstring buff : input)
	{
		out << buff;
		out << endline;
	}
	utf8file.close();
	return true;
}
void readFileUtf8(wstring filename, vector<wstring> &listFile)
{
	//read the files contents into a buffer							 
	wifstream wif(filename);
	if (wif)
	{
		wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
		std::wstringstream wss;
		wss << wif.rdbuf();
		wstring s = wss.str();

		wchar_t* buffer;

		wchar_t* token = wcstok_s((TCHAR*)s.c_str(), (TCHAR*)endline.c_str(), &buffer);
		while (token)
		{
			listFile.push_back(token);
			token = wcstok_s(nullptr, (TCHAR*)endline.c_str(), &buffer);
		}

	}
}


int pushData2List(vector<wstring> listFile, vector<vector<wstring>> &listData)
{
	for (wstring ws : listFile)
	{
		// doc cac note va add vao listData tuong ung voi moi lop
		vector<wstring> tmp;
		readFileUtf8(ws, tmp);
		listData.push_back(tmp);
	}
	return 0;
}

