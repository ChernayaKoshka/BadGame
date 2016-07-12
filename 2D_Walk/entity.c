#include "entity.h"

void setupEntities(Level* level)
{
	for (int i = 0; i < level->entityCount; i++)
	{
		level->entities[i].pos.x = level->entities[i].movementData1.x;
		level->entities[i].pos.y = level->entities[i].movementData1.y;
	}
}

void updateEntities(Level* level, int speedMultiplier)
{
	for (int i = 0; i < level->entityCount; i++)
	{
		if (!level->entities[i].is_enemy) continue;

		if (level->entities[i].movingLeft && level->entities[i].pos.x == level->entities[i].movementData2.x && level->entities[i].pos.y == level->entities[i].movementData2.y)
		{
			level->entities[i].movingLeft = FALSE;
		}
		else if (!level->entities[i].movingLeft && level->entities[i].pos.x == level->entities[i].movementData1.x && level->entities[i].pos.y == level->entities[i].movementData1.y)
		{
			level->entities[i].movingLeft = TRUE;
		}

		Slope slope;
		if (level->entities[i].movingLeft)
			slope = getSlope(level->entities[i].pos, level->entities[i].movementData2);
		else
			slope = getSlope(level->entities[i].pos, level->entities[i].movementData1);

		level->entities[i].pos.x += slope.dx*speedMultiplier;
		level->entities[i].pos.y += slope.dy*speedMultiplier;
	}
}

void displayEntities(Level* level, int* buffer, int bufferWidth)
{
	for (int i = 0; i < level->entityCount; i++)
		writeBitmap(buffer, bufferWidth, level->entities[i].pos.x, level->entities[i].pos.y, level->entities[i].bitmap);
}

POINT* getEntityPoints(Level* level, char entityId)
{
	POINT* points = calloc(4, sizeof(POINT));

	for (int i = 0; i < level->entityCount; i++)
	{
		if (level->entities[i].entityId != entityId) continue;

		points[0].x = level->entities[i].pos.x + 0;
		points[0].y = level->entities[i].pos.y + 0;

		points[1].x = level->entities[i].pos.x + 0;
		points[1].y = level->entities[i].pos.y + SPRITE_HEIGHT - 1;

		points[2].x = level->entities[i].pos.x + SPRITE_WIDTH - 1;
		points[2].y = level->entities[i].pos.y + 0;

		points[3].x = level->entities[i].pos.x + SPRITE_WIDTH - 1;
		points[3].y = level->entities[i].pos.y + SPRITE_HEIGHT - 1;
	}

	return points;
}