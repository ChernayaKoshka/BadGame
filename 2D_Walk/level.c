#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "level.h"
#include "math_custom.h"
#include "debug_custom.h"
#include <assert.h>

wchar_t* unableToOpenFmt = L"Unable to open %hs!";
wchar_t* dbgData[512] = { 0 };

void writeTestData()
{
	TileInfo* tinfo = calloc(3, sizeof(TileInfo));

	strcpy_s(tinfo[0].spritePath, sizeof(tinfo[0].spritePath) - 1, "platform.bmp");
	tinfo[0].identifier = 0;
	tinfo[0].is_collidable = 1;

	strcpy_s(tinfo[1].spritePath, sizeof(tinfo[1].spritePath) - 1, "sky.bmp");
	tinfo[1].identifier = 1;

	strcpy_s(tinfo[2].spritePath, sizeof(tinfo[2].spritePath) - 1, "lava.bmp");
	tinfo[2].identifier = 2;
	tinfo[2].is_dangerous = 1;

	FILE* tinfoFp;
	fopen_s(&tinfoFp, "tileinfo.bin", "wb");

	fwrite(tinfo, sizeof(TileInfo), 3, tinfoFp);
	fclose(tinfoFp);

	//576 bytes
	char* levelData = "\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x00\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x00\x00\x00\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x00\x02\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x00\x02\x02\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x00\x00\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x00\x02\x02\x02\x01\x01\x01\x01\x01\x01\x00\x00\x00\x00\x00\x00\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x00\x02\x02\x02\x02\x01\x01\x01\x01\x01\x00\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x00\x02\x02\x02\x02\x02\x01\x01\x01\x01\x00\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x00\x02\x02\x02\x02\x02\x02\x00\x01\x01\x00\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
	FILE* levelFp;
	fopen_s(&levelFp, "level.bin", "wb");
	fwrite(levelData, sizeof(char), 576, levelFp);
	fclose(levelFp);
}

long getFileSize(FILE* fp)
{
	if (!fp)
	{
		swprintf_s(dbgData, sizeof(dbgData) / sizeof(wchar_t), unableToOpenFmt, "getFileSize");
		OutputDebugString(dbgData);
	}

	rewind(fp);
	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);
	rewind(fp);
	return size;
}

BOOL loadLevel(Level* level)
{
	FILE* fp;
	fopen_s(&fp, level->levelFile, "rb");

	if (!fp)
	{
		swprintf_s(dbgData, sizeof(dbgData) / sizeof(wchar_t), unableToOpenFmt, level->levelFile);
		OutputDebugString(dbgData);
	}

	long levelSize = getFileSize(fp);
	char* levelData = calloc(levelSize, 1);
	fread_s(levelData, levelSize, 1, levelSize, fp);
	level->level = levelData;
	level->levelSize = levelSize;
	fclose(fp);

	loadTileData(level);
	return TRUE;
}

unsigned int* getBitMapData(char* path)
{
	FILE* fp;
	fopen_s(&fp, path, "rb");

	if (!fp)
	{
		swprintf_s(dbgData, sizeof(dbgData) / sizeof(wchar_t), unableToOpenFmt, path);
		OutputDebugString(dbgData);
	}

	long headerlessSize = getFileSize(fp) - 54;
	unsigned int* rgb = calloc(headerlessSize / 3, sizeof(int));
	fseek(fp, 54, SEEK_SET);

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

BOOL loadTileData(Level* level)
{
	FILE* fp;
	fopen_s(&fp, level->tileFile, "rb");
	if (!fp)
	{
		swprintf_s(dbgData, sizeof(dbgData) / sizeof(wchar_t), unableToOpenFmt, level->tileFile);
		OutputDebugString(dbgData);
	}

	long tileFileSize = getFileSize(fp);
	int numTiles = tileFileSize / sizeof(TileInfo);
	TileInfo* tiList = calloc(numTiles, sizeof(TileInfo));
	fread_s(tiList, tileFileSize, sizeof(TileInfo), numTiles, fp);
	fclose(fp);

	level->tileCount = numTiles;
	for (int i = 0; i < numTiles; i++)
		tiList[i].bitmap = getBitMapData(tiList[i].spritePath);
	level->tiles = tiList;

	return TRUE;
}

TileInfo* findTile(Level* level, char id)
{
	for (int i = 0; i < level->tileCount; i++)
		if (level->tiles[i].identifier == id)
			return level->tiles + i;
	assert(!"No corresponding tile for id was located!");
	return NULL;
}

void writeTile(int* buffer, int bufferWidth, int x, int y, TileInfo* tile)
{
	for (int i = 0; i < SPRITE_WIDTH; i++)
		for (int z = 0; z < SPRITE_HEIGHT; z++)
			buffer[(y + z)*bufferWidth + i + x] = tile->bitmap[i*SPRITE_WIDTH + z];
}

TileInfo* getTileAtPos(Level* level, int y, int x)
{
	int width = level->levelWidth / SPRITE_WIDTH;

	int roundedY = roundTo(y, SPRITE_HEIGHT) / SPRITE_HEIGHT;
	int roundedX = roundTo(x, SPRITE_WIDTH) / SPRITE_WIDTH;

	int index = roundedY * width + roundedX;
	char below = level->level[index];
	TileInfo* tile = NULL;
	tile = findTile(level, below);

	return tile;
}

BOOL translate(Level* level, int* buffer, int bufferWidth)
{
	int x = 0;
	int y = 0;
	for (int i = 0; i < level->levelSize; i++)
	{
		writeTile(buffer, bufferWidth, x, y, findTile(level, level->level[i]));
		x += SPRITE_WIDTH;
		if (x == level->levelWidth)
		{
			x = 0;
			y += SPRITE_HEIGHT;
		}
	}
	return TRUE;
}