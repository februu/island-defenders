#pragma once

#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "World.h"
#include "Hud.h"
#include "Constants.h"
#include "Particle.h"
#include "Projectile.h"

class Hud;

class Game
{
private:
    // Const Variables & Enums
    sf::Clock clock;
    sf::Event sfEvent;
    bool fullscreenMode;
    AssetManager am;
    Hud *hud;
    std::vector<Particle> particles;
    std::vector<Projectile> projectiles;

    // Gameplay Variables
    double deltaTime;

    bool debugMode;
    double screenTransition[4] = {0}; // {isActive [bool], value - coordinate [int], transition state (1-out, 2-waiting, 3-in) [in], end GameState [int]}

    // Game Functions
    void createWindow();

    // Update Functions
    void mouseTohoveredTile();
    void callNewWave();
    sf::Vector2i randomizeSpawnTile();

    // Draw Functions
    void drawMap();
    void drawEntities();
    void drawParticles();
    void drawProjectiles();
    void drawDebugInfo();
    void drawScreenTransition();

public:
    Game(int width, int height, float tileScale, bool fullScreenMode);
    virtual ~Game();

    // Public Variables
    sf::RenderWindow *window;
    float tileSize, tileScale = 3.f;
    long double timePassed = 0;
    double spawnTimer = 0;
    int screenWidth, screenHeight;
    int mapXOffset, mapYOffset;
    int gameState = MAINMENU;
    int selectedItem = 0;
    bool isPaused = false;
    int hoveredTileX = -1, hoveredTileY = -1;
    int selectedTileX = -1, selectedTileY = -1;
    bool buildMode = false;

    int wave = 0;

    World *world;

    // Main Functions
    void handleEvents();
    void update();
    void draw();
    void run();

    // Public Draw Functions
    sf::Sprite drawSprite(float x, float y, std::string spriteName, float scaleX = 1.f, float scaleY = 1.f, bool draw = true, bool reversed = false);
    sf::Text drawText(int x, int y, std::string text, int size, sf::Color color = sf::Color::White, bool centered = false, std::string font = "pixelmix", bool draw = true);
    void addParticle(Particle particle);
    void addProjectile(Projectile projectile);

    // Public Gamestate functions
    void changeGameState(int newGameState);

    bool checkIfValidTileSelected();
    bool checkIfValidTileHovered();
};