#pragma once

#include <Windows.h>
#include "math_custom.h"
#include "globals.h"
#include "level.h"
#include <math.h>

#define PLAYER_JUMP_HEIGHT SPRITE_HEIGHT*3

#define PLAYER_HORIZONTAL_SPEED 4 / 60.0f

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
	FloatPoint pos;
	FloatPoint collisionPoints[4];

	ePlayerDirection horizontalDirection;
	ePlayerDirection verticalDirection;

	int jumpHeight;
	unsigned int isJumping : 1;

	unsigned int* bitmap;
}Player;

extern Player* player;

BOOL Player_Init();

void Player_Die();

BOOL Player_ChangePos(float relativeX, float relativeY);

void Player_Update();

void Player_Jump();