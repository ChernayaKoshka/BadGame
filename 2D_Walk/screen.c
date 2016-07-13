#include "screen.h"

WindowDetails* details;

WindowDetails* DefineWindow(HINSTANCE hInstance, int width, int height, wchar_t* className, wchar_t* windowName, int nShowCmd, WNDPROC proc)
{
	WindowDetails* details = calloc(1, sizeof(WindowDetails));
	details->Height = height;
	details->Width = width;

	int bufferSize = height*width * sizeof(int);
	details->BackBuffer = calloc(1, bufferSize); //4 = bytes to display RGB

	details->BitMapInfo.bmiHeader.biSize = sizeof(details->BitMapInfo.bmiHeader);
	details->BitMapInfo.bmiHeader.biWidth = details->Width;
	details->BitMapInfo.bmiHeader.biHeight = -details->Height;
	details->BitMapInfo.bmiHeader.biPlanes = 1;
	details->BitMapInfo.bmiHeader.biBitCount = 32;
	details->BitMapInfo.bmiHeader.biCompression = BI_RGB;

	//define window
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
#pragma warning(disable : 4028)
	wc.lpfnWndProc = proc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	wc.lpszClassName = className;

	if (!RegisterClassEx(&wc))
		return NULL;

	RECT adjustedRect = { 0 };
	adjustedRect.bottom = height;
	adjustedRect.right = width;
	AdjustWindowRect(&adjustedRect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE, FALSE);

	HWND hwndWindow = CreateWindowExW(
		0,
		className,
		windowName,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		Difference(adjustedRect.left, adjustedRect.right),
		Difference(adjustedRect.bottom, adjustedRect.top),
		NULL,
		NULL,
		0,
		0);

	details->Window = hwndWindow;
	details->DC = GetDC(hwndWindow);
	ShowWindow(hwndWindow, nShowCmd);

	return details;
}

void setupWindow(HINSTANCE hInstance, int width, int height, wchar_t* className, wchar_t* windowName, int nShowCmd, WNDPROC proc)
{
	details = DefineWindow(hInstance, width, height, className, windowName, nShowCmd, proc);
	if (!details)
	{
		assert(details && !"WindowDetials* details was NULL");
		return;
	}
	return;
}

void updateWindow()
{
	StretchDIBits(details->DC,
		0, 0, details->Width, details->Height,
		0, 0, details->BitMapInfo.bmiHeader.biWidth, Abs(details->BitMapInfo.bmiHeader.biHeight),
		details->BackBuffer, &details->BitMapInfo,
		DIB_RGB_COLORS, SRCCOPY);
}