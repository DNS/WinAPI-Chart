/*
	Name		: WinAPI Chart
	Author	: Daniel Sirait <dns@cpan.org>
	License : Public Domain
*/

#pragma comment(linker,"\"/manifestdependency:type='win32' \
	name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
	processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib, "Comctl32.lib")


/* force MSVC to use ANSI/WideChar function, must be before #include <windows.h> */
#define UNICODE
//#undef UNICODE


#include <windows.h>
#include <commctrl.h>


HFONT hfont1;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WinapiChart (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DestroyChildWindow(HWND, LPARAM);


INT WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	MSG msg;
	HWND hwnd;
	WNDCLASSEX wc = {0};		// initialize with 0
	INITCOMMONCONTROLSEX iccex;
	LONG style;

	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra= 0;
	wc.cbWndExtra= 0;
	wc.hInstance = hInstance;
	wc.lpszMenuName= NULL;
	wc.lpszClassName = L"Window";
	wc.hbrBackground = (HBRUSH) COLOR_WINDOW;	// default window color
	//wc.hbrBackground = CreateSolidBrush(RGB(255, 0, 0));	// make something different
	wc.hCursor = LoadCursorW(NULL, IDC_ARROW);	// Note: LoadCursor() superseded by LoadImage()
	wc.hIcon = (HICON) LoadImageW(NULL, L"razor.ico", IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wc.hIconSm = (HICON) LoadImageW(NULL, L"razor.ico", IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE);

	iccex.dwICC = ICC_WIN95_CLASSES | ICC_STANDARD_CLASSES;
	iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	//InitCommonControls();			// obsolete
	InitCommonControlsEx(&iccex);

	if (!RegisterClassExW(&wc)) {
		MessageBoxW(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}
	
	// create main window
	hwnd = CreateWindowExW(WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES | WS_EX_CONTROLPARENT, 
		wc.lpszClassName, L"Title", 
		WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_TABSTOP, 
		0, 0, 800, 600, (HWND) NULL, (HMENU) NULL, hInstance, NULL);
	
	style = GetWindowLong(hwnd, GWL_STYLE);
	//style = style & ~(WS_MINIMIZEBOX | WS_SYSMENU);
	
	SetWindowLong(hwnd, GWL_STYLE, style);
	
	//ShowWindow(hwnd, nCmdShow);
	//UpdateWindow(hwnd);

	
	while (GetMessageW(&msg, NULL, 0, 0) > 0) {		/* If no error is received... */
		TranslateMessage(&msg);		/* Translate key codes to chars if present */
		DispatchMessageW(&msg);		/* Send it to WndProc */
	}

	return (int) msg.wParam;
}


LRESULT CALLBACK WndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	HPEN hPen_solid, hPen_noborder;
	HPEN pen_solid1, pen_solid2, holdPen1, holdPen2;
	HBRUSH hBrush_green;
	PAINTSTRUCT ps, ps2;
	HWND hwnd_chart;
	WNDCLASS rwc = {0};
	HWND hwndTrack, hwndBurn;
	

	

	switch (msg) {
		case WM_CREATE:
			hfont1 = CreateFontW(13, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, 
				OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
				DEFAULT_PITCH | FF_DONTCARE, L"Tahoma");

			rwc.lpszClassName = L"winapi-chart";
			rwc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
			rwc.style = CS_HREDRAW;
			rwc.lpfnWndProc = WinapiChart;
			rwc.hCursor = LoadCursor(0, IDC_ARROW);
			RegisterClassW(&rwc);

			hwnd_chart = CreateWindowExW(WS_EX_STATICEDGE , L"winapi-chart", NULL,
				WS_CHILD | WS_VISIBLE, 0, 0, 490, 300, hwnd, (HMENU) 1, NULL, NULL);


			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				
			}	// switch(LOWORD(wParam))
			break;
		case WM_ACTIVATE:
			break;
		case WM_CLOSE:
			EnumChildWindows(hwnd, DestroyChildWindow, lParam);
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			DeleteObject(hfont1);

			PostQuitMessage(0);
			break;
		return 0;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}


LRESULT CALLBACK WinapiChart (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	HBRUSH hBrushYellow, hBrushRed, holdBrush;
	HPEN hPen, holdPen;
	HFONT hFont, holdFont;
	PAINTSTRUCT ps;
	RECT rect, rect2;
	

	wchar_t *cap[] = { L"75", L"150", L"225", L"300", L"375", L"450", 
		L"525", L"600", L"675"};

	HDC hdc;
	int till;
	int step, full;
	int i;
	HPEN hPen_solid, hPen_noborder;
	HBRUSH hBrush_green;

	switch (msg) {
		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			//ARRAYSIZE();
			SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));
			SelectObject(hdc, hfont1);

			TextOutW(hdc, 50, 20, L"test", 4);


			hPen_solid = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			SelectObject(hdc, hPen_solid);

			MoveToEx(hdc, 100, 200, NULL);
			LineTo(hdc, 250, 200);

			MoveToEx(hdc, 100, 200, NULL);
			LineTo(hdc, 100, 50);


			// If a PS_NULL pen is used, the dimensions of the rectangle are 1 pixel less in height and 1 pixel less in width.
			hPen_noborder = CreatePen(PS_NULL, 1, RGB(0, 0, 0));	// no border
			hBrush_green = CreateSolidBrush(RGB(9, 189, 21));

			SelectObject(hdc, hPen_noborder);
			SelectObject(hdc, hBrush_green);

			//Rectangle(hdc, 100+1, 50+1, 250+1, 200+1);
			Rectangle(hdc, 120+1, 50+1, 150+1, 200+1);		
			


			DeleteDC(hdc);

		/*	hdc = BeginPaint(hwnd, &ps);

			GetClientRect(hwnd, &rect);

			till = (rect.right / 750.0) * 150;
			step = rect.right / 10.0;
			full = (rect.right / 750.0) * 700;
				
			hBrushYellow = CreateSolidBrush(RGB(255, 255, 184));
			hBrushRed = CreateSolidBrush(RGB(255, 110, 110));

			hPen = CreatePen(PS_NULL, 1, RGB(0, 0, 0));
			holdPen = SelectObject(hdc, hPen);

			hFont = CreateFontW(13, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 0, 
				0, 0, 0, 0, L"Tahoma");

			holdFont = SelectObject(hdc, hFont);
				
			if (till > full) {
				SelectObject(hdc, hBrushYellow);
				Rectangle(hdc, 0, 0, full, 30);
				holdBrush = SelectObject(hdc, hBrushRed);
				Rectangle(hdc, full, 0, till, 30);

			} else {
				holdBrush = SelectObject(hdc, hBrushYellow);
				Rectangle(hdc, 0, 0, till, 30);
			}

			SelectObject(hdc, holdPen);

			for ( i = 1; i < 10; i++) {
				MoveToEx(hdc, i*step, 0, NULL);
				LineTo(hdc, i*step, 7);

				rect2.bottom = 28;
				rect2.top = 8;
				rect2.left = i*step-10;
				rect2.right = i*step+10;

				SetBkMode(hdc, TRANSPARENT) ;
				DrawTextW(hdc, cap[i-1], wcslen(cap[i-1]), &rect2, DT_CENTER);
			}

			SelectObject(hdc, holdBrush);
			DeleteObject(hBrushYellow);
			DeleteObject(hBrushRed);

			DeleteObject(hPen);

			SelectObject(hdc, holdFont);
			DeleteObject(hFont);
										
			EndPaint(hwnd, &ps);*/
		break;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}


BOOL CALLBACK DestroyChildWindow(HWND hwnd, LPARAM lParam) {
	DestroyWindow(hwnd);
	SendMessageW(hwnd, WM_DESTROY, 0, 0);
	return TRUE;
}


