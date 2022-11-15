#pragma once

#include <SFML\Graphics.hpp>
#include "AssetManager.h"

class Game
{
private:
    // Const Variables
    sf::RenderWindow *window;
    sf::Event sfEvent;
    int mapSize = 16;
    float tileSize, tileScale = 3.f;
    int screenWidth, screenHeight;
    int mapXOffset, mapYOffset;
    bool fullscreenMode;
    bool debugMode;
    AssetManager am;

    // Gameplay Variables
    int selectedTileX = 0, selectedTileY = 0;
    bool isPaused = false;

    void createWindow();

public:
    Game(int width, int height, int mapSize, float tileScale, bool fullScreenMode, bool debugMode);
    virtual ~Game();

    // Main Functions

    void handleEvents();
    void update();
    void draw();
    void run();

    // Functions
    void mouseToSelectedTile();
};