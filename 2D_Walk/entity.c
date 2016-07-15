#include "entity.h"

extern Level* level;
extern WindowDetails* details;

BOOL Entities_Init()
{
	FILE* fp;
	fopen_s(&fp, level->entityFile, "rb");
	if (!fp)
	{
		wchar_t* unableToOpenFmt = L"Unable to open %hs!";
		wchar_t dbgData[512];

		swprintf_s(dbgData, sizeof(dbgData) / sizeof(wchar_t), unableToOpenFmt, level->entityFile);
		OutputDebugString(dbgData);
		assert(fp);
	}

	long entityFileSize = File_GetSize(fp);
	int numEntities = entityFileSize / sizeof(EntityInfo);
	EntityInfo* entities = calloc(numEntities, sizeof(EntityInfo));
	fread_s(entities, entityFileSize, sizeof(EntityInfo), numEntities, fp);
	fclose(fp);

	level->entityCount = numEntities;
	for (int i = 0; i < numEntities; i++)
		entities[i].bitmap = Bitmap_GetBytes(entities[i].spritePath);
	level->entities = entities;

	for (int i = 0; i < level->entityCount; i++)
	{
		level->entities[i].pos.x = level->entities[i].movementData1.x;
		level->entities[i].pos.y = level->entities[i].movementData1.y;
	}

	return TRUE;
}

void Enitites_Update()
{
	for (int i = 0; i < level->entityCount; i++)
	{
		if (!level->entities[i].is_enemy) continue;

		if (level->entities[i].movingLeft && (int)level->entities[i].pos.x == level->entities[i].movementData2.x && (int)level->entities[i].pos.y == level->entities[i].movementData2.y)
		{
			level->entities[i].movingLeft = FALSE;
		}
		else if (!level->entities[i].movingLeft && (int)level->entities[i].pos.x == level->entities[i].movementData1.x && (int)level->entities[i].pos.y == level->entities[i].movementData1.y)
		{
			level->entities[i].movingLeft = TRUE;
		}

		Slope slope;
		POINT integerPoint = { (int)level->entities[i].pos.x,(int)level->entities[i].pos.y };
		if (level->entities[i].movingLeft)
			slope = getSlope(integerPoint, level->entities[i].movementData2);
		else
			slope = getSlope(integerPoint, level->entities[i].movementData1);

		level->entities[i].pos.x += slope.dx*DEFAULT_ENTITY_SPEED;
		level->entities[i].pos.y += slope.dy*DEFAULT_ENTITY_SPEED;
	}
}

void Enities_WriteToBuffer()
{
	for (int i = 0; i < level->entityCount; i++)
		Bitmap_WriteToBuffer(details->BackBuffer, details->Width, SPRITE_WIDTH, SPRITE_HEIGHT, level->entities[i].pos.x, level->entities[i].pos.y, level->entities[i].bitmap);

	for (int i = 0; i < level->entityCount; i++)
	{
		FloatPoint* colPoints = Entities_GetEntityPoints(level->entities[i].entityId);
		for (int j = 0; j < 4; j++)
			Plot(colPoints[j].x, colPoints[j].y, 0x00FFFFFF, details->BackBuffer, details->Width);
		free(colPoints);
	}
}

EntityInfo* Entities_Locate(char entityId)
{
	for (int i = 0; i < level->entityCount; i++)
	{
		if (level->entities[i].entityId != entityId) continue;
		return level->entities + i;
	}
	return NULL;
}

FloatPoint* Entities_GetEntityPoints(char entityId)
{
	FloatPoint* points = calloc(4, sizeof(POINT));

	EntityInfo* entity = Entities_Locate(entityId);

	if (entity == NULL) return NULL;

	points[0].x = entity->pos.x + 0;
	points[0].y = entity->pos.y + 0;

	points[1].x = entity->pos.x + SPRITE_WIDTH - 1;
	points[1].y = entity->pos.y + 0;

	points[2].x = entity->pos.x + 0;
	points[2].y = entity->pos.y + SPRITE_HEIGHT - 1;

	points[3].x = entity->pos.x + SPRITE_WIDTH - 1;
	points[3].y = entity->pos.y + SPRITE_HEIGHT - 1;

	return points;
}