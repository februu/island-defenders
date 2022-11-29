#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "headers/Game.h"
#include "headers/AssetManager.h"
#include "headers/World.h"
#include "headers/Hud.h"

using std::cout;

Game::Game(int width, int height, int mapSize, float tileScale, bool fullScreenMode)
{
    // Variables
    this->screenWidth = width;
    this->screenHeight = height;
    this->mapSize = mapSize;
    this->tileScale = tileScale;
    this->fullscreenMode = fullScreenMode;
    this->debugMode = false;
    this->tileSize = 16 * tileScale;
    this->mapXOffset = width / 2;
    this->mapYOffset = (height - this->tileSize * mapSize) / 2;
    this->createWindow();
    this->world.createNewWorld();
}

Game::~Game()
{
    delete window;
}

void Game::handleEvents()
{
    while (window->pollEvent(sfEvent))
    {
        if (isPaused)
        {
            if (sfEvent.type == sf::Event::KeyPressed)
                if (sfEvent.key.code == sf::Keyboard::Space)
                    window->close();
                else if (sfEvent.key.code == sf::Keyboard::Escape)
                    isPaused = false;
        }
        else
        {
            if (sfEvent.type == sf::Event::KeyPressed)
                if (sfEvent.key.code == sf::Keyboard::Escape)
                    if (gameState == 2)
                        isPaused = true;
            if (sfEvent.key.code == sf::Keyboard::LShift)
                debugMode = true;
            if (sfEvent.key.code == sf::Keyboard::Space)
                world.createNewWorld();
            if (sfEvent.type == sf::Event::KeyReleased)
                if (sfEvent.key.code == sf::Keyboard::LShift)
                    debugMode = false;
            if (sfEvent.key.code == sf::Keyboard::RShift)
                changeGameState(2);
        }
        if (sfEvent.type == sf::Event::Closed)
            window->close();
    }
}

void Game::update()
{
    // Updates delta time.
    deltaTime = clock.restart().asSeconds();

    // TODO: Check if transition is not running - if (screenTransition[0])
    if (!isPaused)
    {
        mouseToSelectedTile();
    }
}

void Game::draw()
{
    window->clear();

    // Draws background.
    drawSprite(0, 0, "bg", 3, 3);
    drawSprite(0, 0, "overlay", screenWidth, screenHeight);

    switch (gameState)
    {

    // Main Menu
    case 0:
        drawMainMenu();
        break;

    // New Game Screen
    case 1:
        drawText(screenWidth / 2, screenHeight / 2, "New Game Creation Screen", tileSize, sf::Color::White, true);
        break;

    // Game
    case 2:
        drawMap();
        drawEntities();
        // TODO: Draw HUD
        break;

    // Game Over / Score screen
    case 3:
        drawText(screenWidth / 2, screenHeight / 2, "Score Screen", tileSize, sf::Color::White, true);
        break;

    // Credits screen
    case 4:
        drawText(screenWidth / 2, screenHeight / 2, "Credits", tileSize, sf::Color::White, true);
        break;

    default:
        drawText(screenWidth / 2, screenHeight / 2, "GameState error has occurred.", tileSize, sf::Color::White, true);
    }

    // Pause Screen
    if (isPaused)
        drawPauseScreen();

    // Draws Screen Transition.
    if (screenTransition[0])
        drawScreenTransition();

    // Debug Mode
    if (debugMode)
        drawDebugInfo();

    // Draws the cursor.
    drawSprite(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y, "cursor");

    window->display();
}

void Game::run()
{
    while (window->isOpen())
    {
        handleEvents();
        update();
        draw();
    }
}

// ================= Game Functions =================

void Game::createWindow()
{
    if (fullscreenMode)
        window = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), "Island Defenders", sf::Style::Fullscreen);
    else
        window = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), "Island Defenders", sf::Style::Close);
    window->setMouseCursorVisible(false);
}

void Game::changeGameState(int newGameState)
{
    screenTransition[0] = 1;
    screenTransition[3] = newGameState;
}

