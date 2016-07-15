#include "screen.h"
#include "player.h"

WindowDetails* details;

extern Player* player;
extern Level* level;

wchar_t* titleFormat = L"Pos: (%f,%f) Velocity Vector: <%f,%f>";
wchar_t title[256];

void Screen_HandleWindowEvents()
{
	MSG msg;
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, WPARAM lParam)
{
	POINT newPos = { 0 };

	LRESULT Result = 0;
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		abort();
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			if (player->velocity.i > -PLAYER_HORIZONTAL_SPEED_MAX)
				player->velocity.i -= PLAYER_HORIZONTAL_SPEED_INCREMENT;
			break;
		case VK_RIGHT:
			if (player->velocity.i < PLAYER_HORIZONTAL_SPEED_MAX)
				player->velocity.i += PLAYER_HORIZONTAL_SPEED_INCREMENT;
			break;
		case VK_SPACE:
			Player_StartJump();
			break;
		}
		break;
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_LEFT:
			player->velocity.i = 0;
			break;
		case VK_RIGHT:
			player->velocity.i = 0;
			break;
		case VK_SPACE:
			Player_EndJump();
			break;
		}

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return Result;
}

WindowDetails* Screen_DefineWindow(HINSTANCE hInstance, int width, int height, wchar_t* className, wchar_t* windowName, int nShowCmd)
{
	WindowDetails* details = calloc(1, sizeof(WindowDetails));
	if (!details) return NULL;

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
	wc.lpfnWndProc = WndProc;
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

	if (!hwndWindow) return NULL;

	details->Window = hwndWindow;
	details->DC = GetDC(hwndWindow);
	ShowWindow(hwndWindow, nShowCmd);

	return details;
}

BOOL Screen_Init(HINSTANCE hInstance, int width, int height, wchar_t* className, wchar_t* windowName, int nShowCmd)
{
	details = Screen_DefineWindow(hInstance, width, height, className, windowName, nShowCmd);
	if (!details) return FALSE;
	return TRUE;
}

void Screen_Render()
{
	swprintf_s(title, 256, titleFormat, player->pos.x, player->pos.y, player->velocity.i, player->velocity.j);
	SetWindowTextW(details->Window, title);

	Level_WriteToBuffer(level, details->BackBuffer, details->Width);

	Bitmap_WriteToBuffer(details->BackBuffer, details->Width, SPRITE_WIDTH, SPRITE_HEIGHT, player->pos.x, player->pos.y, player->bitmap);

	Enities_WriteToBuffer(level, details->BackBuffer, details->Width);

#if DISPLAY_COLLISION_POINTS
	for (int i = 0; i < 4; i++)
		Plot(player->pos.x + player->collisionPoints[i].x, player->pos.y + player->collisionPoints[i].y, 0x00FFFFFF, details->BackBuffer, details->Width);

	for (int i = 0; i < level->entityCount; i++)
	{
		POINT* colPoints = Entities_GetEntityPoints(level->entities[i].entityId);
		for (int j = 0; j < 4; j++)
			Plot(colPoints[j].x, colPoints[j].y, 0x00FFFFFF, details->BackBuffer, details->Width);
		free(colPoints);
	}
#endif

	StretchDIBits(details->DC,
		0, 0, details->Width, details->Height,
		0, 0, details->BitMapInfo.bmiHeader.biWidth, Abs(details->BitMapInfo.bmiHeader.biHeight),
		details->BackBuffer, &details->BitMapInfo,
		DIB_RGB_COLORS, SRCCOPY);
}