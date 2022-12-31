#include <string>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "headers/Projectile.h"
#include "headers/World.h"

Projectile::Projectile(int x, int y, int dest_x, int dest_y, std::string spriteName, Entity *enemy)
{
    this->x = x;
    this->y = y;
    this->start_x = x;
    this->start_y = y;
    this->spriteName = spriteName;
    this->dest_x = dest_x;
    this->dest_y = dest_y;
    this->enemy_x = enemy->getX();
    this->enemy_y = enemy->getY();
    this->world = enemy->getWorld();
}

Projectile::~Projectile()
{
}

bool Projectile::update(double deltaTime)
{
    travelTime += deltaTime;
    float tx = dest_x - start_x;
    float ty = dest_y - start_y;
    float dist = sqrt(tx * tx + ty * ty);

    x += tx / dist * 1000 * deltaTime;
    y += ty / dist * 1000 * deltaTime;

    if (fabs(sqrt(pow((dest_x - x), 2) + pow((dest_y - y), 2))) < 20)
    {
        if (world->getEntity(enemy_x, enemy_y))
        {
            if (spriteName == "projectile_ice")
                world->getEntity(enemy_x, enemy_y)->setActionMultipier(2);
            else if (spriteName == "projectile_magic")
            {
                world->getEntity(enemy_x, enemy_y)->setTimeToNextMove(-3);
                world->getEntity(enemy_x, enemy_y)->setIsMoving(false);
            }
            else if (spriteName == "projectile_fire")
                world->getEntity(enemy_x, enemy_y)->decreaseHealth(2);
            else
                world->getEntity(enemy_x, enemy_y)->decreaseHealth(1);
        }
        return true;
    }

    if (travelTime > 5)
        return true;

    return false;
}

sf::Vector2f Projectile::getPosition()
{
    return sf::Vector2f(x, y);
}

std::string Projectile::getSpriteName()
{
    return spriteName;
}