// ================= Update Functions =================

// Calculates hovered tile.
void Game::mouseToSelectedTile()
{
    int x1 = sf::Mouse::getPosition(*window).x - mapXOffset;
    int y1 = (sf::Mouse::getPosition(*window).y - mapYOffset) * -2;
    double xr = cos(M_PI / 4) * x1 - sin(M_PI / 4) * y1;
    double yr = sin(M_PI / 4) * x1 + cos(M_PI / 4) * y1;
    double diag = tileSize * sqrt(2);
    selectedTileX = floor(xr / diag);
    selectedTileY = floor(yr * -1 / diag);
}

// ================= Draw Functions =================

// Draws sprite.
void Game::drawSprite(int x, int y, std::string spriteName, float scaleX, float scaleY)
{
    sf::Sprite sprite = *am.getSprite(spriteName);
    sprite.setScale(sf::Vector2f(scaleX, scaleY));
    sprite.setPosition(sf::Vector2f(x, y));
    window->draw(sprite);
}

// Draws text.
void Game::drawText(int x, int y, std::string content, int size, sf::Color color, bool centered, std::string font)
{
    sf::Text text(content, *am.getFont(font));
    text.setCharacterSize(size);
    text.setPosition(sf::Vector2f(x, y));
    text.setFillColor(color);
    if (centered)
    {
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top);
    }
    window->draw(text);
}

// Draws main menu.
void Game::drawMainMenu()
{
    int size = tileSize * tileScale * 2;
    int smallSize = tileSize * tileScale / 2;
    int offset = smallSize / 2;
    drawSprite(0, 0, "overlay", screenWidth, screenHeight);
    drawText(mapXOffset, screenHeight / 2 - (size / 2), "ISLAND DEFENDERS", size, sf::Color::White, true, "arcade");
    drawText(mapXOffset, screenHeight / 2, "> Load Game", smallSize, sf::Color::White, true); // TODO: Gray out this option
    drawText(mapXOffset, screenHeight / 2 + smallSize + offset, "> New Game", smallSize, sf::Color::White, true);
    drawText(mapXOffset, screenHeight / 2 + 2 * (smallSize + offset), "> Options", smallSize, sf::Color::White, true);
    drawText(mapXOffset, screenHeight / 2 + 3 * (smallSize + offset), "> Credits", smallSize, sf::Color::White, true);
    drawText(mapXOffset, screenHeight / 2 + 4 * (smallSize + offset), "> Exit", smallSize, sf::Color::White, true);
}

// Draws the map.
void Game::drawMap()
{
    for (int y = 0; y < mapSize; y++)     // height
        for (int x = 0; x < mapSize; x++) // width
        {
            int mapX = tileSize * x - tileSize * y - tileSize + mapXOffset;
            int mapY = (tileSize * y + tileSize * x) / 2 + mapYOffset;

            switch (world.tilemap[x][y])
            {
            case 1:
                drawSprite(mapX, mapY, "tile_grass", tileScale, tileScale);
                break;
            case 2:
                drawSprite(mapX, mapY, "tile_rock", tileScale, tileScale);
                break;
            case 3:
                drawSprite(mapX, mapY, "tile_water", tileScale, tileScale);

                // Draws waterfalls.
                if (y == mapSize - 1) // 36 is offset of water texture!
                {
                    for (int i = 0; i < (screenHeight - (mapY + 19 * tileScale)) / 36; i++)
                        drawSprite(mapX, mapY + 19 * tileScale + i * 36, "water_left", tileScale, tileScale);
                }
                if (x == mapSize - 1)
                {
                    for (int i = 0; i < (screenHeight - (mapY + 19 * tileScale)) / 36; i++)
                        drawSprite(mapX + 16 * tileScale, mapY + 19 * tileScale + i * 36, "water_right", tileScale, tileScale);
                }

                break;
            default:
                drawSprite(mapX, mapY, "tile", tileScale, tileScale);
            }

            // Draws hovered tile.
            if (x == selectedTileX && y == selectedTileY && !isPaused && world.tilemap[x][y] != 3)
                drawSprite(mapX, mapY, "selected_tile", tileScale, tileScale);
        }
}

