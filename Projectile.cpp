#include <string>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "headers/Projectile.h"

Projectile::Projectile(int x, int y, int dest_x, int dest_y, std::string spriteName)
{
    this->x = x;
    this->y = y;
    this->start_x = x;
    this->start_y = y;
    this->spriteName = spriteName;
    this->dest_x = dest_x;
    this->dest_y = dest_y;

    int x_stripped = dest_x - start_x;
    int y_stripped = dest_y - start_y;

    // check if II or IV ćwiartka
    if ((x_stripped > 0 && y_stripped < 0) || (x_stripped < 0 && y_stripped > 0))
    {
        this->circle_x = (pow(x_stripped, 2) + pow(y_stripped, 2)) / (2 * (x_stripped + y_stripped));
        this->circle_y = this->circle_x;
    }
    else
    {
        this->circle_x = (pow(x_stripped, 2) + pow(y_stripped, 2)) / (2 * ((x_stripped) - (y_stripped)));
        this->circle_y = -circle_x;
    }

    this->radius = sqrt(pow(circle_x, 2) + pow(circle_y, 2));

    direction = -1;
    if (
        (x_stripped > 0 && y_stripped > 0 && x_stripped > y_stripped)       // I cwiartka
        || (x_stripped < 0 && y_stripped > 0 && -(x_stripped) < y_stripped) // II cwiartka
        || (x_stripped < 0 && y_stripped < 0 && x_stripped < y_stripped)    // III cwiartka
        || (x_stripped > 0 && y_stripped < 0 && -(x_stripped) > y_stripped) // IV cwiartka
    )
        direction = 1;

    this->startingValue = 0;
    for (;;)
    {
        if (fabs(sqrt(pow((circle_x + sin(startingValue) * radius), 2) + pow((circle_y + cos(startingValue) * radius), 2))) > 2)
        {
            this->startingValue += 0.001;
        }
        else
            break;
    }
}

Projectile::~Projectile()
{
}

bool Projectile::update(double deltaTime)
{
    travelTime += deltaTime;
    x = start_x + circle_x + sin(startingValue + (travelTime / radius * 1000) * direction) * radius;
    y = start_y + circle_y + cos(startingValue + (travelTime / radius * 1000) * direction) * radius;

    return fabs(sqrt(pow((dest_x - x), 2) + pow((dest_y - y), 2))) < 5;
}

sf::Vector2f Projectile::getPosition()
{
    return sf::Vector2f(x, y);
}

std::string Projectile::getSpriteName()
{
    return spriteName;
}
