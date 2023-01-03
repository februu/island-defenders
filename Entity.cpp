#include <algorithm>
#include <cmath>
#include <vector>

#include "headers/Entity.h"
#include "headers/World.h"
#include "headers/Projectile.h"
#include "headers/Particle.h"
#include "headers/Constants.h"

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

int Entity::getX()
{
    return x;
}

int Entity::getY()
{
    return y;
}

float Entity::getMoveX()
{
    return 0;
}

float Entity::getMoveY()
{
    return 0;
}

World *Entity::getWorld()
{
    return world;
}

double Entity::getTimeToNextMove()
{
    return timeToNextMove;
}

int Entity::getDirection()
{
    return 2;
}

int Entity::getIsMoving()
{
    return 0;
}

void Entity::setIsMoving(bool set)
{
}

int Entity::getHealth()
{
    return health;
}

void Entity::decreaseHealth(int damage)
{
    health -= damage;
    if (type == "base")
        world->game->hearts--;
    if (health <= 0)
    {
        world->destroyEntity(x, y);
    }
}

void Entity::setTimeToNextMove(double time)
{
    this->timeToNextMove = time;
}

bool Entity::checkIfPathEmpty()
{
    return false;
}

void Entity::findPath()
{
}

void Entity::setActionMultipier(int multiplier)
{
    actionTimeMultiplier = multiplier;
}

void Entity::stun()
{
    if (!wasStunned)
    {
        setTimeToNextMove(-3);
        setIsMoving(false);
        wasStunned = true;
    }
}

// ============================= Enemy Class =============================

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

sf::Vector2i getFromVector(std::vector<visitedTile> visited, sf::Vector2i vertex)
{
    for (auto i = visited.begin(); i != visited.end(); ++i)
    {
        if ((*i).vertex == vertex)
            return (*i).origin;
    }
}

bool checkIfNotInVector(std::vector<visitedTile> queue, sf::Vector2i newVertex)
{
    for (auto tile = begin(queue); tile != end(queue); ++tile)
        if (tile->vertex == newVertex)
            return false;
    return true;
}

void Enemy::findPath()
{

    dest_x = 11;
    dest_y = 11;
    std::vector<visitedTile> queue;
    std::vector<visitedTile> visited;
    int maxsteps = 5000;
    int currentX = x, currentY = y;
    visited.push_back({sf::Vector2i(currentX, currentY), sf::Vector2i(-1, -1)});
    while (maxsteps > 0)
    {
        if (checkIfNotInVector(queue, sf::Vector2i(currentX + 1, currentY)) && currentX + 1 < MAPSIZE)
            if (world->tilemap[currentX + 1][currentY] != WATERTILE && world->tilemap[currentX + 1][currentY] != TREETILE)
                queue.push_back({sf::Vector2i(currentX + 1, currentY), sf::Vector2i(currentX, currentY)});

        if (checkIfNotInVector(queue, sf::Vector2i(currentX - 1, currentY)) && currentX - 1 >= 0)
            if (world->tilemap[currentX - 1][currentY] != WATERTILE && world->tilemap[currentX - 1][currentY] != TREETILE)
                queue.push_back({sf::Vector2i(currentX - 1, currentY), sf::Vector2i(currentX, currentY)});

        if (checkIfNotInVector(queue, sf::Vector2i(currentX, currentY + 1)) && currentY + 1 < MAPSIZE)
            if (world->tilemap[currentX][currentY + 1] != WATERTILE && world->tilemap[currentX][currentY + 1] != TREETILE)
                queue.push_back({sf::Vector2i(currentX, currentY + 1), sf::Vector2i(currentX, currentY)});

        if (checkIfNotInVector(queue, sf::Vector2i(currentX, currentY - 1)) && currentY - 1 >= 0)
            if (world->tilemap[currentX][currentY - 1] != WATERTILE && world->tilemap[currentX][currentY - 1] != TREETILE)
                queue.push_back({sf::Vector2i(currentX, currentY - 1), sf::Vector2i(currentX, currentY)});

        if (queue.size() == 0)
            break;

        currentX = queue.front().vertex.x;
        currentY = queue.front().vertex.y;
        visited.push_back(queue.front());

        queue.erase(queue.begin());
        if (currentX == dest_x && currentY == dest_y)
            break;

        maxsteps--;
    }

    while (getFromVector(visited, sf::Vector2i(currentX, currentY)).x != -1)
    {
        pathBFS.push_back(sf::Vector2i(currentX, currentY));
        sf::Vector2i currentVector = getFromVector(visited, sf::Vector2i(currentX, currentY));
        currentX = currentVector.x;
        currentY = currentVector.y;
    }
}

