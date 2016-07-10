#pragma once
#include <Windows.h>
#include "math_custom.h"
#include "level.h"

typedef struct tagPlayerPos
{
	float x;
	float y;
}PlayerPos;

typedef struct tagPlayer
{
	POINT pos;
	int jumpTimer;
	unsigned int isJumping : 1;
}Player;

BOOL changePos(Player* player, Level* level, int newX, int newY);