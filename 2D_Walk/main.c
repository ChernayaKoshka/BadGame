#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <ShlObj.h>
#include <process.h>
#include "level.h"
#include "screen.h"
#include "player.h"
#include "timer.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 360

#define WINDOW_TITLE L"2D Walk"
#define WINDOW_CLASS L"2dwalk"

extern WindowDetails* details;

Level* level;

BOOL running = TRUE;

void update(float timeAccumulated)
{
	Enitites_Update();
	Player_Update();
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
#if _DEBUG
	Debug_WriteTestData();
#endif

	/*INIT*/
	if (!Time_Init())
	{
		MessageBoxW(NULL, L"Get a newer operating system.", L"What is this, the stone age?", MB_OK);
		return -1;
	}

	level = calloc(1, sizeof(Level)); //fix later

	 //TODO: REPLACE
	level->levelWidth = WINDOW_WIDTH;
	level->levelHeight = WINDOW_HEIGHT;
	if (!Level_Init(level))
	{
		MessageBox(NULL, L"Unable to Level_Init!", L"Unable to Level_Init!", MB_OK);
		return -1;
	}

	if (!Entities_Init())
	{
		MessageBox(NULL, L"Unable to Entities_Init!", L"Unable to Entities_Init!", MB_OK);
		return -1;
	}

	if (!Player_Init())
	{
		MessageBox(NULL, L"Unable to Player_Init!", L"Unable to Player_Init!", MB_OK);
		return -1;
	}

	if (!Screen_Init(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_CLASS, WINDOW_TITLE, nShowCmd)) //gotta make sure that player/level are initialized before creating window
	{
		MessageBox(NULL, L"Unable to Screen_Init!", L"Unable to Screen_Init!", MB_OK);
		return -1;
	}
	/*END INIT*/


	float prevTime = 0;
	float timeAccumulated = 0;

	while (running)
	{
		Screen_HandleWindowEvents();
		float time = Time_Get();
		timeAccumulated += time - prevTime;
		prevTime = time;
		if (timeAccumulated >= STEPS_PER_SECOND)
		{
			update(timeAccumulated);
			Screen_Render();
			timeAccumulated -= STEPS_PER_SECOND;
		}
	}

	return EXIT_SUCCESS;
}