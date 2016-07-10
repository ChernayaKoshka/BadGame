#pragma once
#include "level.h"
#include "math_custom.h"
#include <assert.h>

void setupEnemies(Level* level);
void updateEnemies(Level* level, int speedMultiplier);
void displayEnemies(Level* level, int* buffer, int bufferWidth);