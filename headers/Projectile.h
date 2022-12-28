#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "Entity.h"

class Projectile
{
private:
    double x, y, travelTime;
    std::string spriteName;
    double start_x,
        start_y,
        dest_x,
        dest_y;
    int enemy_x, enemy_y;
    World *world;

public:
    Projectile(int x, int y, int dest_x, int dest_y, std::string spriteName, Entity *entity);
    ~Projectile();
    bool update(double deltaTime);
    sf::Vector2f getPosition();
    std::string getSpriteName();
};