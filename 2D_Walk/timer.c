#include "timer.h"

double gSecondsPerTick;
__int64 gTimeCount;
double gTimePassed;

BOOL Time_Init()
{
	LARGE_INTEGER tick;
	LARGE_INTEGER frequency;

	if (!QueryPerformanceFrequency(&frequency)) return FALSE;

	gSecondsPerTick = 1.0 / (double)frequency.QuadPart;

	if (!QueryPerformanceCounter(&tick)) return FALSE;

	gTimeCount = tick.QuadPart;

	return TRUE;
}

float Time_Get()
{
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);

	__int64 interval = counter.QuadPart - gTimeCount;
	gTimeCount = counter.QuadPart;
	double secondsGoneBy = (double)interval*gSecondsPerTick;
	gTimePassed += secondsGoneBy;
	return (float)gTimePassed;
}