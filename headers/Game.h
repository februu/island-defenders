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
    AssetManager am;
    World world;

    // Gameplay Variables
    int gameState = 0;
    double deltaTime;
    int selectedTileX = 0, selectedTileY = 0, selectedItem = -1;
    bool debugMode;
    bool isPaused = false;
    double screenTransition[4] = {0}; // {isActive [bool], value - coordinate [int], transition state (1-out, 2-waiting, 3-in) [in], end GameState [int]}

    // Game Functions
    void createWindow();
    void changeGameState(int newGameState);

    // Update Functions
    void mouseToSelectedTile();

    // Draw Functions
    void drawMainMenu();
    void drawMap();
    void drawEntities();
    void drawDebugInfo();
    void drawPauseScreen();
    void drawScreenTransition();
    void drawSprite(int x, int y, std::string spriteName, float scaleX = 1.f, float scaleY = 1.f);
    void drawText(int x, int y, std::string text, int size, sf::Color color = sf::Color::White, bool centered = false, std::string font = "pixelmix");

public:
    Game(int width, int height, int mapSize, float tileScale, bool fullScreenMode);
    virtual ~Game();

    // Main Functions
    void handleEvents();
    void update();
    void draw();
    void run();
};