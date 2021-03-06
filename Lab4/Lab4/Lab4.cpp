
#ifndef UNICODE
#define UNICODE
#endif 

// #include <vector>
// #include <windows.h>
// #include <ctime>
// #include <string>
#include "resource.h"
#include "Object.h"

// Defining the ID values of the windows
#define ID_TIMER1 101
#define HK_EXIT 115


// Declare Windows procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HBRUSH backColor = CreateSolidBrush(RGB(30, 30, 34));
HCURSOR cursor1;

std::vector<Object> ball;
int timerSpeed = 30;


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
	cursor1 = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"VS Windows App";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground = backColor;
	wc.hCursor = cursor1;

	RegisterClass(&wc);

	// Create the window.
	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"PPE Lab #4",		// Window text
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
	static int mouseX, mouseY;

	switch (uMsg) {
	case WM_CREATE: {
		SetTimer(hwnd, ID_TIMER1, timerSpeed, NULL);

		RegisterHotKey(hwnd, HK_EXIT, MOD_ALT, 'Q');
		HANDLE icon = LoadImage((HINSTANCE)hwnd, L"icon1.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
		SendMessage(hwnd, (UINT)WM_SETICON, ICON_BIG, (LPARAM)icon);
	}
	case WM_TIMER:
	{
		InvalidateRect(hwnd, NULL, FALSE); // FALSE - don't erase background when it is next drawn, thus we don't have flickering problems!
		break;
	}
	case WM_LBUTTONDOWN:
	{
		mouseX = LOWORD(lParam), mouseY = HIWORD(lParam);
		Object tempBall({ mouseX, mouseY }, rand() % 2 >= 1 ? rand() % 14 + 1 : -(rand() % 14 + 1), rand() % 2 >= 1 ? rand() % 14 + 1 : -(rand() % 14 + 1), (int)(rand() % 40) + 20, RGB((int)(rand() % 256), (int)(rand() % 256), (int)(rand() % 256)));
		ball.insert(ball.end(), tempBall);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		
		break;
	}
	case WM_MOUSEWHEEL:
	{
		if ((short)HIWORD(wParam) < 0) {
			timerSpeed += 10;
		} else {
			timerSpeed -= 10;
			if (timerSpeed < 1) timerSpeed = 1;
		}
		KillTimer(hwnd, ID_TIMER1);
		SetTimer(hwnd, ID_TIMER1, timerSpeed, NULL);
		break;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		RECT rect;
		HDC hdc = BeginPaint(hwnd, &ps);

		GetClientRect(hwnd, &rect);
		HDC hdcBuffer = CreateCompatibleDC(hdc);
		HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		HGDIOBJ hbmOldBuffer = SelectObject(hdcBuffer, hbmBuffer);

		FillRect(hdcBuffer, &ps.rcPaint, backColor);

		for (auto object = ball.begin(); object != ball.end(); object++) {
			object->Draw(hdcBuffer, &rect);
		}

		for (Object & i : ball) {
			for (Object & j : ball) {
				if (&i == &j) continue;
				POINT colPoint;
				if (i.CollideWith(j, &colPoint)) {
					i.dx = -(colPoint.x - i.pos.x) / 2;
					i.dy = -(colPoint.y - i.pos.y) / 2;
												   
					j.dx = -(colPoint.x - j.pos.x) / 2;
					j.dy = -(colPoint.y - j.pos.y) / 2;
					i.color = RGB((GetRValue(i.color) + 10) % 255, (GetGValue(i.color) + 10) % 255, (GetBValue(i.color) + 10) % 255);
					j.color = RGB((GetRValue(j.color) + 10) % 255, (GetGValue(j.color) + 10) % 255, (GetBValue(j.color) + 10) % 255);
				}
			}
		}
		BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcBuffer, 0, 0, SRCCOPY);

		SelectObject(hdcBuffer, hbmOldBuffer);
		DeleteObject(hbmBuffer);
		DeleteDC(hdcBuffer);
		EndPaint(hwnd, &ps);
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