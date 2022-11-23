#pragma once

#include <SFML\Graphics.hpp>
#include "AssetManager.h"
#include "World.h"

class Game
{
private:
    // Const Variables
    sf::RenderWindow *window;
    sf::Clock clock;
    sf::Event sfEvent;
    int mapSize = 16;
    float tileSize, tileScale = 3.f;
    int screenWidth, screenHeight;
    int mapXOffset, mapYOffset;
    bool fullscreenMode;
    bool debugMode;
    AssetManager am;
    World world;

    // Gameplay Variables
    int selectedTileX = 0, selectedTileY = 0;
    bool isPaused = false;

    void createWindow();

public:
    Game(int width, int height, int mapSize, float tileScale, bool fullScreenMode);
    virtual ~Game();

    // Main Functions

    void handleEvents();
    void update();
    void draw();
    void run();

    // Functions
    void mouseToSelectedTile();
    void drawSprite(int x, int y, sf::Sprite sprite, float scaleX = 1.f, float scaleY = 1.f);
    void drawText(int x, int y, std::string text, int size, sf::Color color = sf::Color::White, bool centered = false);
};