bool Enemy::checkForNearBuildings()
{
    for (int target_y = std::max(y - 1, 0); target_y <= std::min(y + 1, MAPSIZE - 1); target_y++)     // height
        for (int target_x = std::max(x - 1, 0); target_x <= std::min(x + 1, MAPSIZE - 1); target_x++) // width
        {
            if (target_x == x && target_y == y)
                continue;
            if (world->getEntity(target_x, target_y))
                if (world->getEntity(target_x, target_y)->getType() == "turret" || world->getEntity(target_x, target_y)->getType() == "base" || world->getEntity(target_x, target_y)->getType() == "mine")
                {
                    return true;
                }
        }
    return false;
}

void Enemy::performAction(double deltaTime)
{
    timeToNextMove += deltaTime;

    double moveSpeed = 0;
    if (type == "wasp")
        moveSpeed = 0.5 * actionTimeMultiplier;
    else if (type == "slime")
        moveSpeed = 0.8 * actionTimeMultiplier;

    if (!isMoving)
    {
        if (timeToNextMove > moveSpeed)
            if (checkForNearBuildings())
            {
                for (int target_y = std::max(y - 1, 0); target_y <= std::min(y + 1, MAPSIZE - 1); target_y++)     // height
                    for (int target_x = std::max(x - 1, 0); target_x <= std::min(x + 1, MAPSIZE - 1); target_x++) // width
                    {
                        if (target_x == x && target_y == y)
                            continue;
                        if (world->getEntity(target_x, target_y))
                            if (world->getEntity(target_x, target_y)->getType() == "turret" || world->getEntity(target_x, target_y)->getType() == "base" || world->getEntity(target_x, target_y)->getType() == "mine")
                            {
                                std::string targetType = world->getEntity(target_x, target_y)->getType();
                                world->getEntity(target_x, target_y)->decreaseHealth(1);

                                float scale = 1.5f;
                                int mapX = world->game->tileSize * target_x - world->game->tileSize * target_y - world->game->tileSize + world->game->mapXOffset + 3 * world->game->tileScale * scale;
                                int mapY = (world->game->tileSize * target_y + world->game->tileSize * target_x) / 2 + world->game->mapYOffset - world->game->tileScale * scale - 4 * world->game->tileScale;
                                Particle particle(mapX, mapY, "particles/hit", 6, scale);
                                world->game->addParticle(particle);

                                timeToNextMove = 0;
                                if (targetType == "base")
                                {
                                    this->decreaseHealth(100);
                                }
                                return;
                            }
                            else
                                continue;
                    }
            }
            else
            {
                // Chooses most optimal direction.
                if (pathBFS.empty())
                    return;
                // findPath();

                int new_x = pathBFS.back().x, new_y = pathBFS.back().y;

                if (new_x > x)
                    direction = 1;
                else if (new_x < x)
                    direction = 3;
                else if (new_y > y)
                    direction = 2;
                else if (new_y < y)
                    direction = 0;

                // ==============================

                if (!world->entities[new_x * MAPSIZE + new_y])
                {
                    pathBFS.pop_back();
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

int Enemy::getIsMoving()
{
    return isMoving;
}

void Enemy::setIsMoving(bool set)
{
    isMoving = set;
}

bool Enemy::checkIfPathEmpty()
{
    return pathBFS.empty();
}

// ============================= Building Class =============================

Building::Building()
{
}

Building::~Building()
{
}

void Building::createProjectile(int target_x, int target_y, Entity *enemy)
{
    int start_x = world->game->tileSize * x - world->game->tileSize * y - world->game->tileSize + world->game->mapXOffset + 15 * world->game->tileScale;
    int start_y = (world->game->tileSize * y + world->game->tileSize * x) / 2 + world->game->mapYOffset - 7 * world->game->tileScale;
    int dest_x = world->game->tileSize * target_x - world->game->tileSize * target_y - world->game->tileSize + world->game->mapXOffset + world->game->tileSize / 2;
    int dest_y = (world->game->tileSize * target_y + world->game->tileSize * target_x) / 2 + world->game->mapYOffset;
    std::string projectileType = "projectile";
    if (spriteName == "turret_ice")
        projectileType = "projectile_ice";
    else if (spriteName == "turret_fire")
        projectileType = "projectile_fire";
    else if (spriteName == "turret_magic")
        projectileType = "projectile_magic";
    Projectile projectile(start_x, start_y, dest_x, dest_y, projectileType, enemy);
    world->game->addProjectile(projectile);
    timeToNextMove = 0;
}

void Building::performAction(double deltaTime)
{
    if (type == "base" || type == "wall" || type == "tree")
        return;

    timeToNextMove += deltaTime;

    if (type == "mine")
    {
        if (timeToNextMove > 5)
        {
            float scale = 1.f;
            int mapX = world->game->tileSize * x - world->game->tileSize * y - world->game->tileSize + world->game->mapXOffset + 0.5 * world->game->tileSize * world->game->tileScale - world->game->tileScale * 3 * scale;
            int mapY = (world->game->tileSize * y + world->game->tileSize * x) / 2 + world->game->mapYOffset - world->game->tileScale * 15 * scale;
            Particle particle(mapX, mapY, "particles/gem", 7, scale);
            world->game->addParticle(particle);
            world->game->crystals += 20;
            world->game->crystalsEarned += 20;
            timeToNextMove = 0;
        }
        return;
    }

    if (timeToNextMove > 0.3)
    {

        std::vector<Entity *> enemies;

        // Puts nearest enemies into a vector.
        for (int i = 0; i < 3; i++)
        {
            for (int target_y = std::max(y - 1 - i, 0); target_y <= std::min(y + 1 + i, MAPSIZE - 1); target_y++)     // height
                for (int target_x = std::max(x - 1 - i, 0); target_x <= std::min(x + 1 + i, MAPSIZE - 1); target_x++) // width
                {
                    if (target_x == x && target_y == y)
                        continue;
                    if (world->getEntity(target_x, target_y))
                        if (world->getEntity(target_x, target_y)->getType() == "wasp" || world->getEntity(target_x, target_y)->getType() == "slime")
                        {
                            enemies.push_back(world->getEntity(target_x, target_y));
                        }
                }
            if (enemies.size() > 0)
                break;
        }

        // Searches for an enemy with the shortest distance.
        int shortestDistance = 10000000;
        Entity *closestEnemy = nullptr;
        for (auto enemy = begin(enemies); enemy != end(enemies); ++enemy)
        {
            double distance = (x - (*enemy)->getX()) * (x - (*enemy)->getX()) + (y - (*enemy)->getY()) * (y - (*enemy)->getY());
            if (distance < shortestDistance)
            {
                shortestDistance = distance;
                closestEnemy = *enemy;
            }
        }

        if (closestEnemy)
            createProjectile(closestEnemy->getX(), closestEnemy->getY(), closestEnemy);

        return;
    }
}
