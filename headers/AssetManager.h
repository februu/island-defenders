#pragma once

#include <map>
#include <string>
#include <SFML\Graphics.hpp>

class AssetManager
{
private:
    sf::Texture txt;
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::Sprite> sprites;
    sf::Font font;

public:
    AssetManager();
    virtual ~AssetManager();

    // Functions
    sf::Sprite *getSprite(std::string id);
    sf::Font *getFont();
};
