#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <SFML\Graphics.hpp>
#include "headers\AssetManager.h"

AssetManager::AssetManager()
{
    std::vector<std::string> fileList{"tile", "tile_grass", "tile_rock", "tile_water", "selected_tile", "pause", "cursor", "mountain", "water_left", "water_right"};

    for (std::string i : fileList)
    {
        sf::Texture texture;
        texture.loadFromFile("assets/" + i + ".png");
        this->textures.insert({i, texture});
        sf::Sprite sprite;
        sprite.setTexture(this->textures[i]);

        this->sprites.insert({i, sprite});
    }

    // Font
    this->font.loadFromFile("assets/font.ttf");
}

AssetManager::~AssetManager()
{
}

sf::Sprite *AssetManager::getSprite(std::string id)
{
    return &(this->sprites[id]);
}

sf::Font *AssetManager::getFont()
{
    return &this->font;
}
