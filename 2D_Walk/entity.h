#pragma once

#include <Windows.h>
#include "level.h"
#include "math_custom.h"
#include "drawing.h"

void setupEntities(Level* level);
void updateEntities(Level* level, int speedMultiplier);
void displayEntities(Level* level, int* buffer, int bufferWidth);
POINT* getEntityPoints(Level* level, char entityId);