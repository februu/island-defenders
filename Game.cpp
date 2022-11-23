#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "headers/Game.h"
#include "headers/AssetManager.h"
#include "headers/World.h"

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
    delete this->window;
}

void Game::createWindow()
{
    if (fullscreenMode)
        this->window = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), "Island Defenders", sf::Style::Fullscreen);
    else
        this->window = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), "Island Defenders", sf::Style::Close);
    this->window->setMouseCursorVisible(false);
}

void Game::handleEvents()
{
    while (this->window->pollEvent(this->sfEvent))
    {
        if (this->isPaused)
        {
            if (this->sfEvent.type == sf::Event::KeyPressed)
                if (this->sfEvent.key.code == sf::Keyboard::Space)
                    this->window->close();
                else if (this->sfEvent.key.code == sf::Keyboard::Escape)
                    this->isPaused = false;
        }
        else
        {
            if (this->sfEvent.type == sf::Event::KeyPressed)
                if (this->sfEvent.key.code == sf::Keyboard::Escape)
                    this->isPaused = true;
                else if (this->sfEvent.key.code == sf::Keyboard::LShift)
                    this->debugMode = true;
                else if (this->sfEvent.key.code == sf::Keyboard::Space)
                    this->world.createNewWorld();
            if (this->sfEvent.type == sf::Event::KeyReleased)
                if (this->sfEvent.key.code == sf::Keyboard::LShift)
                    this->debugMode = false;
        }
        if (this->sfEvent.type == sf::Event::Closed)
            this->window->close();
    }
}

void Game::update()
{
    if (!this->isPaused)
    {
        mouseToSelectedTile();
    }
}

void Game::draw()
{
    this->window->clear();

    // Draws the map.
    for (int y = 0; y < mapSize; y++)     // height
        for (int x = 0; x < mapSize; x++) // width
        {
            int mapX = tileSize * x - tileSize * y - tileSize + mapXOffset;
            int mapY = (tileSize * y + tileSize * x) / 2 + mapYOffset;

            switch (this->world.tilemap[x][y])
            {
            case 1:
                this->drawSprite(mapX, mapY, *this->am.getSprite("tile_grass"), this->tileScale, this->tileScale);
                break;
            case 2:
                this->drawSprite(mapX, mapY, *this->am.getSprite("tile_rock"), this->tileScale, this->tileScale);
                break;
            case 3:
                this->drawSprite(mapX, mapY, *this->am.getSprite("tile_water"), this->tileScale, this->tileScale);

                // Draws waterfalls.
                if (y == this->mapSize - 1) // 36 is offset of water texture!
                {
                    for (int i = 0; i < (this->screenHeight - (mapY + 19 * this->tileScale)) / 36; i++)
                        this->drawSprite(mapX, mapY + 19 * this->tileScale + i * 36, *this->am.getSprite("water_left"), this->tileScale, this->tileScale);
                }
                if (x == this->mapSize - 1)
                {
                    for (int i = 0; i < (this->screenHeight - (mapY + 19 * this->tileScale)) / 36; i++)
                        this->drawSprite(mapX + 16 * this->tileScale, mapY + 19 * this->tileScale + i * 36, *this->am.getSprite("water_right"), this->tileScale, this->tileScale);
                }

                break;
            default:
                this->drawSprite(mapX, mapY, *this->am.getSprite("tile"), this->tileScale, this->tileScale);
            }

            // Draws hovered tile.
            if (x == this->selectedTileX && y == selectedTileY)
                this->drawSprite(mapX, mapY, *this->am.getSprite("selected_tile"), this->tileScale, this->tileScale);
        }

    // Draw Entities aka Game Objects.
    for (int y = 0; y < mapSize; y++)     // height
        for (int x = 0; x < mapSize; x++) // width
        {
            int mapX = tileSize * x - tileSize * y - tileSize + mapXOffset;
            int mapY = (tileSize * y + tileSize * x) / 2 + mapYOffset;
            if (this->world.getEntity(y, x).getType() != "")
            {
                std::string spriteName = this->world.getEntity(y, x).getSpriteName();
                int spriteOffsetX = this->world.getEntity(y, x).getXOffset();
                int spriteOffsetY = this->world.getEntity(y, x).getYOffset();

                drawSprite(mapX - spriteOffsetX * this->tileScale, mapY - spriteOffsetY * this->tileScale, *this->am.getSprite(spriteName), this->tileScale, this->tileScale);
            }
        }

    // Debug Mode
    if (this->debugMode)
    {
        // fps count
        float currentTime = this->clock.restart().asSeconds();
        int fps = 1 / currentTime;
        this->drawText(10, 10, "fps=" + std::to_string(fps), 15);
        // mouse position
        this->drawText(10, 25, "mousePos=" + std::to_string(sf::Mouse::getPosition(*this->window).x) + "," + std::to_string(sf::Mouse::getPosition(*this->window).y), 15);
        // hovered tile coords
        this->drawText(10, 40, "hoveredTile=" + std::to_string(this->selectedTileX) + "," + std::to_string(this->selectedTileY), 15);
    }

    // Pause Screen
    if (this->isPaused)
    {
        this->drawSprite(0, 0, *this->am.getSprite("pause"), this->screenWidth, this->screenHeight);
        this->drawText(this->mapXOffset, this->screenHeight / 2, "PAUSED", tileSize, sf::Color::White, true);
    }

    // Draws the cursor.
    this->drawSprite(sf::Mouse::getPosition(*this->window).x, sf::Mouse::getPosition(*this->window).y, *this->am.getSprite("cursor"));

    this->window->display();
}

void Game::run()
{
    while (this->window->isOpen())
    {
        this->handleEvents();
        this->update();
        this->draw();
    }
}

void Game::mouseToSelectedTile()
{
    int x1 = sf::Mouse::getPosition(*this->window).x - mapXOffset;
    int y1 = (sf::Mouse::getPosition(*this->window).y - mapYOffset) * -2;
    double xr = cos(M_PI / 4) * x1 - sin(M_PI / 4) * y1;
    double yr = sin(M_PI / 4) * x1 + cos(M_PI / 4) * y1;
    double diag = tileSize * sqrt(2);
    this->selectedTileX = floor(xr / diag);
    this->selectedTileY = floor(yr * -1 / diag);
}

void Game::drawSprite(int x, int y, sf::Sprite sprite, float scaleX, float scaleY)
{
    sprite.setScale(sf::Vector2f(scaleX, scaleY));
    sprite.setPosition(sf::Vector2f(x, y));
    this->window->draw(sprite);
}

void Game::drawText(int x, int y, std::string content, int size, sf::Color color, bool centered)
{
    sf::Text text(content, *this->am.getFont());
    text.setCharacterSize(size);
    text.setPosition(sf::Vector2f(x, y));
    text.setFillColor(color);
    if (centered)
    {
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top + textRect.height / 2.0f);
    }
    this->window->draw(text);
}
