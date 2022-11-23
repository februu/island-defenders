#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>

#include "headers/World.h"
#include "headers/AssetManager.h"

// ========================= Game Object Class =========================

Entity::Entity()
{
}

Entity::~Entity()
{
}

void Entity::createEntity(std::string type, std::string spriteName, int xOffset, int yOffset, int health)
{

    this->spriteName = spriteName;
    this->xOffset = xOffset;
    this->yOffset = yOffset;
    this->health = health;
    this->type = type;
}

std::string Entity::getSpriteName()
{
    return this->spriteName;
}

std::string Entity::getType()
{
    return this->type;
}

int Entity::getXOffset()
{
    return this->xOffset;
}

int Entity::getYOffset()
{
    return this->yOffset;
}

// ========================= World Class =========================

World::World()
{
}

World::~World()
{
}

// TODO: World generation.
void World::createNewWorld()
{
    // Clears entities.
    Entity ent;
    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 20; j++)
            this->world[i][j] = ent;

    // Generates tilemap.
    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 20; j++)
            this->tilemap[i][j] = 0;

    // Adds tiles with grass and rocks.
    for (int i = 40 + rand() % 80; i > 0; i--)
    {
        int x = rand() % 20;
        int y = rand() % 20;
        this->tilemap[rand() % 20][rand() % 20] = rand() % 2 + 1;
    }

    srand((unsigned)time(NULL));

    // Adds water pools in available spaces.
    for (int i = 2 + rand() % 4; i > 0; i--) // Amount of pools
    {
        int x = rand() % 20;
        int y = rand() % 20;
        int direction = rand() % 4; // top, right, bottom, left
        std::string waterSchematics[8] = {
            "1100011011111100",
            "1011011011100100",
            "0010011101100000",
            "0000001011110110",
            "1100111011110011",
            "0000110001110010",
            "0000011000100000",
            "0110101101100010"};
        int schematic = rand() % 8;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
            {
                if (waterSchematics[schematic][i * 4 + j] == '1')
                    this->tilemap[std::max(std::min(x - 1 + i, 19), 0)][std::max(std::min(y - 1 + j, 19), 0)] = 3;
            }
    }

    // // Places Mountains.
    // for (int i = 3 + rand() % 4; i > 0; i--)
    // {
    //     while (true)
    //     {
    //         int x = rand() % 20;
    //         int y = rand() % 20;
    //         if (this->tilemap[x][y] != 3)
    //             this->world[x][y].createEntity("mountain", "mountain", 0, 16, 10);
    //         break;
    //     };
    // }
}

Entity World::getEntity(int x, int y)
{
    return this->world[x][y];
}