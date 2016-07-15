#include "player.h"

extern Level* level;

#if DISPLAY_COLLISION_PROCESS
extern WindowDetails* details;
#endif

Player* player;

BOOL Player_Init()
{
	player = calloc(1, sizeof(Player));
	if (!player) return FALSE;

	player->bitmap = Bitmap_GetBytes("player.bmp");
	player->pos.x = 0;
	player->pos.y = 200;

	player->collisionPoints[0].x = 0;
	player->collisionPoints[0].y = 0;

	player->collisionPoints[1].x = 0;
	player->collisionPoints[1].y = SPRITE_HEIGHT - 1;

	player->collisionPoints[2].x = SPRITE_WIDTH - 1;
	player->collisionPoints[2].y = 0;

	player->collisionPoints[3].x = SPRITE_WIDTH - 1;
	player->collisionPoints[3].y = SPRITE_HEIGHT - 1;

	return TRUE;
}

void Player_Die()
{
	player->pos.x = 0;
	player->pos.y = 200;
}

BOOL Player_AdjustPos(float relativeX, float relativeY)
{
	FloatPoint newPos = { 0 };
	newPos.x = relativeX + player->pos.x;
	newPos.y = relativeY + player->pos.y;

	POINT integerPos = { 0 };
	integerPos.x = (int)roundf(player->pos.x + relativeX);
	integerPos.y = (int)roundf(player->pos.y + relativeY);

	if (integerPos.y > level->levelHeight - SPRITE_HEIGHT)
		return TRUE;

	for (int i = 0; i < 4; i++)
	{
		FloatPoint colPoint = player->collisionPoints[i];
		if (colPoint.y + integerPos.y >= level->levelHeight || colPoint.y + integerPos.y < 0 || integerPos.x  > level->levelWidth - SPRITE_WIDTH || integerPos.x < 0)
			return TRUE;

		for (int j = 0; j < level->entityCount; j++)
		{
			POINT* entityPoints = Entities_GetEntityPoints(level->entities[j].entityId);
			if (!Vector_RectContainsPoints(entityPoints, integerPos))
			{
				free(entityPoints);
				continue;
			}
			if (level->entities[j].is_dangerous)
			{
				free(entityPoints);
				Player_Die(player);
				return FALSE;
			}
			free(entityPoints);
		}

		int adjustedY = roundUpTo(integerPos.y - colPoint.y, SPRITE_HEIGHT);
		int adjustedX = roundUpTo(integerPos.x - colPoint.x, SPRITE_WIDTH);

#if DISPLAY_COLLISION_PROCESS
		Bitmap_WriteToBuffer(details->BackBuffer, details->Width, 20, 20, adjustedX, adjustedY, calloc(20 * 20, sizeof(int)));
		StretchDIBits(details->DC,
			0, 0, details->Width, details->Height,
			0, 0, details->BitMapInfo.bmiHeader.biWidth, Abs(details->BitMapInfo.bmiHeader.biHeight),
			details->BackBuffer, &details->BitMapInfo,
			DIB_RGB_COLORS, SRCCOPY);
		Sleep(50);
#endif

		TileInfo* info = Level_GetTileAtPos(level, adjustedY, adjustedX);
		if (info->is_collidable)
			return TRUE;
	}
	player->pos.x += relativeX;
	player->pos.y += relativeY;
	return FALSE;
}

TileInfo* Player_GetTileUnderPlayer()
{
	int adjustedY = roundDownTo(player->pos.y, SPRITE_HEIGHT);
	int adjustedX = roundDownTo(player->pos.x, SPRITE_WIDTH);
	return Level_GetTileAtPos(level, adjustedY + SPRITE_HEIGHT, adjustedX);
}

void Player_Update()
{
	player->velocity.j += GRAVITY;
	TileInfo* tile = Player_GetTileUnderPlayer(player, level);
	if (tile->is_collidable && !player->isJumping)
		player->velocity.j = 0;

	if (Player_AdjustPos(0, player->velocity.j))
	{
		player->velocity.j = 0;
		player->isJumping = FALSE;
	}

	Player_AdjustPos(player->velocity.i, 0);

	if (tile->is_dangerous)
	{
		Player_Die();
		return;
	}
}

void Player_StartJump()
{
	if (!player->isJumping && Player_GetTileUnderPlayer(player, level)->is_collidable)
	{
		player->velocity.j = -PLAYER_JUMP_VELOCITY;
		player->isJumping = TRUE;
	}
}

void Player_EndJump()
{
	if (player->velocity.j < -PLAYER_JUMP_VELOCITY / 2)
		player->velocity.j = -PLAYER_JUMP_VELOCITY / 2;
}