#include "player.h"
#include "globals.h"
#include "screen.h"

extern POINT dbgTilePos;
extern WindowDetails* details;
extern TileInfo* dbgTile;

BOOL changePos(Player* player, Level* level, int newX, int newY)
{
	for (int i = 0; i < 4; i++)
	{
		POINT colPoint = player->collisionPoints[i];
		if (colPoint.y + newY >= level->levelHeight || colPoint.y + newY < 0 || newX > level->levelWidth - SPRITE_WIDTH || newX < 0)
			return TRUE;
		int test = roundUpTo(newY - colPoint.y, SPRITE_HEIGHT);
		int test2 = roundUpTo(newX - colPoint.x, SPRITE_WIDTH);

		dbgTilePos.y = test;
		dbgTilePos.x = test2;

		writeTile(details->BackBuffer, details->Width, dbgTilePos.x, dbgTilePos.y, dbgTile);
		StretchDIBits(details->DC,
			0, 0, details->Width, details->Height,
			0, 0, details->BitMapInfo.bmiHeader.biWidth, Abs(details->BitMapInfo.bmiHeader.biHeight),
			details->BackBuffer, &details->BitMapInfo,
			DIB_RGB_COLORS, SRCCOPY);
		Sleep(250);

		TileInfo* info = getTileAtPos(level, test, test2);
		if (info->is_collidable)
			return TRUE;
	}
	player->pos.x = newX;
	player->pos.y = newY;
}

TileInfo* getTileUnderPlayer(Player* player, Level* level)
{
	return getTileAtPos(level, player->pos.y + SPRITE_HEIGHT, player->pos.x);
}

void killPlayer(Player* player)
{
	player->pos.x = 0;
	player->pos.y = 0;
}

void updatePlayer(Player* player, Level* level, unsigned long step)
{
	TileInfo* tile = getTileUnderPlayer(player, level);

	if (step % 200 == 0)
	{
		changePos(player, level, player->pos.x, player->pos.y + 10);
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