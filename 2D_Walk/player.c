#include "player.h"

#if DISPLAY_COLLISION_PROCESS
extern POINT dbgTilePos;
extern WindowDetails* details;
extern TileInfo* dbgTile;
#endif

void killPlayer(Player* player)
{
	player->pos.x = 0;
	player->pos.y = 200;
}

BOOL changePos(Player* player, Level* level, int newX, int newY)
{
	POINT newPos = { 0 };
	newPos.x = newX;
	newPos.y = newY;

	for (int i = 0; i < 4; i++)
	{
		POINT colPoint = player->collisionPoints[i];
		if (colPoint.y + newY >= level->levelHeight || colPoint.y + newY < 0 || newX > level->levelWidth - SPRITE_WIDTH || newX < 0)
			return TRUE;

		for (int j = 0; j < level->entityCount; j++)
		{
			POINT* entityPoints = getEntityPoints(level, level->entities[j].entityId);
			if (!rectContainsPoint(entityPoints, newPos))
			{
				free(entityPoints);
				continue;
			}
			if (level->entities[j].is_dangerous)
			{
				free(entityPoints);
				killPlayer(player);
				return FALSE;
			}
			free(entityPoints);
		}

		int adjustedY = roundUpTo(newY - colPoint.y, SPRITE_HEIGHT);
		int adjustedX = roundUpTo(newX - colPoint.x, SPRITE_WIDTH);

#if DISPLAY_COLLISION_PROCESS
		dbgTilePos.y = adjustedY;
		dbgTilePos.x = adjustedX;

		writeBitmap(details->BackBuffer, details->Width, dbgTilePos.x, dbgTilePos.y, dbgTile->bitmap);
		StretchDIBits(details->DC,
			0, 0, details->Width, details->Height,
			0, 0, details->BitMapInfo.bmiHeader.biWidth, Abs(details->BitMapInfo.bmiHeader.biHeight),
			details->BackBuffer, &details->BitMapInfo,
			DIB_RGB_COLORS, SRCCOPY);
		Sleep(50);
#endif

		TileInfo* info = getTileAtPos(level, adjustedY, adjustedX);
		if (info->is_collidable)
			return TRUE;
	}
	player->pos.x = newX;
	player->pos.y = newY;
	return FALSE;
}

TileInfo* getTileUnderPlayer(Player* player, Level* level)
{
	return getTileAtPos(level, player->pos.y + SPRITE_HEIGHT, player->pos.x);
}

void updatePlayer(Player* player, Level* level, unsigned long step)
{
	TileInfo* tile = getTileUnderPlayer(player, level);

	if (step % 200 == 0)
	{
		changePos(player, level, player->pos.x, player->pos.y + GRAVITY);
	}

	if (step % 50 == 0)
		switch (player->horizontalDirection)
		{
		case LEFT:
			changePos(player, level, player->pos.x - PLAYER_HORIZONTAL_SPEED, player->pos.y);
			break;
		case RIGHT:
			changePos(player, level, player->pos.x + PLAYER_HORIZONTAL_SPEED, player->pos.y);
			break;
		default:
			break;
		}

	if (step % 300 == 0)
	{
		player->isJumping = FALSE;
	}

	if (tile->is_dangerous)
	{
		killPlayer(player);
		return;
	}
}