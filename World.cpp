#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <string>

#include "headers/World.h"
#include "headers/AssetManager.h"
#include "headers/Entity.h"
#include "headers/Constants.h"
#include "headers/Projectile.h"

// ========================= World Class =========================

World::World(Game *game)
{
    this->game = game;
}

World::~World()
{
    for (int i = 0; i < MAPSIZE * MAPSIZE; i++)
        delete entities[i];
}

// TODO: World generation. Add conditions for tiles, mines, water etc.
void World::createNewWorld()
{
    srand((unsigned)time(NULL));

    for (int i = 0; i < MAPSIZE * MAPSIZE; i++)
        entities[i] = nullptr;

    // Generates tilemap.
    for (int i = 0; i < MAPSIZE; i++)
        for (int j = 0; j < MAPSIZE; j++)
            this->tilemap[i][j] = 0;

    // Adds tiles with grass and rocks.
    for (int i = 40 + rand() % 80; i > 0; i--)
    {
        this->tilemap[rand() % MAPSIZE][rand() % MAPSIZE] = rand() % 3 + 1; // Sets array to REGULARTILE, GRASSTILE, ROCKTILE or PLANTTILE
    }

    for (int i = 0; i < 4; i++)
    {
        this->tilemap[rand() % MAPSIZE][rand() % MAPSIZE] = MINETILE; // Sets array to REGULARTILE, GRASSTILE, ROCKTILE, PLANTTILE or MINETILE.
    }

    // Adds water pools in available spaces.
    for (int i = 2 + rand() % 4; i > 0; i--) // Amount of pools
    {
        int x = rand() % MAPSIZE;
        int y = rand() % MAPSIZE;
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
                    this->tilemap[std::max(std::min(x - 1 + i, MAPSIZE - 1), 0)][std::max(std::min(y - 1 + j, MAPSIZE - 1), 0)] = WATERTILE;
            }
    }

    // Creates entities.
    // TODO: Make map of tuples for offsets in enums.h.
    int x, y;

    for (int i = 0; i < 5; i++)
    {
        x = rand() % MAPSIZE;
        y = rand() % MAPSIZE;
        Enemy *r_wasp = new Enemy();
        r_wasp->createEntity(x, y, "wasp", "wasp_red", 5, 7, 3, this);
        entities[x * MAPSIZE + y] = r_wasp;
    }

    for (int i = 0; i < 5; i++)
    {
        x = rand() % MAPSIZE;
        y = rand() % MAPSIZE;
        Enemy *b_slime = new Enemy();
        b_slime->createEntity(x, y, "slime", "slime_blue", 8, 4, 3, this);
        entities[x * MAPSIZE + y] = b_slime;
    }

    Building *main_base = new Building();
    main_base->createEntity(10, 10, "base", "main_base", -11, 25, 20, this);
    entities[10 * MAPSIZE + 10] = main_base;
}

Entity *World::getEntity(int x, int y)
{
    return entities[x * MAPSIZE + y];
}

void World::placeNewBuilding(int x, int y, std::string type, std::string spriteName, int xOffset, int yOffset, int health)
{
    if (entities[x * MAPSIZE + y] == nullptr)
    {
        Building *building = new Building();
        building->createEntity(x, y, type, spriteName, xOffset, yOffset, health, this);
        entities[x * MAPSIZE + y] = building;
    }
    return;
}