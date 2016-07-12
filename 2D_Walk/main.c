#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <ShlObj.h>
#include <process.h>
#include "math_custom.h"
#include "globals.h"
#include "drawing.h"
#include "level.h"
#include "misc.h"
#include "player.h"
#include "enemy.h"
#include "screen.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 360

#define WINDOW_TITLE L"2D Walk"
#define WINDOW_CLASS L"2dwalk"
#define GRAVITY 1

wchar_t* titleFormat = L"(%d,%d)";
wchar_t title[256] = L"(%d,%d)";

wchar_t* dbgFmt = L"%d rounds down to %d\n";
wchar_t dbg[256];

BOOL running = TRUE;

unsigned long int generation = 0;

int ButtonMessage = 0;

int paused = 0;

HBRUSH hbrWhite, hbrGray;

Level* level;
Player player = { 0 };

WindowDetails* details;

TileInfo* dbgTile;

POINT dbgTilePos;

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
			if (!player.isJumping && getTileUnderPlayer(&player, level)->is_collidable)
			{
				player.jumpHeight = PLAYER_JUMP_HEIGHT;
				changePos(&player, level, player.pos.x, (player.pos.y - PLAYER_JUMP_HEIGHT));
				player.isJumping = TRUE;
			}
			break;
		}
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
	player.pos.y = 200;

	player.collisionPoints[0].x = 0;
	player.collisionPoints[0].y = 0;

	player.collisionPoints[1].x = 0;
	player.collisionPoints[1].y = SPRITE_HEIGHT - 1;

	player.collisionPoints[2].x = SPRITE_WIDTH - 1;
	player.collisionPoints[2].y = 0;

	player.collisionPoints[3].x = SPRITE_WIDTH - 1;
	player.collisionPoints[3].y = SPRITE_HEIGHT - 1;

	dbgTile = calloc(1, sizeof(TileInfo));
	dbgTile->bitmap = getBitMapData("debug_tile.bmp");

	setupEnemies(level);

	MSG msg;
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
			swprintf_s(title, 256, titleFormat, player.pos.x, player.pos.y);
			SetWindowTextW(details->Window, title);

			runCount++;

			if (runCount % 100 == 0)
			{
				updateEnemies(level, 2);
			}

			updatePlayer(&player, level, runCount);
			translate(level, details->BackBuffer, details->Width);
			writeTile(details->BackBuffer, details->Width, player.pos.x, player.pos.y, playerSprite);
			for (int i = 0; i < 4; i++)
				Plot(player.pos.x + player.collisionPoints[i].x, player.pos.y + player.collisionPoints[i].y, 0x00FFFFFF, details->BackBuffer, details->Width, details->Height);
			displayEnemies(level, details->BackBuffer, details->Width);

			writeTile(details->BackBuffer, details->Width, dbgTilePos.x, dbgTilePos.y, dbgTile);
			StretchDIBits(details->DC,
				0, 0, details->Width, details->Height,
				0, 0, details->BitMapInfo.bmiHeader.biWidth, Abs(details->BitMapInfo.bmiHeader.biHeight),
				details->BackBuffer, &details->BitMapInfo,
				DIB_RGB_COLORS, SRCCOPY);
		}
	}

	return EXIT_SUCCESS;
}