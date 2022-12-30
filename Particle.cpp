#include <SFML/Graphics.hpp>
#include "headers/Particle.h"
#include "headers/Constants.h"
#include "headers/AssetManager.h"

Particle::Particle(int x, int y, std::string spriteName, int maxframe, float scale)
{
    this->x = x;
    this->y = y;
    this->spriteName = spriteName;
    this->maxframe = maxframe;
    this->scale = scale;
}

Particle::~Particle() {}

bool Particle::update(double deltaTime)
{
    if (time > 0.05) // Animation speed.
    {
        if (frame > maxframe)
            return true;
        else
        {
            frame++;
            time = 0;
        }
    }
    else
        time += deltaTime;
    return false;
}

sf::Vector2i Particle::getPosition()
{
    return sf::Vector2i(x, y);
}

std::string Particle::getSpriteName()
{
    return spriteName + std::to_string(frame);
}

float Particle::getScale()
{
    return scale;
}
