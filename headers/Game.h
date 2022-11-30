#pragma once

#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "World.h"
#include "Hud.h"

class Hud;

class Game
{
private:
    // Const Variables & Enums
    enum gameStates
    {
        MAINMENU,
        NEWGAME,
        GAME,
        SCORE,
        CREDITS
    };

    sf::Clock clock;
    sf::Event sfEvent;
    int mapSize = 16;
    bool fullscreenMode;
    AssetManager am;
    World world;
    Hud *hud;

    // Gameplay Variables
    double deltaTime;
    int selectedTileX = 0, selectedTileY = 0;
    bool debugMode;
    double screenTransition[4] = {0}; // {isActive [bool], value - coordinate [int], transition state (1-out, 2-waiting, 3-in) [in], end GameState [int]}

    // Game Functions
    void createWindow();

    // Update Functions
    void mouseToSelectedTile();

    // Draw Functions
    void drawMap();
    void drawEntities();
    void drawDebugInfo();
    void drawScreenTransition();

public:
    Game(int width, int height, int mapSize, float tileScale, bool fullScreenMode);
    virtual ~Game();

    // Public Variables
    sf::RenderWindow *window;
    float tileSize, tileScale = 3.f;
    int screenWidth, screenHeight;
    int mapXOffset, mapYOffset;
    int gameState = MAINMENU;
    int selectedItem = 0;
    bool isPaused = false;

    // Main Functions
    void handleEvents();
    void update();
    void draw();
    void run();

    // Public Draw Functions
    sf::Sprite drawSprite(int x, int y, std::string spriteName, float scaleX = 1.f, float scaleY = 1.f, bool draw = true);
    sf::Text drawText(int x, int y, std::string text, int size, sf::Color color = sf::Color::White, bool centered = false, std::string font = "pixelmix", bool draw = true);

    // Public Gamestate functions
    void changeGameState(int newGameState);
};