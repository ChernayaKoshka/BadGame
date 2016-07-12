#pragma once
#include <Windows.h>
#include "math_custom.h"
#include "level.h"

#define PLAYER_JUMP_HEIGHT SPRITE_HEIGHT*2

typedef struct tagPlayerPos
{
	float x;
	float y;
}PlayerPos;

typedef struct tagPlayer
{
	POINT pos;
	POINT collisionPoints[4];
	int jumpHeight;
	unsigned int isJumping : 1;
}Player;

typedef enum tageCollisionLocation
{
	TOPLEFT = 0,
	BOTTOMLEFT,
	TOPRIGHT,
	BOTTOMRIGHT
}eCollisionLocation;

BOOL changePos(Player* player, Level* level, int newX, int newY);

TileInfo* getTileUnderPlayer(Player* player, Level* level);

void updatePlayer(Player* player, Level* level, unsigned long step);