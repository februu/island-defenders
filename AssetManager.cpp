#include <iostream>
#include <map>
#include <string>
#include <regex>
#include <vector>
#include <SFML\Graphics.hpp>
#include "headers\AssetManager.h"

AssetManager::AssetManager()
{
    std::vector<std::string> fileList{"assets/tile.png", "assets/selected_tile.png",
                                      "assets/pause.png", "assets/cursor.png"};

    for (std::string i : fileList)
    {
        sf::Texture texture;
        texture.loadFromFile(i);
        i.erase(i.find("assets/"), 7);
        i.erase(i.find(".png"), 4);
        this->textures.insert({i, texture});
    }

    this->font.loadFromFile("assets/font.ttf");
}

AssetManager::~AssetManager()
{
}

sf::Texture *AssetManager::getTexture(std::string id)
{
    return &(this->textures[id]);
}

sf::Font *AssetManager::getFont()
{
    return &this->font;
}
