#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "headers\AssetManager.h"

AssetManager::AssetManager()
{

    // Loads all textures and converts them into sprites.
    std::vector<std::string> fileList{"bg", "tile", "tile_grass", "tile_rock",
                                      "tile_water", "selected_tile", "overlay",
                                      "cursor", "mountain", "water_left", "water_right", "main_base"};
    for (std::string i : fileList)
    {
        sf::Texture texture;
        texture.loadFromFile("assets/textures/" + i + ".png");
        this->textures.insert({i, texture});
        sf::Sprite sprite;
        sprite.setTexture(this->textures[i]);

        this->sprites.insert({i, sprite});
    }

    // Loads all fonts.
    sf::Font font;
    font.loadFromFile("assets/fonts/arcade.ttf");
    this->fonts.insert({"arcade", font});
    font.loadFromFile("assets/fonts/arcade_outline.ttf");
    this->fonts.insert({"arcade_outline", font});
    font.loadFromFile("assets/fonts/pixelmix.ttf");
    this->fonts.insert({"pixelmix", font});
    font.loadFromFile("assets/fonts/pixelmix_bold.ttf");
    this->fonts.insert({"pixelmix_bold", font});
}

AssetManager::~AssetManager()
{
}

sf::Sprite *AssetManager::getSprite(std::string id)
{
    return &(this->sprites[id]);
}

sf::Font *AssetManager::getFont(std::string fontName)
{
    return &this->fonts[fontName];
}
