#include "debug_custom.h"

wchar_t* fmt = L"%s took %lf seconds\n";
wchar_t data[512] = { 0 };

void dbgPrint(clock_t time, wchar_t* functionName)
{
	swprintf_s(data, 512, fmt, functionName, time / CLOCKS_PER_SEC);
	OutputDebugString(data);
}