// Draws Entities aka Game Objects.
void Game::drawEntities()
{

    for (int y = 0; y < mapSize; y++)     // height
        for (int x = 0; x < mapSize; x++) // width
        {
            int mapX = tileSize * x - tileSize * y - tileSize + mapXOffset;
            int mapY = (tileSize * y + tileSize * x) / 2 + mapYOffset;
            if (world.getEntity(y, x).getType() != "")
            {
                std::string spriteName = world.getEntity(y, x).getSpriteName();
                int spriteOffsetX = world.getEntity(y, x).getXOffset();
                int spriteOffsetY = world.getEntity(y, x).getYOffset();

                drawSprite(mapX - spriteOffsetX * tileScale, mapY - spriteOffsetY * tileScale, spriteName, tileScale, tileScale);
            }
        }
}

// Draws debug info in top left corner of the screen.
void Game::drawDebugInfo()
{
    // fps count
    int fps = 1 / deltaTime;
    drawText(10, 10, "fps=" + std::to_string(fps), 15);
    // mouse position
    drawText(10, 25, "mousePos=" + std::to_string(sf::Mouse::getPosition(*window).x) + "," + std::to_string(sf::Mouse::getPosition(*window).y), 15);
    // hovered tile coords
    drawText(10, 40, "hoveredTile=" + std::to_string(selectedTileX) + "," + std::to_string(selectedTileY), 15);
    // screen transition value
    drawText(10, 55, "screenTransitionValue=" + std::to_string(screenTransition[1]), 15);
    // screen transition value
    drawText(10, 70, "deltaTime=" + std::to_string(deltaTime * 1000), 15);
}

// Draws pause screen
void Game::drawPauseScreen()
{
    int size = tileSize * tileScale;
    drawSprite(0, 0, "overlay", screenWidth, screenHeight);
    drawText(mapXOffset, screenHeight / 2 - 48, "PAUSED", size, sf::Color::White, true, "arcade");
    drawText(mapXOffset, screenHeight / 2, "Click ESC to return to game.", 8 * tileScale, sf::Color::White, true);
    drawText(mapXOffset, screenHeight / 2 + 12 * tileScale, "Click Space to exit.", 8 * tileScale, sf::Color::White, true);
}

// Draws simple scene transition.
void Game::drawScreenTransition()
{
    double value = screenTransition[1];

    // Check Transition State
    if (screenTransition[2] != 1) // Runs if transition is active and is not in waiting state(2).
    {
        sf::RectangleShape rectangle(sf::Vector2f(0, 0));
        rectangle.setSize(sf::Vector2f(value, screenHeight));
        rectangle.setFillColor(sf::Color::Black);
        window->draw(rectangle);

        rectangle.setPosition(sf::Vector2f(screenWidth - value, 0));
        window->draw(rectangle);

        if (screenTransition[2] != 0) // Runs if in transition in running.
        {
            if (value <= 0)
            {
                screenTransition[2] = 0;
                screenTransition[1] = 0;
                screenTransition[0] = 0; // Turns off transition.
                return;
            }
            screenTransition[1] -= 1 * deltaTime * 1000;
        }
        else // Runs if out transition in running.
        {
            if (value >= screenWidth / 2)
            {
                screenTransition[2] = 1;
                screenTransition[1] = 0;
                return;
            }

            screenTransition[1] += 1 * deltaTime * 1000;
        }
    }
    else
    {
        if (value >= 500)
        {
            gameState = screenTransition[3];
            screenTransition[2] = 2;
            screenTransition[1] = screenWidth / 2;
        }
        else
        {
            sf::RectangleShape rectangle(sf::Vector2f(0, 0));
            rectangle.setSize(sf::Vector2f(screenWidth, screenHeight));
            rectangle.setFillColor(sf::Color::Black);
            window->draw(rectangle);
            screenTransition[1]++;
        }
    }
}

//