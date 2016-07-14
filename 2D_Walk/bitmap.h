#pragma once
#include <Windows.h>
#include <stdio.h>
#include "file.h"

unsigned int* Bitmap_GetBytes(char* path);

void Bitmap_WriteToBuffer(int* buffer, int bufferWidth, int bitmapWidth, int bitmapHeight, int x, int y, unsigned int* bitmap);