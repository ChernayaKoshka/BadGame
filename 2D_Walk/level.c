#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "level.h"
#include "math_custom.h"
#include <assert.h>

long getFileSize(FILE* fp)
{
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
	TileInfo* tile = findTile(level, below);

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