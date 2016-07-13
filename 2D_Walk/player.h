#pragma once

#include <Windows.h>
#include "math_custom.h"
#include "level.h"
#include "globals.h"
#include "screen.h"
#include "entity.h"
#include "drawing.h"

#define PLAYER_JUMP_HEIGHT SPRITE_HEIGHT*3

#define PLAYER_HORIZONTAL_SPEED 4

typedef struct tagPlayerPos
{
	float x;
	float y;
}PlayerPos;

typedef enum tagPlayerDirection
{
	NONE = 0,
	LEFT,
	UP,
	RIGHT,
	DOWN
}ePlayerDirection;

typedef struct tagPlayer
{
	POINT pos;
	POINT collisionPoints[4];

	ePlayerDirection horizontalDirection;
	ePlayerDirection verticalDirection;

	int jumpHeight;
	unsigned int isJumping : 1;

	unsigned int* bitmap;
}Player;

BOOL changePos(Player* player, Level* level, int newX, int newY);

TileInfo* getTileUnderPlayer(Player* player, Level* level);

void updatePlayer(Player* player, Level* level, unsigned long step);