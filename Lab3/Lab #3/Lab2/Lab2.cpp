
#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <ctime>
#include <string>
#include "resource.h"

// Defining the ID values of the windows
#define IDC_DRAWAREA 101 
#define IDC_BUTTON1 102
#define IDB_RADIO1 103
#define IDB_RADIO2 104
#define IDB_RADIO3 105
#define IDB_RADIO4 106
#define IDB_RADIO5 107
#define IDB_RADIO6 108
#define IDB_RADIO7 109
#define IDB_CHECKBOX1 110
#define IDB_TEXTBOX1 111
#define IDB_TEXTBOX2 112
#define IDB_TEXTBOX3 113
#define IDB_TEXTBOX4 114

#define HK_EXIT 115
#define HK_ABOUT 116


// Declare Windows procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HBRUSH backColor = CreateSolidBrush(RGB(25, 25, 112));
COLORREF listColor = RGB(80, 80, 80);
HCURSOR cursor;
HBITMAP bitmapHBmp;
BITMAP bitmapBmp;


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
		L"Lab #3 PPE",		// Window text
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

	bitmapHBmp = (HBITMAP)LoadImage(hInstance, L"bitmap.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GetObject(bitmapHBmp, sizeof(bitmapBmp), &bitmapBmp);

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

void getInBounds(int* xEnd, int* yEnd, int width, int height, int penWeight) {
	if (*xEnd < penWeight / 2 + 2) // left limit
		*xEnd = penWeight / 2 + 2;
	else if (*xEnd >(width < 720 ? (int)(width / 1.3) : (width - 166)) - 3 - penWeight / 2) // right limit
		*xEnd = width < 720 ? (int)(width / 1.3) : (width - 166) - 3 - penWeight / 2;

	if (*yEnd < penWeight / 2 + 2)
		*yEnd = penWeight / 2 + 2;
	else if (*yEnd > height - penWeight / 2 - 3)
		*yEnd = height - penWeight / 2 - 3;
}

void resetTools(HWND hwnd, HPEN hpen, int xStart, int* xEnd, int yStart, int* yEnd) {
	HDC hdc = GetDC(hwnd);
	SelectObject(hdc, hpen);

	SetROP2(hdc, R2_NOTXORPEN);
	MoveToEx(hdc, xStart, yStart, NULL);
	LineTo(hdc, *xEnd, *yEnd);
	*xEnd = xStart;
	*yEnd = yStart;
	DeleteObject(hpen);
	ReleaseDC(hwnd, hdc);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	// RECT rect;
	static HWND drawArea1, button1, radio1, radio2, radio3, radio4, radio5, radio6, radio7, checkbox1, textBox1, textBox2, textBox3, textBox4, label1, colorVisual;
	HGDIOBJ customFont;
	static int	width, height,
		xStart, yStart, xEnd, yEnd;
	HDC hdc;
	HPEN hpen;
	static int penWeight = 5;
	static COLORREF penColor = RGB(92, 138, 54);

	static POINT bez[4];
	static int bezSelector = 0;

	static int mouseX, mouseY;
	static bool isDrawing = FALSE;

	switch (uMsg) {
	case WM_CREATE: {
		drawArea1 = CreateWindowEx(
			WS_EX_CLIENTEDGE, L"STATIC", L"",
			WS_VISIBLE | WS_CHILD,
			0, 0, 0, 0,
			hwnd,
			(HMENU)IDC_DRAWAREA,
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL
		);
		button1 = CreateWindowEx(
			WS_EX_CLIENTEDGE, L"BUTTON", L"Clear All",
			WS_VISIBLE | WS_CHILD,
			0, 0, 0, 0,
			hwnd,
			(HMENU)IDC_BUTTON1, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL
		);
		radio1 = CreateWindow(
			L"BUTTON", L"Pen",
			WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
			0, 0, 0, 0,
			hwnd,
			(HMENU)IDB_RADIO1, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL
		);
		radio2 = CreateWindow(
			L"BUTTON", L"Line",
			WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
			0, 0, 0, 0,
			hwnd,
			(HMENU)IDB_RADIO2, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL
		);
		radio3 = CreateWindow(
			L"BUTTON", L"Polyline",
			WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
			0, 0, 0, 0,
			hwnd,
			(HMENU)IDB_RADIO3, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL
		);
		radio4 = CreateWindow(
			L"BUTTON", L"Bezier",
			WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
			0, 0, 0, 0,
			hwnd,
			(HMENU)IDB_RADIO4, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL
		);
		radio5 = CreateWindow(
			L"BUTTON", L"Rectangle",
			WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
			0, 0, 0, 0,
			hwnd,
			(HMENU)IDB_RADIO5, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL
		);
		radio6 = CreateWindow(
			L"BUTTON", L"Ellipse",
			WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
			0, 0, 0, 0,
			hwnd,
			(HMENU)IDB_RADIO6, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL
		);
		radio7 = CreateWindow(
			L"BUTTON", L"Eraser",
			WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
			0, 0, 0, 0,
			hwnd,
			(HMENU)IDB_RADIO7, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL
		);
		checkbox1 = CreateWindow(
			L"BUTTON", L"Filled",
			WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			0, 0, 0, 0,
			hwnd,
			(HMENU)IDB_CHECKBOX1, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL
		);
		textBox1 = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"EDIT", L"5",
			WS_VISIBLE | WS_CHILD | ES_NUMBER | ES_CENTER,
			0, 0, 0, 0,
			hwnd,
			(HMENU)IDB_TEXTBOX1, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL
		);
		textBox2 = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"EDIT", L"92",
			WS_VISIBLE | WS_CHILD | ES_NUMBER | ES_CENTER,
			0, 0, 0, 0,
			hwnd,
			(HMENU)IDB_TEXTBOX2, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL
		);
		textBox3 = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"EDIT", L"138",
			WS_VISIBLE | WS_CHILD | ES_NUMBER | ES_CENTER,
			0, 0, 0, 0,
			hwnd,
			(HMENU)IDB_TEXTBOX3, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL
		);
		textBox4 = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"EDIT", L"54",
			WS_VISIBLE | WS_CHILD | ES_NUMBER | ES_CENTER,
			0, 0, 0, 0,
			hwnd,
			(HMENU)IDB_TEXTBOX4, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL
		);
		label1 = CreateWindow(
			L"STATIC", L"Brush size:",
			WS_VISIBLE | WS_CHILD | SS_SIMPLE,
			0, 0, 0, 0,
			hwnd,
			NULL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL
		);
		colorVisual = CreateWindow(
			L"STATIC", L"",
			WS_VISIBLE | WS_CHILD,
			0, 0, 0, 0,
			hwnd,
			NULL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL
		);

		SendDlgItemMessage(hwnd, IDB_RADIO1, BM_SETCHECK, 1, 0);

		RegisterHotKey(hwnd, HK_EXIT, MOD_ALT, 'N');
		RegisterHotKey(hwnd, HK_ABOUT, MOD_ALT, 'L');

		HANDLE icon = LoadImage((HINSTANCE)hwnd, L"icon.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
		SendMessage(hwnd, (UINT)WM_SETICON, ICON_BIG, (LPARAM)icon);

		bitmapHBmp = (HBITMAP)LoadImage((HINSTANCE)hwnd, L"bitmap.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		// === FONT ===

		customFont = CreateFont(20, 0, 0, 0, FW_LIGHT, false, false, false,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, FF_DONTCARE, L"Helvetica");
		// SendMessage(button1, WM_SETFONT, WPARAM(customFont), TRUE);
	}
	case WM_LBUTTONDOWN:
	{
		mouseX = LOWORD(lParam), mouseY = HIWORD(lParam);
		if (width < 720 && (mouseX > 1 && mouseX < (int)(width / 1.3 - 1) && mouseY > 1 && mouseY < height - 1) || width >= 720 && (mouseX > 1 && mouseX < width - 167 && mouseY > 1 && mouseY < height - 1)) {
			hdc = GetDC(hwnd);
			hpen = CreatePen(PS_SOLID, penWeight, penColor);
			if (SendDlgItemMessage(hwnd, IDB_RADIO3, BM_GETCHECK, 0, 0) == 1) {
				SelectObject(hdc, hpen);
				MoveToEx(hdc, xStart, yStart, NULL);
				LineTo(hdc, xEnd, yEnd);
			}

			if (SendDlgItemMessage(hwnd, IDB_RADIO4, BM_GETCHECK, 0, 0) == 1) {
				if (bezSelector == 4) {
					SelectObject(hdc, hpen);
					SetROP2(hdc, R2_NOTXORPEN);
					PolyBezier(hdc, bez, 4);
					bezSelector = 0;
					break;
				}

				if (bezSelector > 3)
					bezSelector = 0;
				getInBounds(&mouseX, &mouseY, width, height, penWeight);
				bez[bezSelector] = { mouseX, mouseY };
				bezSelector++;

				// SetROP2(hdc, R2_COPYPEN);
				if (bezSelector == 4) {
					SelectObject(hdc, hpen);
					PolyBezier(hdc, bez, 4);
				}
			}

			xStart = LOWORD(lParam);
			yStart = HIWORD(lParam);
			getInBounds(&xStart, &yStart, width, height, penWeight);
			xEnd = LOWORD(lParam);
			yEnd = HIWORD(lParam);
			getInBounds(&xEnd, &yEnd, width, height, penWeight);
			isDrawing = TRUE;

			DeleteObject(hpen);
			ReleaseDC(hwnd, hdc);
		}
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (width < 720 && (mouseX > 1 && mouseX < (int)(width / 1.3 - 1) && mouseY > 1 && mouseY < height - 1) || width >= 720 && (mouseX > 1 && mouseX < width - 167 && mouseY > 1 && mouseY < height - 1)) {
			hdc = GetDC(hwnd);
			if (isDrawing == TRUE) {
				hpen = CreatePen(PS_SOLID, penWeight, penColor);
				SelectObject(hdc, hpen);
				// SetROP2(hdc, R2_NOTXORPEN);
				if (SendDlgItemMessage(hwnd, IDB_RADIO1, BM_GETCHECK, 0, 0) == 1) {
					MoveToEx(hdc, xEnd, yEnd, NULL);
					xEnd = LOWORD(lParam);
					yEnd = HIWORD(lParam);
					getInBounds(&xEnd, &yEnd, width, height, penWeight);
					LineTo(hdc, xEnd, yEnd);
				}
				else if (SendDlgItemMessage(hwnd, IDB_RADIO2, BM_GETCHECK, 0, 0) == 1) {
					SetROP2(hdc, R2_NOTXORPEN);
					MoveToEx(hdc, xStart, yStart, NULL);
					LineTo(hdc, xEnd, yEnd);
					xEnd = LOWORD(lParam);
					yEnd = HIWORD(lParam);
					getInBounds(&xEnd, &yEnd, width, height, penWeight);
					MoveToEx(hdc, xStart, yStart, NULL);
					LineTo(hdc, xEnd, yEnd);
				}
				else if (SendDlgItemMessage(hwnd, IDB_RADIO3, BM_GETCHECK, 0, 0) == 1) {
					SetROP2(hdc, R2_NOTXORPEN);
					MoveToEx(hdc, xStart, yStart, NULL);
					LineTo(hdc, xEnd, yEnd);
					xEnd = LOWORD(lParam);
					yEnd = HIWORD(lParam);
					getInBounds(&xEnd, &yEnd, width, height, penWeight);
					MoveToEx(hdc, xStart, yStart, NULL);
					LineTo(hdc, xEnd, yEnd);
				}
				else if (SendDlgItemMessage(hwnd, IDB_RADIO5, BM_GETCHECK, 0, 0) == 1) {
					SelectObject(hdc, SendDlgItemMessage(hwnd, IDB_CHECKBOX1, BM_GETCHECK, 0, 0) == 1 ? CreateSolidBrush(penColor) : GetStockObject(NULL_BRUSH));
					SetROP2(hdc, R2_NOTXORPEN);
					Rectangle(hdc, xStart, yStart, xEnd, yEnd);
					xEnd = LOWORD(lParam);
					yEnd = HIWORD(lParam);
					getInBounds(&xEnd, &yEnd, width, height, penWeight);
					Rectangle(hdc, xStart, yStart, xEnd, yEnd);
				}
				else if (SendDlgItemMessage(hwnd, IDB_RADIO6, BM_GETCHECK, 0, 0) == 1) {
					SelectObject(hdc, SendDlgItemMessage(hwnd, IDB_CHECKBOX1, BM_GETCHECK, 0, 0) == 1 ? CreateSolidBrush(penColor) : GetStockObject(NULL_BRUSH));
					SetROP2(hdc, R2_NOTXORPEN);
					Ellipse(hdc, xStart, yStart, xEnd, yEnd);
					xEnd = LOWORD(lParam);
					yEnd = HIWORD(lParam);
					getInBounds(&xEnd, &yEnd, width, height, penWeight);
					Ellipse(hdc, xStart, yStart, xEnd, yEnd);
				}
				else if (SendDlgItemMessage(hwnd, IDB_RADIO7, BM_GETCHECK, 0, 0) == 1) {
					SelectObject(hdc, CreatePen(PS_SOLID, penWeight, RGB(255, 255, 255)));
					MoveToEx(hdc, xEnd, yEnd, NULL);
					xEnd = LOWORD(lParam);
					yEnd = HIWORD(lParam);
					getInBounds(&xEnd, &yEnd, width, height, penWeight);
					LineTo(hdc, xEnd, yEnd);
				}
				DeleteObject(hpen);
				ReleaseDC(hwnd, hdc);
			}
		}
		break;
	}
	case WM_LBUTTONUP:
	{
		hdc = GetDC(hwnd);
		hpen = CreatePen(PS_SOLID, penWeight, penColor);
		SelectObject(hdc, hpen);
		if (SendDlgItemMessage(hwnd, IDB_RADIO2, BM_GETCHECK, 0, 0) == 1) {
			MoveToEx(hdc, xStart, yStart, NULL);
			LineTo(hdc, xEnd, yEnd);
		}
		else if (SendDlgItemMessage(hwnd, IDB_RADIO5, BM_GETCHECK, 0, 0) == 1) {
			SelectObject(hdc, SendDlgItemMessage(hwnd, IDB_CHECKBOX1, BM_GETCHECK, 0, 0) == 1 ? CreateSolidBrush(penColor) : GetStockObject(NULL_BRUSH));
			Rectangle(hdc, xStart, yStart, xEnd, yEnd);
		}
		else if (SendDlgItemMessage(hwnd, IDB_RADIO6, BM_GETCHECK, 0, 0) == 1) {
			SelectObject(hdc, SendDlgItemMessage(hwnd, IDB_CHECKBOX1, BM_GETCHECK, 0, 0) == 1 ? CreateSolidBrush(penColor) : GetStockObject(NULL_BRUSH));
			Ellipse(hdc, xStart, yStart, xEnd, yEnd);
		}

		DeleteObject(hpen);
		ReleaseDC(hwnd, hdc);

		if (SendDlgItemMessage(hwnd, IDB_RADIO3, BM_GETCHECK, 0, 0) != 1)
			isDrawing = FALSE;
		break;
	}

	case WM_RBUTTONDOWN:
	{
		if (SendDlgItemMessage(hwnd, IDB_RADIO3, BM_GETCHECK, 0, 0) == 1) { // Finishing Polyline
			hdc = GetDC(hwnd);
			hpen = CreatePen(PS_SOLID, penWeight, penColor);
			SelectObject(hdc, hpen);
			MoveToEx(hdc, xStart, yStart, NULL);
			LineTo(hdc, xEnd, yEnd);
			DeleteObject(hpen);
			ReleaseDC(hwnd, hdc);
		}
		isDrawing = FALSE;
		break;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, backColor);

		HDC hdc_mem = CreateCompatibleDC(hdc);
		SelectObject(hdc_mem, bitmapHBmp);
		BitBlt(hdc, (int)((width < 720 ? (width / 1.3) : (width - 166)) + (width - (width < 720 ? (width / 1.3) : (width - 166))) / 2 - bitmapBmp.bmWidth / 2), 422, bitmapBmp.bmWidth, bitmapBmp.bmHeight, hdc_mem, 0, 0, SRCCOPY);
		DeleteObject(hdc_mem);
		SetBkMode(hdc, TRANSPARENT);

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
		winSize->ptMinTrackSize.x = 640;
		winSize->ptMinTrackSize.y = 600;
		break;
	}
	case WM_HOTKEY: {
		switch (wParam) {
		case HK_EXIT: {
			PostQuitMessage(0);
			break;
		}
		case HK_ABOUT: {
			MessageBox(hwnd, L"Program By Sezgin Erol", L"About", MB_ICONINFORMATION);
			break;
		}
		default:
			break;
		}
		break;
	}
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1: {
			RedrawWindow(drawArea1, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
			break;
		}
		case IDB_RADIO1: {
			SendDlgItemMessage(hwnd, IDB_RADIO1, BM_SETCHECK, 1, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO2, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO3, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO4, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO5, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO6, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO7, BM_SETCHECK, 0, 0);
			break;
		}
		case IDB_RADIO2: {
			SendDlgItemMessage(hwnd, IDB_RADIO1, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO2, BM_SETCHECK, 1, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO3, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO4, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO5, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO6, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO7, BM_SETCHECK, 0, 0);
			break;
		}
		case IDB_RADIO3: {
			SendDlgItemMessage(hwnd, IDB_RADIO1, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO2, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO3, BM_SETCHECK, 1, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO4, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO5, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO6, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO7, BM_SETCHECK, 0, 0);
			break;
		}
		case IDB_RADIO4: {
			SendDlgItemMessage(hwnd, IDB_RADIO1, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO2, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO3, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO4, BM_SETCHECK, 1, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO5, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO6, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO7, BM_SETCHECK, 0, 0);
			break;
		}
		case IDB_RADIO5: {
			SendDlgItemMessage(hwnd, IDB_RADIO1, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO2, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO3, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO4, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO5, BM_SETCHECK, 1, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO6, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO7, BM_SETCHECK, 0, 0);
			break;
		}
		case IDB_RADIO6: {
			SendDlgItemMessage(hwnd, IDB_RADIO1, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO2, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO3, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO4, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO5, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO6, BM_SETCHECK, 1, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO7, BM_SETCHECK, 0, 0);
			break;
		}
		case IDB_RADIO7: {
			SendDlgItemMessage(hwnd, IDB_RADIO1, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO2, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO3, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO4, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO5, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO6, BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hwnd, IDB_RADIO7, BM_SETCHECK, 1, 0);
			break;
		}
		}

		switch (HIWORD(wParam)) {
		case EN_CHANGE: {
			if ((HWND)lParam == textBox1) {
				TCHAR textStore[50];
				GetWindowText(textBox1, textStore, 50);
				if (_wtoi(textStore) > 0 && _wtoi(textStore) < 201)
					penWeight = _wtoi(textStore);
				if (lstrcmpW(textStore, L"") == 1 && _wtoi(textStore) < 1) {
					SetWindowText(textBox1, L"1");
					penWeight = 1;
				}
				else if (_wtoi(textStore) > 200) {
					SetWindowText(textBox1, L"200");
					penWeight = 200;
				}
			}
			else {
				TCHAR r[50], g[50], b[50];
				int cr, cg, cb;
				GetWindowText(textBox2, r, 50);
				GetWindowText(textBox3, g, 50);
				GetWindowText(textBox4, b, 50);
				if (lstrcmpW(r, L"") == 0 || lstrcmpW(g, L"") == 0 || lstrcmpW(b, L"") == 0) {
					return 0;
				}
				cr = _wtoi(r);
				cg = _wtoi(g);
				cb = _wtoi(b);

				if (_wtoi(r) > 255) {
					cr = 255;
					SetWindowText(textBox2, L"255");
				}
				if (_wtoi(g) > 255) {
					cg = 255;
					SetWindowText(textBox3, L"255");
				}
				if (_wtoi(b) > 255) {
					cb = 255;
					SetWindowText(textBox4, L"255");
				}
				penColor = RGB(cr, cg, cb);
				RedrawWindow(colorVisual, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
			}
			break;
		}
		}
		bezSelector = 0;
		isDrawing = FALSE;
		break;
	}
	case WM_SIZE: {
		width = LOWORD(lParam);
		height = HIWORD(lParam);
		if (width < 720) {
			MoveWindow(drawArea1, 0, 0, (int)(width / 1.3), height, TRUE);
			MoveWindow(checkbox1, (int)(width / 1.3) + width / 70, 270, width - (int)(width / 1.3) - width / 35, 24, TRUE);
			MoveWindow(textBox1, (int)(width / 1.3) + (int)(width / 6.5), 300, width - (int)(width / 1.3) - width / 6, 24, TRUE);
			MoveWindow(textBox2, (int)(width / 1.3) + (int)(width / 70), 330, width - (int)(width / 1.3) - width / 6, 24, TRUE);
			MoveWindow(textBox3, (int)(width / 1.3) + (int)(width / 12), 330, width - (int)(width / 1.3) - width / 6, 24, TRUE);
			MoveWindow(textBox4, (int)(width / 1.3) + (int)(width / 6.5), 330, width - (int)(width / 1.3) - width / 6, 24, TRUE);
			MoveWindow(label1, (int)(width / 1.3) + width / 70, 303, 80, 24, TRUE);
			MoveWindow(colorVisual, (int)(width / 1.3) + width / 70, 360, width - (int)(width / 1.3) - width / 35, 14, TRUE);
			MoveWindow(button1, (int)(width / 1.3) + width / 70, 380, width - (int)(width / 1.3) - width / 35, 30, TRUE);
			MoveWindow(radio1, (int)(width / 1.3) + width / 70, 30, width - (int)(width / 1.3) - width / 35, 30, TRUE);
			MoveWindow(radio2, (int)(width / 1.3) + width / 70, 60, width - (int)(width / 1.3) - width / 35, 30, TRUE);
			MoveWindow(radio3, (int)(width / 1.3) + width / 70, 90, width - (int)(width / 1.3) - width / 35, 30, TRUE);
			MoveWindow(radio4, (int)(width / 1.3) + width / 70, 120, width - (int)(width / 1.3) - width / 35, 30, TRUE);
			MoveWindow(radio5, (int)(width / 1.3) + width / 70, 150, width - (int)(width / 1.3) - width / 35, 30, TRUE);
			MoveWindow(radio6, (int)(width / 1.3) + width / 70, 180, width - (int)(width / 1.3) - width / 35, 30, TRUE);
			MoveWindow(radio7, (int)(width / 1.3) + width / 70, 210, width - (int)(width / 1.3) - width / 35, 30, TRUE);
		}
		else {
			MoveWindow(drawArea1, 0, 0, width - 166, height, TRUE);
			MoveWindow(checkbox1, width - 156, 270, 146, 24, TRUE);
			MoveWindow(textBox1, width - 55, 300, 46, 24, TRUE);
			MoveWindow(textBox2, width - 156, 330, 46, 24, TRUE);
			MoveWindow(textBox3, width - 106, 330, 46, 24, TRUE);
			MoveWindow(textBox4, width - 55, 330, 46, 24, TRUE);
			MoveWindow(label1, width - 156, 303, 80, 24, TRUE);
			MoveWindow(colorVisual, width - 156, 360, 147, 14, TRUE);
			MoveWindow(button1, width - 156, 380, 147, 30, TRUE);
			MoveWindow(radio1, width - 156, 30, 146, 30, TRUE);
			MoveWindow(radio2, width - 156, 60, 146, 30, TRUE);
			MoveWindow(radio3, width - 156, 90, 146, 30, TRUE);
			MoveWindow(radio4, width - 156, 120, 146, 30, TRUE);
			MoveWindow(radio5, width - 156, 150, 146, 30, TRUE);
			MoveWindow(radio6, width - 156, 180, 146, 30, TRUE);
			MoveWindow(radio7, width - 156, 210, 146, 30, TRUE);
		}
		RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE); // To prevent overdraw
		break;
	}
	case WM_CTLCOLORSTATIC:
	{
		if ((HWND)lParam == colorVisual) {
			return (INT_PTR)CreateSolidBrush(penColor);
		}
		SetBkMode((HDC)wParam, TRANSPARENT);
		SetTextColor((HDC)wParam, RGB(60, 60, 64));
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