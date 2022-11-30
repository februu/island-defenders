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
    this->hud = new Hud(this);
    hud->updateHudElements(0);
}

Game::~Game()
{
    delete window;
    delete hud;
}

void Game::handleEvents()
{
    while (window->pollEvent(sfEvent))
    {

        if (sfEvent.type == sf::Event::MouseButtonPressed)
            if (sfEvent.mouseButton.button == sf::Mouse::Left)
                if (selectedItem != 0)
                    hud->executeMenuAction();

        if (sfEvent.type == sf::Event::KeyPressed)
            if (sfEvent.key.code == sf::Keyboard::LShift)
                debugMode = true;
        if (sfEvent.type == sf::Event::KeyReleased)
        {
            if (sfEvent.key.code == sf::Keyboard::LShift)
                debugMode = false;
            if (sfEvent.key.code == sf::Keyboard::Home)
                changeGameState(MAINMENU);
            if (sfEvent.key.code == sf::Keyboard::Escape)
                switch (gameState)
                {
                case NEWGAME:
                case CREDITS:
                    changeGameState(MAINMENU);
                    break;
                case GAME:
                    isPaused = !isPaused;
                    hud->updateHudElements(GAME);
                    break;
                }
        }

        // If closed
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

    selectedItem = hud->checkMouse(sf::Vector2f(sf::Mouse::getPosition(*window)));
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
    case MAINMENU:
        hud->drawMainMenu();
        break;

    // New Game Screen
    case NEWGAME:
        hud->drawNewGameScreen();
        break;

    // Game
    case GAME:
        drawMap();
        drawEntities();
        hud->drawGameHud(selectedTileX);
        break;

    // Game Over / Score screen
    case SCORE:
        hud->drawScore();
        break;

    // Credits screen
    case CREDITS:
        hud->drawCredits();
        break;

    default:
        drawText(screenWidth / 2, screenHeight / 2, "GameState error has occurred.", tileSize, sf::Color::White, true);
    }

    // Pause Screen
    if (isPaused)
        hud->drawPauseOverlay();

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
    window->setFramerateLimit(120);
    // TODO: window->setIcon()
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
sf::Sprite Game::drawSprite(int x, int y, std::string spriteName, float scaleX, float scaleY, bool draw)
{
    sf::Sprite sprite = *am.getSprite(spriteName);
    sprite.setScale(sf::Vector2f(scaleX, scaleY));
    sprite.setPosition(sf::Vector2f(x, y));
    if (draw)
        window->draw(sprite);
    return sprite;
}

// Draws text.
sf::Text Game::drawText(int x, int y, std::string content, int size, sf::Color color, bool centered, std::string font, bool draw)
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
    if (draw)
        window->draw(text);
    return text;
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
    // game state
    drawText(10, 55, "gameState=" + std::to_string(gameState), 15);
    // screen transition value
    drawText(10, 70, "selectedItem=" + std::to_string(selectedItem), 15);
    // screen transition value
    drawText(10, 85, "screenTransitionValue=" + std::to_string(screenTransition[1]), 15);
    // screen transition value
    drawText(10, 100, "deltaTime=" + std::to_string(deltaTime * 1000), 15);
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
            screenTransition[1] -= 2 * deltaTime * 1000;
        }
        else // Runs if out transition in running.
        {
            if (value >= screenWidth / 2)
            {
                screenTransition[2] = 1;
                screenTransition[1] = 0;
                return;
            }

            screenTransition[1] += 2 * deltaTime * 1000;
        }
    }
    else
    {
        sf::RectangleShape rectangle(sf::Vector2f(0, 0));
        rectangle.setSize(sf::Vector2f(screenWidth, screenHeight));
        rectangle.setFillColor(sf::Color::Black);
        window->draw(rectangle);
        if (value >= 250)
        {
            gameState = (int)screenTransition[3];
            isPaused = false;
            hud->updateHudElements(gameState);
            screenTransition[2] = 2;
            screenTransition[1] = screenWidth / 2;
        }
        else
        {
            screenTransition[1] += 2 * deltaTime * 1000;
        }
    }
}

//