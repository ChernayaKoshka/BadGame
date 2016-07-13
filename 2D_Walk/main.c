#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <ShlObj.h>
#include <process.h>
#include "level.h"
#include "math_custom.h"
#include "globals.h"
#include "drawing.h"
#include "entity.h"
#include "player.h"
#include "screen.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 360

#define WINDOW_TITLE L"2D Walk"
#define WINDOW_CLASS L"2dwalk"

wchar_t* titleFormat = L"(%d,%d)";
wchar_t title[256] = L"(%d,%d)";

wchar_t* dbgFmt = L"%d rounds down to %d\n";
wchar_t dbg[256];

BOOL running = TRUE;

Level* level;
Player player = { 0 };

extern WindowDetails* details;

//#if DISPLAY_COLLISION_PROCESS
TileInfo* dbgTile;
POINT dbgTilePos;
//#endif

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
			player.horizontalDirection = LEFT;
			break;
		case VK_RIGHT:
			player.horizontalDirection = RIGHT;
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
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_LEFT:
			player.horizontalDirection = NONE;
			break;
		case VK_RIGHT:
			player.horizontalDirection = NONE;
			break;
		}

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return Result;
}

void update(int runCount)
{
	if (runCount % 100 == 0)
		updateEntities(level, 2);
	updatePlayer(&player, level, runCount);
}

void render()
{
	swprintf_s(title, 256, titleFormat, player.pos.x, player.pos.y);
	SetWindowTextW(details->Window, title);

	levelToScreen(level, details->BackBuffer, details->Width);

	writeBitmap(details->BackBuffer, details->Width, player.pos.x, player.pos.y, player.bitmap);

	displayEntities(level, details->BackBuffer, details->Width);

#if DISPLAY_COLLISION_POINTS
	for (int i = 0; i < 4; i++)
		Plot(player.pos.x + player.collisionPoints[i].x, player.pos.y + player.collisionPoints[i].y, 0x00FFFFFF, details->BackBuffer, details->Width);

	for (int i = 0; i < level->entityCount; i++)
	{
		POINT* colPoints = getEntityPoints(level, level->entities[i].entityId);
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

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	setupWindow(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_CLASS, WINDOW_TITLE, nShowCmd, WndProc);

	clock_t prevTime = clock();

#if _DEBUG
	writeTestData();
#endif

	level = calloc(1, sizeof(Level));

	strcpy_s(level->levelFile, sizeof(level->levelFile) - 1, "level.bin");
	strcpy_s(level->tileFile, sizeof(level->tileFile) - 1, "tileinfo.bin");
	strcpy_s(level->entityFile, sizeof(level->entityFile) - 1, "entityinfo.bin");

	level->levelWidth = WINDOW_WIDTH;
	level->levelHeight = WINDOW_HEIGHT;

	loadLevel(level);

	player.bitmap = getBitMapData("player.bmp");

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

	//#if DISPLAY_COLLISION_PROCESS
	dbgTile = calloc(1, sizeof(TileInfo));
	dbgTile->bitmap = getBitMapData("debug_tile.bmp");
	//#endif

	setupEntities(level);

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
			update(runCount++);
			render();
		}
	}

	return EXIT_SUCCESS;
}