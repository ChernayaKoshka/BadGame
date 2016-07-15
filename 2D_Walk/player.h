#pragma once

#include <Windows.h>
#include <math.h>
#include "math_custom.h"
#include "globals.h"
#include "level.h"
#include "vector.h"

#if DISPLAY_COLLISION_PROCESS
#include "screen.h"
#endif

#define PLAYER_JUMP_VELOCITY 60.0/60.0f

#define PLAYER_HORIZONTAL_SPEED_MAX 60.0 / 60.0f

#define PLAYER_HORIZONTAL_SPEED_INCREMENT PLAYER_HORIZONTAL_SPEED_MAX / 4

typedef struct tagPlayerPos
{
	float x;
	float y;
}PlayerPos;

typedef struct tagPlayer
{
	FloatPoint pos;
	FloatPoint collisionPoints[4];

	Vector velocity;

	int jumpHeight;
	unsigned int isJumping : 1;

	unsigned int* bitmap;
}Player;

extern Player* player;

BOOL Player_Init();

void Player_Die();

BOOL Player_AdjustPos(float relativeX, float relativeY);

void Player_Update();

void Player_StartJump();

void Player_EndJump();