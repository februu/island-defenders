#pragma once
#include "Constants.h"
#include "Entity.h"

void loadSettings(int settings[3]);
void loadSaveFile();
void createSaveFile(int tilemap[MAPSIZE][MAPSIZE], Entity *entities[MAPSIZE * MAPSIZE], long double timePassed, int hearts, int crystals, int monstersKilled, int difficulty);
bool loadSaveFile(World *world, long double *timePassed, int *hearts, int *crystals, int *monstersKilled);