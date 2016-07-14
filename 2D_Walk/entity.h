#pragma once

#include <Windows.h>
#include "level.h"
#include "math_custom.h"
#include "drawing.h"
#include "screen.h"
#include "file.h"

#define DEFAULT_ENTITY_SPEED 1.0f/60.0f

BOOL Entities_Init();
void Enitites_Update();
void Enities_WriteToBuffer();
POINT* Entities_GetEntityPoints(char entityId);