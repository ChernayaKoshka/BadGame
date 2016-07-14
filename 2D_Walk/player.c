#include "player.h"

extern Level* level;

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

BOOL Player_ChangePos(float relativeX, float relativeY)
{
	FloatPoint newPos = { 0 };
	newPos.x = relativeX + player->pos.x;
	newPos.y = relativeY + player->pos.y;

	POINT integerPos = { 0 };
	integerPos.x = (int)roundf(player->pos.x + relativeX);
	integerPos.y = (int)roundf(player->pos.y + relativeY);

	for (int i = 0; i < 4; i++)
	{
		FloatPoint colPoint = player->collisionPoints[i];
		if (colPoint.y + integerPos.y >= level->levelHeight || colPoint.y + integerPos.y < 0 || integerPos.x  > level->levelWidth - SPRITE_WIDTH || integerPos.x < 0)
			return TRUE;

		for (int j = 0; j < level->entityCount; j++)
		{
			POINT* entityPoints = Entities_GetEntityPoints(level->entities[j].entityId);
			if (!rectContainsPoint(entityPoints, integerPos))
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
	return Level_GetTileAtPos(level, player->pos.y + SPRITE_HEIGHT, player->pos.x);
}

void Player_Update()
{
	TileInfo* tile = Player_GetTileUnderPlayer(player, level);

	Player_ChangePos(0, GRAVITY);

	switch (player->horizontalDirection)
	{
	case LEFT:
		Player_ChangePos(-PLAYER_HORIZONTAL_SPEED, 0);
		break;
	case RIGHT:
		Player_ChangePos(PLAYER_HORIZONTAL_SPEED, 0);
		break;
	default:
		break;
	}

	player->isJumping = FALSE;

	if (tile->is_dangerous)
	{
		Player_Die();
		return;
	}
}

void Player_Jump()
{
	if (!player->isJumping && Player_GetTileUnderPlayer(player, level)->is_collidable)
	{
		player->jumpHeight = PLAYER_JUMP_HEIGHT;
		Player_ChangePos(0, -PLAYER_JUMP_HEIGHT);
		player->isJumping = TRUE;
	}
}