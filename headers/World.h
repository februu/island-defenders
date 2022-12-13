#pragma once

#include <vector>
#include "SFML/Graphics.hpp"
#include "Entity.h"
#include "Constants.h"
#include "Game.h"

class Game;

class Entity;

class World
{
private:
    // TODO: Add enemies.
    int income = 0; // TODO: set starting value.

public:
    Game *game;
    Entity *entities[MAPSIZE * MAPSIZE];
    int tilemap[MAPSIZE][MAPSIZE];
    World(Game *game);
    ~World();
    void createNewWorld();
    Entity *getEntity(int x, int y);
    void placeNewBuilding(int x, int y, std::string type, std::string spriteName, int xOffset, int yOffset, int health);
};
