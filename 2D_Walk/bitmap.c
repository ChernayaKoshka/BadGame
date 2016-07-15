#include "bitmap.h"

unsigned int* Bitmap_GetBytes(char* path)
{
	wchar_t* unableToOpenFmt = L"Unable to open %hs!";
	wchar_t dbgData[512];

	FILE* fp;
	fopen_s(&fp, path, "rb");

	if (!fp)
	{
		swprintf_s(dbgData, sizeof(dbgData) / sizeof(wchar_t), unableToOpenFmt, path);
		OutputDebugString(dbgData);
		assert(fp && !"Unable to load bitmap.");
	}

	long headerlessSize = File_GetSize(fp) - 54;
	unsigned int* rgb = calloc(headerlessSize / 3, sizeof(int));
	fseek(fp, 54, SEEK_SET); //skip header

	int z = 0;
	for (int j = 0; j < headerlessSize; j += 3)
	{
		unsigned char r = fgetc(fp);
		unsigned char g = fgetc(fp);
		unsigned char b = fgetc(fp);
		rgb[z] = RGB(r, g, b);
		z++;
	}
	return rgb;
}

void Bitmap_WriteToBuffer(int* buffer, int bufferWidth, int bitmapWidth, int bitmapHeight, int x, int y, unsigned int* bitmap)
{
	for (int i = 0; i < bitmapWidth; i++)
		for (int z = 0; z < bitmapHeight; z++)
			buffer[(y + z)*bufferWidth + i + x] = bitmap[i*bitmapWidth + z];
}