#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class Projectile
{
private:
    double x, y, direction;
    std::string spriteName;
    double start_x,
        start_y,
        dest_x,
        dest_y;
    double circle_x, circle_y, radius, startingValue;
    double travelTime = 0;

public:
    Projectile(int x, int y, int dest_x, int dest_y, std::string spriteName);
    ~Projectile();
    bool update(double deltaTime);
    sf::Vector2f getPosition();
    std::string getSpriteName();
};