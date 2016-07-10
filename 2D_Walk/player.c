#include "player.h"

BOOL changePos(Player* player, Level* level, int newX, int newY)
{
	int roundedY = roundUpTo(newY, SPRITE_HEIGHT);
	if (roundedY >= level->levelHeight || roundedY < 0 || newX >= level->levelWidth || newX < 0)
		return TRUE;

	TileInfo* tileAtPos = getTileAtPos(level, roundedY, newX);

	if (tileAtPos->is_collidable)
	{
		return TRUE;
	}
	else if (tileAtPos->is_dangerous)
	{
		player->pos.x = 0;
		player->pos.y = 0;
		return FALSE;
	}

	player->pos.x = newX;
	player->pos.y = roundedY;
	return FALSE;
}