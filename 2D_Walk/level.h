#pragma once
#include <Windows.h>

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
	unsigned int is_enemy : 1;
	POINT pos;
	POINT movementData1;
	POINT movementData2;
	BOOL movingLeft;
}TileInfo;

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
}Level;

BOOL loadLevel(Level* level);

TileInfo* findTile(Level* level, char id);

unsigned int* getBitMapData(char* path);

BOOL loadTileData(Level* level);

BOOL translate(Level* level, int* buffer, int bufferWidth);

void writeTile(int* buffer, int bufferWidth, int x, int y, TileInfo* tile);

TileInfo* getTileAtPos(Level* level, int y, int x);

#if _DEBUG
void writeTestData();
#endif