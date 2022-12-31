#include <SFML/Graphics.hpp>
#include <algorithm>
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

void World::createNewWorld()
{
    srand((unsigned)time(NULL));

    for (int i = 0; i < MAPSIZE * MAPSIZE; i++)
        entities[i] = nullptr; // NULLPTR

    // Generates tilemap.
    for (int i = 0; i < MAPSIZE; i++)
        for (int j = 0; j < MAPSIZE; j++)
            this->tilemap[i][j] = 0;

    // Adds tiles with grass and rocks.
    for (int i = 40 + rand() % 80; i > 0; i--)
    {
        this->tilemap[rand() % MAPSIZE][rand() % MAPSIZE] = rand() % 3 + 1; // Sets array to REGULARTILE, GRASSTILE, ROCKTILE or PLANTTILE
    }

    for (int i = 0; i < 8; i++)
    {
        this->tilemap[rand() % MAPSIZE][rand() % MAPSIZE] = MINETILE; // Sets array to REGULARTILE, GRASSTILE, ROCKTILE, PLANTTILE or MINETILE.
    }

    // Adds water pools in available spaces.
    for (int i = 4 + rand() % 5; i > 0; i--) // Amount of pools
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

    // Generates tilemap.
    for (int i = 9; i < 13; i++)
        for (int j = 9; j < 13; j++)
            this->tilemap[i][j] = rand() % 3 + 1;

    // Creates entities.
    Building *main_base = new Building();
    main_base->createEntity(10, 10, "base", "main_base", -11, 25, 9999, this);
    entities[10 * MAPSIZE + 10] = main_base;

    Building *base_fragments1 = new Building();
    base_fragments1->createEntity(10, 11, "base", "", 0, 0, 9999, this);
    entities[10 * MAPSIZE + 11] = base_fragments1;

    Building *base_fragments2 = new Building();
    base_fragments2->createEntity(10, 11, "base", "", 0, 0, 9999, this);
    entities[11 * MAPSIZE + 10] = base_fragments2;

    Building *base_fragments3 = new Building();
    base_fragments3->createEntity(10, 11, "base", "", 0, 0, 9999, this);
    entities[11 * MAPSIZE + 11] = base_fragments3;

    for (int i = 7 + rand() % 5; i > 0; i--)
    {
        int x, y;
        do
        {
            x = rand() % MAPSIZE;
            y = rand() % MAPSIZE;
        } while (getEntity(x, y) || tilemap[x][y] == WATERTILE || tilemap[x][y] == MINETILE);

        Building *tree = new Building();
        tree->createEntity(x, y, "tree", "tree", 8, 15, 9999, this);
        entities[x * MAPSIZE + y] = tree;
        tilemap[x][y] = TREETILE;
    }
}

Entity *World::getEntity(int x, int y)
{
    return entities[x * MAPSIZE + y];
}

bool World::placeNewBuilding(int x, int y, std::string type, std::string spriteName, int xOffset, int yOffset, int health)
{
    if (!entities[x * MAPSIZE + y])
    {
        Building *building = new Building();
        building->createEntity(x, y, type, spriteName, xOffset, yOffset, health, this);
        entities[x * MAPSIZE + y] = building;
        return true;
    }
    return false;
}

void World::destroyEntity(int x, int y)
{
    if (getEntity(x, y))
    {

        // Checks if entity is an enemy - if yes, add crytals.
        if (getEntity(x, y)->getType() == "wasp" || getEntity(x, y)->getType() == "slime")
        {
            game->crystals += 5;
            game->monstersKilled++;
            game->crystalsEarned += 5;
        }
        else if (getEntity(x, y)->getType() == "turret")
        {
            game->destroyTurretParticle(x, y);
        }

        // Deletes entity.
        float scale = 1.5f;
        int mapX = game->tileSize * x - game->tileSize * y - game->tileSize + game->mapXOffset + 3 * game->tileScale * scale;
        int mapY = (game->tileSize * y + game->tileSize * x) / 2 + game->mapYOffset - game->tileScale * scale - 4 * game->tileScale;
        Particle particle(mapX, mapY, "particles/explosion", 9, scale);
        game->addParticle(particle);
        delete getEntity(x, y);
        entities[x * MAPSIZE + y] = nullptr;
    }
}