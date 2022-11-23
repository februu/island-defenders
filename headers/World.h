#pragma once

#include <vector>
#include "SFML/Graphics.hpp"

// ========================= Game Object Class =========================

class Entity
{
private:
    int x, y, xOffset, yOffset;
    int health;
    std::string spriteName, type;

public:
    Entity();
    ~Entity();
    void createEntity(std::string type, std::string spriteName, int xOffset, int yOffset, int health);
    std::string getSpriteName();
    std::string getType();
    int getXOffset();
    int getYOffset();
};

// ========================= World Class =========================

class World
{
private:
    Entity world[20][20]; // TODO: Add enemies.
    int income = 0;       // TODO: set starting value.

public:
    int tilemap[20][20];
    World();
    ~World();
    void createNewWorld();
    Entity getEntity(int x, int y);
};
