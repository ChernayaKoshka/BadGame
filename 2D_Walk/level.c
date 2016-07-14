#include "level.h"

BOOL Level_LoadTileData(Level* level)
{
	FILE* fp;
	fopen_s(&fp, level->tileFile, "rb");
	if (!fp)
	{
		wchar_t* unableToOpenFmt = L"Unable to open %hs!";
		wchar_t dbgData[512];
		swprintf_s(dbgData, sizeof(dbgData) / sizeof(wchar_t), unableToOpenFmt, level->tileFile);
		OutputDebugString(dbgData);
		assert(fp);
	}

	long tileFileSize = File_GetSize(fp);
	int numTiles = tileFileSize / sizeof(TileInfo);
	TileInfo* tiList = calloc(numTiles, sizeof(TileInfo));
	fread_s(tiList, tileFileSize, sizeof(TileInfo), numTiles, fp);
	fclose(fp);

	level->tileCount = numTiles;
	for (int i = 0; i < numTiles; i++)
		tiList[i].bitmap = Bitmap_GetBytes(tiList[i].spritePath);
	level->tiles = tiList;

	return TRUE;
}

BOOL Level_Init(Level* level)
{
	strcpy_s(level->levelFile, sizeof(level->levelFile) - 1, "level.bin");
	strcpy_s(level->tileFile, sizeof(level->tileFile) - 1, "tileinfo.bin");
	strcpy_s(level->entityFile, sizeof(level->entityFile) - 1, "entityinfo.bin");

	FILE* fp;
	fopen_s(&fp, level->levelFile, "rb");

	if (!fp)
	{
		wchar_t* unableToOpenFmt = L"Unable to open %hs!";
		wchar_t dbgData[512];
		swprintf_s(dbgData, sizeof(dbgData) / sizeof(wchar_t), unableToOpenFmt, level->levelFile);
		OutputDebugString(dbgData);
		assert(fp);
	}

	long levelSize = File_GetSize(fp);
	char* levelData = calloc(levelSize, 1);
	fread_s(levelData, levelSize, 1, levelSize, fp);
	level->level = levelData;
	level->levelSize = levelSize;
	fclose(fp);

	Level_LoadTileData(level);
	return TRUE;
}

TileInfo* Level_FindTile(Level* level, char id)
{
	for (int i = 0; i < level->tileCount; i++)
		if (level->tiles[i].identifier == id)
			return level->tiles + i;
	assert(!"No corresponding tile for id was located!");
	return NULL;
}

TileInfo* Level_GetTileAtPos(Level* level, int y, int x)
{
	int width = level->levelWidth / SPRITE_WIDTH;

	int roundedY = roundTo(y, SPRITE_HEIGHT) / SPRITE_HEIGHT;
	int roundedX = roundTo(x, SPRITE_WIDTH) / SPRITE_WIDTH;

	TileInfo* tile = NULL;

	int index = roundedY * width + roundedX;
	char below = level->level[index];
	tile = Level_FindTile(level, below);

	return tile;
}

BOOL Level_WriteToBuffer(Level* level, int* buffer, int bufferWidth)
{
	int x = 0;
	int y = 0;
	for (int i = 0; i < level->levelSize; i++)
	{
		Bitmap_WriteToBuffer(buffer, bufferWidth, SPRITE_WIDTH, SPRITE_HEIGHT, x, y, Level_FindTile(level, level->level[i])->bitmap);
		x += SPRITE_WIDTH;
		if (x == level->levelWidth)
		{
			x = 0;
			y += SPRITE_HEIGHT;
		}
	}
	return TRUE;
}