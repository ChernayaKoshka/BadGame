#pragma once

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "math_custom.h"
#include "debug_custom.h"
#include "file.h"
#include "bitmap.h"

#define SPRITE_HEIGHT 20
#define SPRITE_WIDTH 20

typedef struct tagTileInfo
{
	char identifier; //0-255, 256 max types
	unsigned int* bitmap;
	char spritePath[256];

	unsigned int is_collidable : 1;
	unsigned int is_dangerous : 1; //lava
	unsigned int is_ladder : 1;
	unsigned int is_destructible : 1;
}TileInfo;

typedef struct tagEntity
{
	char entityId;
	char alliance;

	char spritePath[256];
	unsigned int* bitmap;

	FloatPoint pos;
	POINT movementData1;
	POINT movementData2;
	BOOL movingLeft;

	unsigned int is_enemy : 1;
	unsigned int is_dangerous : 1;
}EntityInfo;

typedef struct tagLevel
{
	int levelWidth;
	int levelHeight;
	int levelSize;

	char levelFile[256];
	char* level;

	char tileFile[256];
	int tileCount;
	TileInfo* tiles;

	char entityFile[256];
	int entityCount;
	EntityInfo* entities;
}Level;

BOOL Level_Init(Level* level);

TileInfo* Level_FindTile(Level* level, char id);

unsigned int* Bitmap_GetBytes(char* path);

BOOL Level_LoadTileData(Level* level);

BOOL Level_WriteToBuffer(Level* level, int* buffer, int bufferWidth);

TileInfo* Level_GetTileAtPos(Level* level, int y, int x);