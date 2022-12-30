#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Particle
{
private:
    int x, y;
    double speed = 0.25, time = 0;
    float scale;
    int frame = 0, maxframe;
    std::string spriteName;

public:
    Particle(int x, int y, std::string spriteName, int maxframe, float scale);
    ~Particle();
    bool update(double deltaTime);
    sf::Vector2i getPosition();
    float getScale();
    std::string getSpriteName();
};