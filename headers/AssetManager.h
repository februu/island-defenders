#pragma once

#include <map>
#include <string>
#include <SFML\Graphics.hpp>

class AssetManager
{
private:
    sf::Texture txt;
    std::map<std::string, sf::Texture> textures;
    sf::Font font;

public:
    AssetManager();
    virtual ~AssetManager();

    // Functions
    sf::Texture *getTexture(std::string id);
    sf::Font *getFont();
};
