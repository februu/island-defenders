#include <map>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "headers\AssetManager.h"

AssetManager::AssetManager()
{
    // Loads hud sprites.
    addSprite("bg", true);
    addSprite("cursor", true);
    addSprite("info_hud", true);
    addSprite("build_hud", true);
    addSprite("build_hud_selected", true);
    addSprite("build_hud_disabled", true);
    addSprite("overlay", true);
    addSprite("gem", true);
    addSprite("heart", true);

    // Loads building sprites
    addSprite("main_base", true);
    addSprite("mine", true);
    addSprite("tree", true);

    // Loads tile sprites.
    addSprite("tile", false, "", 0, 0, 32, 26);
    addSprite("tile", false, "grass", 32, 0, 32, 26);
    addSprite("tile", false, "rocks", 64, 0, 32, 26);
    addSprite("tile", false, "plants", 96, 0, 32, 26);
    addSprite("tile", false, "mine", 128, 0, 32, 26);
    addSprite("tile", false, "hovered", 160, 0, 32, 16);
    addSprite("tile", false, "selected", 192, 0, 34, 18);
    addSprite("water", false, "", 0, 0, 32, 26);
    addSprite("water", false, "left", 32, 0, 16, 43);
    addSprite("water", false, "right", 48, 0, 16, 43);

    // Loads enemy spites.
    addSprite("slime", false, "red", 0, 0, 16, 16);
    addSprite("slime", false, "green", 16, 0, 16, 16);
    addSprite("slime", false, "blue", 32, 0, 16, 16);
    addSprite("wasp", false, "red", 0, 0, 22, 19);
    addSprite("wasp", false, "green", 22, 0, 22, 19);
    addSprite("wasp", false, "blue", 44, 0, 22, 19);

    // Loads turret sprites.
    addSprite("turret", true);
    addSprite("turret_ice", true);
    addSprite("turret_fire", true);
    addSprite("turret_magic", true);
    addSprite("turret_poison", true);

    // Loads projectile and particle sprites.
    addSprite("projectile", true);
    addSprite("projectile_ice", true);
    addSprite("projectile_fire", true);
    addSprite("projectile_magic", true);
    for (int i = 1; i <= 7; i++)
        addSprite("particles/gem" + std::to_string(i), true);
    for (int i = 1; i <= 9; i++)
        addSprite("particles/explosion" + std::to_string(i), true);
    for (int i = 1; i <= 6; i++)
        addSprite("particles/hit" + std::to_string(i), true);
    for (int i = 1; i <= 4; i++)
        addSprite("particles/shine" + std::to_string(i), true);
    for (int i = 1; i <= 5; i++)
        addSprite("particles/fire" + std::to_string(i), true);
    for (int i = 1; i <= 5; i++)
        addSprite("particles/ice" + std::to_string(i), true);
    for (int i = 1; i <= 5; i++)
        addSprite("particles/magic" + std::to_string(i), true);
    for (int i = 1; i <= 5; i++)
        addSprite("particles/poison" + std::to_string(i), true);

    // Loads all fonts.
    sf::Font font;
    font.loadFromFile("assets/fonts/arcade.ttf");
    this->fonts.insert({"arcade", font});
    font.loadFromFile("assets/fonts/pixelmix.ttf");
    this->fonts.insert({"pixelmix", font});
    font.loadFromFile("assets/fonts/pixelmix_bold.ttf");
    this->fonts.insert({"pixelmix_bold", font});

    // TODO: Load sounds.
}

AssetManager::~AssetManager()
{
}

void AssetManager::addSprite(std::string path, bool full, std::string variant, int x, int y, int width, int height)
{
    std::string name;
    if (!full && variant != "")
        name = path + "_" + variant;
    else
        name = path;

    sf::Texture texture;
    if (!full)
        texture.loadFromFile("assets/textures/" + path + ".png", sf::IntRect(x, y, width, height));
    else
        texture.loadFromFile("assets/textures/" + path + ".png");
    this->textures.insert({name, texture});

    sf::Sprite sprite;
    sprite.setTexture(textures.at(name));
    this->sprites.insert({name, sprite});
}

sf::Sprite *AssetManager::getSprite(std::string id)
{
    return &this->sprites[id];
}

sf::Font *AssetManager::getFont(std::string fontName)
{
    return &this->fonts[fontName];
}
