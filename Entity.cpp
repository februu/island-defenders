#include "headers/Entity.h"
#include "headers/World.h"
#include "headers/Projectile.h"
#include "headers/Constants.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

// ============================= Entity Class =============================

Entity::Entity()
{
}

Entity::~Entity()
{
}

void Entity::createEntity(int x, int y, std::string type, std::string spriteName, int xOffset, int yOffset, int health, World *world)
{
    this->x = x;
    this->y = y;
    this->spriteName = spriteName;
    this->xOffset = xOffset;
    this->yOffset = yOffset;
    this->health = health;
    this->type = type;
    this->world = world;
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

void Entity::performAction(double time)
{
}

float Entity::getMoveX()
{
    return 0;
}

float Entity::getMoveY()
{
    return 0;
}

int Entity::getDirection()
{
    return 2;
}

// ============================= Enemy Class =============================

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::chooseNewTile(int *new_x, int *new_y)
{
    double distance[4];
    distance[0] = sqrt(((10.5 - std::max(0, y - 1)) * (10.5 - std::max(0, y - 1))) + ((10.5 - x) * (10.5 - x)));
    distance[1] = sqrt(((10.5 - y) * (10.5 - y)) + ((10.5 - std::min(MAPSIZE - 1, x + 1)) * (10.5 - std::min(MAPSIZE - 1, x + 1))));
    distance[2] = sqrt(((10.5 - std::min(MAPSIZE - 1, y + 1)) * (10.5 - std::min(MAPSIZE - 1, y + 1))) + ((10.5 - x) * (10.5 - x)));
    distance[3] = sqrt(((10.5 - y) * (10.5 - y)) + ((10.5 - std::max(0, x - 1)) * (10.5 - std::max(0, x - 1))));
    for (;;)
    {
        int shortest_distance = 10000;
        int optimal_direction;
        for (int i = 0; i < 4; i++)
        {
            if (distance[i] < shortest_distance)
            {
                shortest_distance = distance[i];
                optimal_direction = i;
            }
        }
        direction = optimal_direction;

        switch (direction)
        {
        case 0:
            *new_x = x;
            *new_y = std::min(std::max(0, y - 1), MAPSIZE - 1);
            break;
        case 1:
            *new_x = std::min(std::max(0, x + 1), MAPSIZE - 1);
            *new_y = y;
            break;
        case 2:
            *new_x = x;
            *new_y = std::min(std::max(0, y + 1), MAPSIZE - 1);
            break;
        case 3:
            *new_x = std::min(std::max(0, x - 1), MAPSIZE - 1);
            *new_y = y;
            break;
        }

        if (world->tilemap[*new_x][*new_y] != WATERTILE)
            return;

        distance[direction] = 20000;
    }
}

void Enemy::performAction(double deltaTime)
{
    timeToNextMove += deltaTime;
    // std::cout << "deltaTime=" << deltaTime << " moveX=" << moveX << " moveY=" << moveY << " timeToNextMove=" << timeToNextMove << " isMoving=" << isMoving << "\n";

    // FIXME: Add moveSpeed variable to .createEntity() function.
    double moveSpeed = 0;
    if (type == "wasp")
        moveSpeed = 0.2;
    else if (type == "slime")
        moveSpeed = 0.2;

    if (!isMoving)
    {
        if (timeToNextMove > moveSpeed)
        {
            // Chooses most optimal direction.
            int new_x, new_y;

            chooseNewTile(&new_x, &new_y);

            // ==============================

            if (world->entities[new_x * MAPSIZE + new_y] == nullptr)
            {
                world->entities[new_x * MAPSIZE + new_y] = world->entities[x * MAPSIZE + y];
                world->entities[x * MAPSIZE + y] = nullptr;
                x = new_x;
                y = new_y;
                timeToNextMove = 0;
                isMoving = true;

                switch (direction)
                {
                case 0:
                    moveX = -16.f;
                    moveY = 8.f;
                    break;
                case 1:
                    moveX = -15.f;
                    moveY = -8.f;
                    break;
                case 2:
                    moveX = 16.f;
                    moveY = -8.f;
                    break;
                case 3:
                    moveX = 15.f;
                    moveY = 8.f;
                    break;
                }
            }
        }
    }
    else
    {
        deltaTime *= 4; // speed of animation
        switch (direction)
        {
        case 0:
            moveX += 16 * deltaTime;
            moveY -= 8 * deltaTime;
            break;
        case 1:
            moveX += 15 * deltaTime;
            moveY += 8 * deltaTime;
            break;
        case 2:
            moveX -= 16 * deltaTime;
            moveY += 8 * deltaTime;
            break;
        case 3:
            moveX -= 15 * deltaTime;
            moveY -= 8 * deltaTime;
            break;
        }
        if (timeToNextMove > 0.25)
        {
            timeToNextMove = 0;
            isMoving = false;
            moveX = 0.f;
            moveY = 0.f;
        }
    }
}

float Enemy::getMoveX()
{
    return moveX;
}
float Enemy::getMoveY()
{
    return moveY;
}

int Enemy::getDirection()
{
    return direction;
}

// ============================= Building Class =============================

Building::Building()
{
}

Building::~Building()
{
}

void Building::performAction(double deltaTime)
{
    if (type == "base")
        return;

    timeToNextMove += deltaTime;
    if (timeToNextMove > 1)
        for (int target_y = 0; target_y < MAPSIZE; target_y++)     // height
            for (int target_x = 0; target_x < MAPSIZE; target_x++) // width
                if (world->getEntity(target_y, target_x))
                    if (world->getEntity(target_y, target_x)->getType() == "wasp" || world->getEntity(target_y, target_x)->getType() == "slime")
                    {
                        int start_x = world->game->tileSize * x - world->game->tileSize * y - world->game->tileSize + world->game->mapXOffset + world->game->tileSize / 2;
                        int start_y = (world->game->tileSize * y + world->game->tileSize * x) / 2 + world->game->mapYOffset;
                        int dest_x = world->game->tileSize * target_x - world->game->tileSize * target_y - world->game->tileSize + world->game->mapXOffset + world->game->tileSize / 2;
                        int dest_y = (world->game->tileSize * target_y + world->game->tileSize * target_x) / 2 + world->game->mapYOffset;
                        Projectile projectile(start_x, start_y, dest_x, dest_y, "projectile");
                        world->game->addProjectile(projectile);
                        timeToNextMove = 0;
                        return;
                    }
}