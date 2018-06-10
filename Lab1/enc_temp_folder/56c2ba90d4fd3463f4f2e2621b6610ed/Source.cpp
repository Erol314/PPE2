#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>

// Defining the ID values of the windows
#define IDC_BUTTON1 102
#define IDC_BUTTON2 103
#define IDC_TEXT1	104
#define IDC_TEXT2	105

// Declare Windows procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HBRUSH backColor = CreateSolidBrush(RGB(213, 213, 173));


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"VS Windows App";

	WNDCLASS wndclass = {};

	wndclass.lpfnWndProc = WindowProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.lpszClassName = CLASS_NAME;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = backColor;

	RegisterClass(&wndclass);

	// Create the window.
	HWND hwnd = CreateWindow(
		//0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Event-Driven Programming Lab #1",		// Window text
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
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	RECT rect;
	static HWND button1, button2, label1, label2, textBox1, textBox2;
	HGDIOBJ defaultFont, labelFont, customFont;
	int width, height;
	static bool textEntered;

	switch (uMsg) {
	case WM_CREATE: {
		button1 = CreateWindow(
			L"BUTTON", L"Maximally Default Button [APPEND TEXT]",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			0, 0, 0, 0,
			hwnd,		// Parent window
			(HMENU)IDC_BUTTON1,
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL		// Pointer not needed.
		);
		button2 = CreateWindow(
			L"BUTTON", L"Custom Button [CLEAR TEXT]",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			0, 0, 0, 0,
			hwnd, (HMENU)IDC_BUTTON2, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL
		);
		label1 = CreateWindow(
			L"STATIC", L"FAF-161 Dincer Deniz Event-Driven Programming - Lab #1",
			WS_VISIBLE | WS_CHILD | SS_SIMPLE,
			0, 0, 0, 0,
			hwnd, NULL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL
		);
		label2 = CreateWindow(
			L"STATIC", L"Text collection: ",
			WS_VISIBLE | WS_CHILD | SS_SIMPLE,
			0, 0, 0, 0,
			hwnd, NULL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL
		);
		textBox1 = CreateWindow(
			L"EDIT", L"Type something here...",
			WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
			0, 0, 0, 0,
			hwnd, HMENU(IDC_TEXT1), (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL
		);
		textBox2 = CreateWindow(
			L"EDIT", L"",
			WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
			0, 0, 0, 0,
			hwnd, HMENU(IDC_TEXT2), (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL
		);


		defaultFont = GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(button1, WM_SETFONT, (WPARAM)defaultFont, TRUE);

		labelFont = CreateFont(20, 0, 0, 0, FW_LIGHT, false, false, false,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, FF_DONTCARE, L"Helvetica");
		SendMessage(label1, WM_SETFONT, WPARAM(labelFont), TRUE);
		SendMessage(textBox1, WM_SETFONT, WPARAM(labelFont), TRUE);

		customFont = CreateFont(20, 0, 0, 0, FW_DONTCARE, false, false, false,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, FF_DONTCARE, L"Consolas");
		SendMessage(button2, WM_SETFONT, WPARAM(customFont), TRUE);
		break;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, backColor);

		EndPaint(hwnd, &ps);
		break;
	}
	case WM_GETMINMAXINFO: {
		LPMINMAXINFO winSize = (LPMINMAXINFO)lParam;
		winSize->ptMinTrackSize.x = 480;
		winSize->ptMinTrackSize.y = 360;
		winSize->ptMaxTrackSize.x = 800;
		winSize->ptMaxTrackSize.y = 600;
		break;
	}
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1: {
			TCHAR textBuff[1024];
			TCHAR textExit[] = L"exit";
			GetWindowText(textBox1, textBuff, 1024);
			if (lstrcmp(textBuff, textExit) == 0) {
				PostQuitMessage(0);
				break;
			}
			SendMessage(textBox2, EM_REPLACESEL, 0, (LPARAM)textBuff);
			SendMessage(textBox2, EM_REPLACESEL, 0, (LPARAM)" ");
			RedrawWindow(textBox2, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
			textEntered = true;
			RedrawWindow(label2, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
			break;
		}
		case IDC_BUTTON2: {
			//MessageBox(hwnd, L"Why did you click this button?", L"No sense", MB_OK | MB_ICONEXCLAMATION);
			SetWindowText(textBox2, L"");
			SetWindowText(textBox1, L"Type something here...");
			textEntered = false;
			RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
			break;
		}
		}
		break;
	}
	case WM_CLOSE: {
		GetWindowRect(hwnd, &rect);
		int width = rect.right - rect.left,
			height = rect.bottom - rect.top;
		MoveWindow(hwnd, rand() % (GetSystemMetrics(SM_CXSCREEN) - width), rand() % (GetSystemMetrics(SM_CYSCREEN) - height), width, height, TRUE);
		MessageBox(hwnd, L"To close the application type 'exit' in the form and press 'Append Text' button!", L"Program termination", MB_OK | MB_ICONINFORMATION);
		break;
	}
	case WM_SYSCOMMAND: {
		switch (wParam & 0xFFF0) {
		case SC_MAXIMIZE: {
			backColor = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
			RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
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
		MoveWindow(label1, (width - 440) / 2, 10, 440, 20, TRUE);
		MoveWindow(label2, width / 2 - width / 4 - 102, height / 2 - height / 8, 120, 20, TRUE);
		MoveWindow(button1, width / 10, height - 67, width - width / 5, 50, TRUE);
		MoveWindow(button2, width / 6, height - 110, width - width / 3, 32, TRUE);
		MoveWindow(textBox1, width / 2 - width / 4, 37, width / 2, height / 4, TRUE);
		MoveWindow(textBox2, width / 2 - width / 4, height / 2 - height / 8, width / 2, height / 4, TRUE);
		RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE); // To prevent overdraw
		break;
	}
	case WM_CTLCOLORSTATIC: {
		if ((HWND)lParam == label1) {
			SetBkMode((HDC)wParam, TRANSPARENT);
		}
		else if ((HWND)lParam == label2) {
			SetBkMode((HDC)wParam, TRANSPARENT);
			SetTextColor((HDC)wParam, textEntered ? RGB(47, 170, 40) : RGB(170, 40, 47));
		}
		else if (IDC_TEXT2 == GetDlgCtrlID((HWND)lParam)) {
			HBRUSH textBoxBrush = CreateSolidBrush(RGB(255, 255, 255));
			return(INT_PTR)textBoxBrush;
		}
		break;
	}
	case WM_CTLCOLOREDIT: {
		if (IDC_TEXT1 == GetDlgCtrlID((HWND)lParam)) {
			HBRUSH textBoxBrush = CreateSolidBrush(RGB(78, 55, 20));
			SetBkColor((HDC)wParam, RGB(78, 55, 20));
			SetTextColor((HDC)wParam, RGB(198, 255, 220));
			return(INT_PTR)textBoxBrush;
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