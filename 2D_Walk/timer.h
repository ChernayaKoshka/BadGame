#pragma once

#include <Windows.h>

extern __int64 gTimeCount;
extern double gTimePassed;
extern double gSecondsPerTick;

BOOL Time_Init();
float Time_Get();