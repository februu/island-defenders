#pragma once

#include <map>
#include <string>
#include <SFML/Graphics.hpp>

class AssetManager
{
private:
    sf::Texture txt;
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::Sprite> sprites;
    std::map<std::string, sf::Font> fonts;

public:
    AssetManager();
    virtual ~AssetManager();

    // Functions
    void addSprite(std::string path, bool full = true, std::string variant = "", int x = 0, int y = 0, int width = 0, int height = 0);
    void addParticleTexture();
    sf::Sprite *getSprite(std::string id);
    sf::Font *getFont(std::string fontName);
};
