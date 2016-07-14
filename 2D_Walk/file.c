#include "file.h"

long File_GetSize(FILE* fp)
{
	long prev = ftell(fp);

	if (!fp)
	{
		wchar_t* unableToOpenFmt = L"Unable to open %hs!";
		wchar_t* dbgData[512] = { 0 };

		swprintf_s(dbgData, sizeof(dbgData) / sizeof(wchar_t), unableToOpenFmt, "getFileSize");
		OutputDebugString(dbgData);
		assert(fp);
	}

	rewind(fp);
	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);
	fseek(fp, prev, SEEK_SET);
	return size;
}