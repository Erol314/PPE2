
#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <ctime>
#include <string>
#include "resource.h"

// Defining the ID values of the windows
#define IDC_LISTBOX1 101
#define IDC_BUTTON1 102
#define IDC_BUTTON2 103
#define IDC_SCROLLBAR1 104
#define IDC_SCROLLBAR2 105
#define IDM_MENUITEM1 106
#define IDM_MENUITEM2 107
#define IDM_MENUITEM3 108
#define IDC_TEXTBOX1 109
#define HK_EXIT 110
#define HK_ABOUT 111
#define IDC_HSCROLLBAR 112
#define IDC_VSCROLLBAR 113


// Declare Windows procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HBRUSH backColor = CreateSolidBrush(RGB(25, 25, 112));
COLORREF listColor = RGB(80, 80, 80);
HCURSOR cursor;


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
	cursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"VS Windows App";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground = backColor;
	wc.hCursor = cursor;

	RegisterClass(&wc);

	// Create the window.
	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Lab #2 PPE",		// Window text
		WS_OVERLAPPEDWINDOW,            // Window style

										// Position and size
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL) {
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	RECT rect;
	static HWND button1, button2, textBox1, scrollBar1, listBox1, label1, hScrollBar, vScrollBar;
	HGDIOBJ labelFont;
	int width, height;
	static int oldVScrollVal = 600, oldHScrollVal = 800, oldColorValue = 80;

	switch (uMsg) {
	case WM_CREATE: {
		HMENU hMenubar = CreateMenu();
		HMENU hFile = CreateMenu();
		HMENU hEdit = CreateMenu();

		AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hFile, L"File");
		AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hEdit, L"Visual Settings");
		AppendMenu(hMenubar, MF_POPUP, IDM_MENUITEM3, L"Info");

		AppendMenu(hFile, MF_STRING, IDM_MENUITEM1, L"Shut Down");
		AppendMenu(hEdit, MF_STRING, IDM_MENUITEM2, L"Random background Color");

		SetMenu(hwnd, hMenubar);

		listBox1 = CreateWindowEx(
			WS_EX_CLIENTEDGE, L"LISTBOX", L"",
			WS_VISIBLE | WS_CHILD | LBS_NOTIFY,
			0, 0, 0, 0,
			hwnd,
			(HMENU)IDC_LISTBOX1,
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL
		);
		button2 = CreateWindow(
			L"BUTTON", L"New Item",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			0, 0, 0, 0,
			hwnd,
			(HMENU)IDC_BUTTON2, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL);
		textBox1 = CreateWindowEx(
			WS_EX_CLIENTEDGE, L"EDIT", L"",
			WS_VISIBLE | WS_CHILD,
			0, 0, 0, 0,
			hwnd,
			(HMENU)IDC_TEXTBOX1, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL);
		button1 = CreateWindow(
			L"BUTTON", L"The Date",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			0, 0, 0, 0,
			hwnd,		// Parent window
			(HMENU)IDC_BUTTON1,
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL		// Pointer not needed.
		);
		scrollBar1 = CreateWindow(
			L"SCROLLBAR", L"",
			WS_VISIBLE | WS_CHILD | SBS_HORZ,
			0, 0, 0, 0,
			hwnd,
			(HMENU)IDC_SCROLLBAR1,
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL
		);
		hScrollBar = CreateWindow(
			L"SCROLLBAR", L"",
			WS_VISIBLE | WS_CHILD | SBS_HORZ,
			0, 0, 0, 0,
			hwnd,
			(HMENU)IDC_HSCROLLBAR,
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL
		);
		vScrollBar = CreateWindow(
			L"SCROLLBAR", L"",
			WS_VISIBLE | WS_CHILD | SBS_VERT,
			0, 0, 0, 0,
			hwnd,
			(HMENU)IDC_VSCROLLBAR,
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL
		);
		label1 = CreateWindow(
			L"STATIC", L"Select and right click to Delete.",
			WS_VISIBLE | WS_CHILD | SS_SIMPLE,
			0, 0, 0, 0,
			hwnd,
			NULL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL);

		SetScrollRange(scrollBar1, SB_CTL, 0, 255, FALSE);
		SetScrollPos(scrollBar1, SB_CTL, 10, TRUE);
		SetScrollRange(hScrollBar, SB_CTL, 400, 800, FALSE);
		SetScrollPos(hScrollBar, SB_CTL, 800, TRUE);
		SetScrollRange(vScrollBar, SB_CTL, 360, 600, FALSE);
		SetScrollPos(vScrollBar, SB_CTL, 600, TRUE);
		SetScrollRange(hwnd, SB_CTL, 0, 800, FALSE);

		SendMessage(listBox1, LB_ADDSTRING, 0, (LPARAM)L"Reinstall.");
		SendMessage(listBox1, LB_ADDSTRING, 0, (LPARAM)L"Verify Disk.");
		SendMessage(listBox1, LB_ADDSTRING, 0, (LPARAM)L"Check for Updates.");
		SendMessage(listBox1, LB_ADDSTRING, 0, (LPARAM)L"Buy Licence.");
		SendMessage(listBox1, LB_ADDSTRING, 0, (LPARAM)L"Show Release Notes.");

		RegisterHotKey(hwnd, HK_EXIT, MOD_ALT, 'N');
		RegisterHotKey(hwnd, HK_ABOUT, MOD_ALT, 'L');

		HANDLE icon = LoadImage((HINSTANCE)hwnd, L"icon.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
		SendMessage(hwnd, (UINT)WM_SETICON, ICON_BIG, (LPARAM)icon);

		// === FONT ===


		labelFont= CreateFont(20, 0, 0, 0, FW_LIGHT, false, false, false,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, FF_DONTCARE, L"Helvetica");
		SendMessage(button1, WM_SETFONT, WPARAM(labelFont), TRUE);
		SendMessage(button2, WM_SETFONT, WPARAM(labelFont), TRUE);
		SendMessage(textBox1, WM_SETFONT, WPARAM(labelFont), TRUE);
		SendMessage(listBox1, WM_SETFONT, WPARAM(labelFont), TRUE);
		SendMessage(label1, WM_SETFONT, WPARAM(labelFont), TRUE);
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, backColor);

		EndPaint(hwnd, &ps);
		break;
	}
	case WM_SETCURSOR: {
		if (LOWORD(lParam) == HTCLIENT) {
			SetCursor(cursor); 
			return TRUE;
		}
		break;
	}
	case WM_GETMINMAXINFO: {
		LPMINMAXINFO winSize = (LPMINMAXINFO)lParam;
		winSize->ptMinTrackSize.x = 420;
		winSize->ptMinTrackSize.y = 400;
		winSize->ptMaxTrackSize.x = 800;
		winSize->ptMaxTrackSize.y = 600;
		break;
	}
	case WM_CONTEXTMENU: {
		if ((HWND)wParam == listBox1) {
			if (MessageBox(hwnd, L"Do you want to remove the selected item?", L"Alert", MB_YESNO) == IDYES) {
				int index = SendMessage(GetDlgItem(hwnd, IDC_LISTBOX1), LB_GETCURSEL, 0, 0);
				SendMessage(GetDlgItem(hwnd, IDC_LISTBOX1), LB_DELETESTRING, index, 0);
			}
		}
		break;
	}
	case WM_HOTKEY: {
		switch (wParam) {
		case HK_EXIT: {
			PostQuitMessage(0);
			break;
		}
		case HK_ABOUT: {
			MessageBox(hwnd, L"Program By Sezgin ERol.", L"About", MB_ICONINFORMATION);
			break;
		}
		default:
			break;
		}
		break;
	}
	case WM_VSCROLL: {
		if ((HWND)lParam == vScrollBar) {
			auto action = LOWORD(wParam);
			HWND vScroll = (HWND)lParam;
			int pos = -1;
			if (action == SB_THUMBPOSITION || action == SB_THUMBTRACK) {
				pos = HIWORD(wParam);
			}
			else if (action == SB_LINEDOWN) {
				pos = oldVScrollVal + 30;
			}
			else if (action == SB_LINEUP) {
				pos = oldVScrollVal - 30;
			}
			if (pos == -1)
				break;
			SCROLLINFO si = { 0 };
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_POS;
			si.nPos = pos;
			si.nTrackPos = 0;
			SetScrollInfo(hwnd, SB_VERT, &si, true);
			GetScrollInfo(hwnd, SB_VERT, &si);
			pos = si.nPos;
			SetScrollPos(vScrollBar, SB_CTL, pos, TRUE);
			GetWindowRect(hwnd, &rect);
			MoveWindow(hwnd, rect.left, rect.top, rect.right - rect.left, pos, TRUE);

			oldVScrollVal = pos;
			break;
		}
	}
	case WM_HSCROLL: {
		if ((HWND)lParam == scrollBar1) {
			int pos = -1;
			if (LOWORD(wParam) == SB_THUMBPOSITION || LOWORD(wParam) == SB_THUMBTRACK) {
				pos = HIWORD(wParam);
			}
			else if (LOWORD(wParam) == SB_LINELEFT) {
				pos = oldColorValue - 30;
				if (pos < 0)
					pos = 0;
			}
			else if (LOWORD(wParam) == SB_LINERIGHT) {
				pos = oldColorValue + 30;
				if (pos > 255)
					pos = 255;
			}
			if (pos == -1)
				break;
			SCROLLINFO si = { 0 };
			si.nPos = pos;
			SetScrollInfo(scrollBar1, SB_HORZ, &si, TRUE);
			GetScrollInfo(scrollBar1, SB_HORZ, &si);
			pos = si.nPos;
			SetScrollPos(scrollBar1, SB_CTL, pos, TRUE);
			oldColorValue = pos;

			listColor = RGB(pos, pos, pos);
			RedrawWindow(listBox1, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
			break;
		} else if ((HWND)lParam == hScrollBar) {
			auto action = LOWORD(wParam);
			HWND vScroll = (HWND)lParam;
			int pos = -1;
			if (action == SB_THUMBPOSITION || action == SB_THUMBTRACK) {
				pos = HIWORD(wParam);
			}
			else if (action == SB_LINEDOWN) {
				pos = oldHScrollVal + 30;
			}
			else if (action == SB_LINEUP) {
				pos = oldHScrollVal - 30;
			}
			if (pos == -1)
				break;
			SCROLLINFO si = { 0 };
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_POS;
			si.nPos = pos;
			si.nTrackPos = 0;
			SetScrollInfo(hwnd, SB_VERT, &si, true);
			GetScrollInfo(hwnd, SB_VERT, &si);
			pos = si.nPos;
			SetScrollPos(hScrollBar, SB_CTL, pos, TRUE);
			GetWindowRect(hwnd, &rect);
			MoveWindow(hwnd, rect.left, rect.top, pos, rect.bottom - rect.top, TRUE);
			oldHScrollVal = pos;
			break;
		}
		break;
	}
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1: {
			time_t tm = time(0);
			struct tm now;
			localtime_s(&now, &tm);
			std::wstring date = std::to_wstring(now.tm_mday);
			if (date.length() == 1) date = L"0" + date;
			date += L".";
			if (std::to_wstring(now.tm_mon).length() == 1) date += L"0";
			date += std::to_wstring(now.tm_mon);
			date += L".";
			date += std::to_wstring(now.tm_year + 1900);
			MessageBoxW(hwnd, date.c_str(), L"Date", NULL);
			break;
		}
		case IDC_BUTTON2: {
			int length = SendMessage(textBox1, WM_GETTEXTLENGTH, NULL, NULL);
			if (length == 0)
				break;
			TCHAR txt[256];
			GetWindowText(textBox1, txt, 256);
			SendMessage(listBox1, LB_ADDSTRING, 0, (LPARAM)txt);
			break;
		}
		case IDM_MENUITEM1: {
			PostQuitMessage(0);
			break;
		}
		case IDM_MENUITEM2: {
			backColor = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
			RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
			break;
		}
		case IDM_MENUITEM3: {
			MessageBox(hwnd, L"Program By Sezgin Erol.", L"Info", MB_ICONINFORMATION);
			break;
		}
		}
		break;
	}
	case WM_SYSCOMMAND: {
		switch (wParam & 0xFFF0) {
		case SC_MAXIMIZE: {
			return 0;
		}
		case SC_MINIMIZE: {
			GetWindowRect(hwnd, &rect);
			int width = rect.right - rect.left,
				height = rect.bottom - rect.top;
			int newWidth = rand() % 320 + 480,
				newHeight = rand() % 240 + 360;
			int posX = rect.left + (width - newWidth) / 2,
				posY = rect.top + (height - newHeight) / 2;
			MoveWindow(hwnd, posX, posY, newWidth, newHeight, TRUE);
			return 0;
		}
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		break;
	}
	case WM_SIZE: {
		width = LOWORD(lParam);
		height = HIWORD(lParam);
		MoveWindow(textBox1, width / 2 - width / 3, height / 12, (int)(width / 1.85), 24, TRUE);
		if (height > 419) {
			MoveWindow(button1, width / 2 - width / 7, (int)(height / 1.155), (int)(width / 3.5), 34, TRUE);
		} else {
			MoveWindow(button1, width / 2 - width / 7, -50, (int)(width / 3.5), 34, TRUE);
		}
		MoveWindow(button2, width / 2 + width / 3 - (int)(width / 9), height / 12, (int)(width / 9), 24, TRUE);
		MoveWindow(scrollBar1, width / 8, (int)(height / 1.35) + 30, width - (width / 4), 20, TRUE);
		MoveWindow(listBox1, width / 2 - width / 3, height / 12 + 30, (int)(width / 1.5), (int)(height / 1.74), TRUE);
		MoveWindow(label1, width / 2 - 172, (int)(height / 1.35), 350, 17, TRUE);
		MoveWindow(hScrollBar, 0, height - 20, width - 20, 20, TRUE);
		MoveWindow(vScrollBar, width - 20, 0, 20, height - 20, TRUE);
		RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE); // To prevent overdraw
		break;
	}
	case WM_CTLCOLORSTATIC:
	{
		if ((HWND)lParam == label1) {
			SetBkMode((HDC)wParam, TRANSPARENT);
			SetTextColor((HDC)wParam, RGB(120, 120, 120));
		}
	}
	case WM_CTLCOLORLISTBOX: {
		if ((HWND)lParam == listBox1) {
			SetTextColor((HDC)wParam, listColor);
			return (INT_PTR)CreateSolidBrush(RGB(255, 255, 255));
		}
		break;
	}
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}