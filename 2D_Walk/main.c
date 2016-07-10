#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <ShlObj.h>
#include "math_custom.h"
#include "globals.h"
#include "drawing.h"
#include "level.h"
#include "misc.h"
#include "player.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 360

#define WINDOW_TITLE L"2D Walk"
#define WINDOW_CLASS L"2dwalk"
#define GRAVITY 1

wchar_t* titleFormat = L"(%d,%d)";
wchar_t title[256] = L"(%d,%d)";

typedef struct tagWindowDetails
{
	int Width;
	int Height;
	void* BackBuffer;
	HDC DC;
	HWND Window;
	BITMAPINFO BitMapInfo;
}WindowDetails;

BOOL running = TRUE;

unsigned long int generation = 0;

int ButtonMessage = 0;

int paused = 0;

HBRUSH hbrWhite, hbrGray;

Level* level;
Player player = { 0 };

WindowDetails* details;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, WPARAM lParam)
{
	POINT newPos = { 0 };

	LRESULT Result = 0;
	switch (uMsg)
	{
	case WM_CLOSE:
		running = FALSE;
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			changePos(&player, level, player.pos.x - 5, player.pos.y);
			break;
		case VK_RIGHT:
			changePos(&player, level, player.pos.x + 5, player.pos.y);
			break;
		case VK_SPACE:
			if (!player.isJumping && getTileAtPos(level, player.pos.y + SPRITE_HEIGHT, player.pos.x)->is_collidable)
			{
				changePos(&player, level, player.pos.x, player.pos.y - SPRITE_HEIGHT * 2);
				player.isJumping = TRUE;
				player.jumpTimer = 2000;
			}
			break;
		}
		swprintf_s(title, 256, titleFormat, player.pos.x, player.pos.y);
		SetWindowTextW(details->Window, title);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return Result;
}

WindowDetails* DefineWindow(HINSTANCE hInstance, int width, int height, wchar_t* className, wchar_t* windowName, int nShowCmd)
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

	details->Window = hwndWindow;
	details->DC = GetDC(hwndWindow);
	ShowWindow(hwndWindow, nShowCmd);

	return details;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	details = DefineWindow(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_CLASS, WINDOW_TITLE, nShowCmd);

	MSG msg;
	clock_t prevTime = clock();

#if _DEBUG
	writeTestData();
#endif

	level = calloc(1, sizeof(Level));

	strcpy_s(level->levelFile, sizeof(level->levelFile) - 1, "level.bin");
	strcpy_s(level->tileFile, sizeof(level->tileFile) - 1, "tileinfo.bin");

	level->levelWidth = WINDOW_WIDTH;
	level->levelHeight = WINDOW_HEIGHT;

	loadLevel(level);

	TileInfo* playerSprite = calloc(1, sizeof(TileInfo));
	strcpy_s(playerSprite->spritePath, sizeof(playerSprite->spritePath) - 1, "player.bmp");

	playerSprite->bitmap = getBitMapData(playerSprite->spritePath);

	player.pos.x = 0;
	player.pos.y = SPRITE_HEIGHT;

	unsigned long runCount = 0;
	while (running)
	{
		while (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		clock_t curTime = clock();
		double timePassed = (double)(curTime - prevTime) / CLOCKS_PER_SEC;
		if (timePassed >= STEPS_PER_SECOND)
		{
			runCount++;
			translate(level, details->BackBuffer, details->Width);
			if (player.isJumping)
			{
				if (player.jumpTimer <= 0)
					player.isJumping = FALSE;

				player.jumpTimer--;

				if (player.jumpTimer == 1000)
					changePos(&player, level, player.pos.x, player.pos.y + SPRITE_HEIGHT);

				TileInfo* tile = getTileAtPos(level, player.pos.y + SPRITE_HEIGHT, player.pos.x);

				if (tile->is_collidable)
					player.isJumping = FALSE;
			}
			else if (runCount % 100 == 0)
			{
				changePos(&player, level, player.pos.x, player.pos.y + 1);
			}
			writeTile(details->BackBuffer, details->Width, player.pos.x, player.pos.y, playerSprite);

			StretchDIBits(details->DC,
				0, 0, details->Width, details->Height,
				0, 0, details->BitMapInfo.bmiHeader.biWidth, Abs(details->BitMapInfo.bmiHeader.biHeight),
				details->BackBuffer, &details->BitMapInfo,
				DIB_RGB_COLORS, SRCCOPY);
		}
	}
	return EXIT_SUCCESS;
}