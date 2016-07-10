#include "enemy.h"

void setupEnemies(Level* level)
{
	for (int i = 0; i < level->tileCount; i++)
	{
		if (!level->tiles[i].is_enemy) continue;
		level->tiles[i].pos.x = level->tiles[i].movementData1.x;
		level->tiles[i].pos.y = level->tiles[i].movementData1.y;
	}
}

void updateEnemies(Level* level, int speedMultiplier)
{
	for (int i = 0; i < level->tileCount; i++)
	{
		if (!level->tiles[i].is_enemy) continue;

		if (level->tiles[i].movingLeft && level->tiles[i].pos.x == level->tiles[i].movementData2.x && level->tiles[i].pos.y == level->tiles[i].movementData2.y)
		{
			level->tiles[i].movingLeft = FALSE;
		}
		else if (!level->tiles[i].movingLeft && level->tiles[i].pos.x == level->tiles[i].movementData1.x && level->tiles[i].pos.y == level->tiles[i].movementData1.y)
		{
			level->tiles[i].movingLeft = TRUE;
		}

		Slope slope;
		if (level->tiles[i].movingLeft)
			slope = getSlope(level->tiles[i].pos, level->tiles[i].movementData2);
		else
			slope = getSlope(level->tiles[i].pos, level->tiles[i].movementData1);

		level->tiles[i].pos.x += slope.dx*speedMultiplier;
		level->tiles[i].pos.y += slope.dy*speedMultiplier;
	}
}

void displayEnemies(Level* level, int* buffer, int bufferWidth)
{
	for (int i = 0; i < level->tileCount; i++)
		if (level->tiles[i].is_enemy)
			writeTile(buffer, bufferWidth, level->tiles[i].pos.x, level->tiles[i].pos.y, findTile(level, level->tiles[i].identifier));
}