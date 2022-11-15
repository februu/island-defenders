#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "headers/Game.h"
#include "headers/AssetManager.h"

using std::cout;

Game::Game(int width, int height, int mapSize, float tileScale, bool fullScreenMode, bool debugMode)
{
    // Variables
    this->screenWidth = width;
    this->screenHeight = height;
    this->mapSize = mapSize;
    this->tileScale = tileScale;
    this->fullscreenMode = fullScreenMode;
    this->debugMode = debugMode;
    this->tileSize = 16 * tileScale;
    this->mapXOffset = width / 2;
    this->mapYOffset = (height - this->tileSize * mapSize) / 2;
    this->createWindow();
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
        if (!this->isPaused)
        {
            if (this->sfEvent.type == sf::Event::KeyPressed)
                if (this->sfEvent.key.code == sf::Keyboard::Escape)
                    this->isPaused = true;
        }
        else
        {
            if (this->sfEvent.type == sf::Event::KeyPressed)
                if (this->sfEvent.key.code == sf::Keyboard::Escape)
                    this->window->close();
                else if (this->sfEvent.key.code == sf::Keyboard::Space)
                    this->isPaused = false;
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

    for (int i = 0; i < mapSize; i++) // height
    {
        for (int j = 0; j < mapSize; j++) // width
        {
            sf::Sprite sprite;
            sprite.setTexture(*this->am.getTexture("tile"));
            sprite.setScale(sf::Vector2f(tileScale, tileScale));
            int x = tileSize * j - tileSize * i - tileSize + mapXOffset;
            int y = (tileSize * i + tileSize * j) / 2 + mapYOffset;
            sprite.setPosition(sf::Vector2f(x, y));
            this->window->draw(sprite);

            if (j == this->selectedTileX && i == selectedTileY)
            {
                sf::Sprite sprite;
                sprite.setTexture(*this->am.getTexture("selected_tile"));
                sprite.setScale(sf::Vector2f(tileScale, tileScale));
                sprite.setPosition(sf::Vector2f(x, y));
                this->window->draw(sprite);
            }
        }
    }

    if (debugMode)
    {
        sf::Text text("hoveredTile=" + std::to_string(this->selectedTileX) + "," + std::to_string(this->selectedTileY), *this->am.getFont());
        text.setCharacterSize(15);
        text.setPosition(sf::Vector2f(10, 10));
        text.setFillColor(sf::Color::White);
        this->window->draw(text);

        sf::Text text2("mousePos=" + std::to_string(sf::Mouse::getPosition(*this->window).x) + "," + std::to_string(sf::Mouse::getPosition(*this->window).y), *this->am.getFont());
        text2.setCharacterSize(15);
        text2.setPosition(sf::Vector2f(10, 25));
        text2.setFillColor(sf::Color::White);
        this->window->draw(text2);
    }

    if (this->isPaused)
    {
        sf::Sprite overlay;
        overlay.setTexture(*this->am.getTexture("pause"));
        overlay.setScale(sf::Vector2f(this->screenWidth, this->screenWidth));
        overlay.setPosition(sf::Vector2f(0, 0));
        this->window->draw(overlay);

        sf::Text text("PAUSED", *this->am.getFont());
        text.setCharacterSize(tileSize);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top + textRect.height / 2.0f);
        text.setPosition(sf::Vector2f(this->mapXOffset, this->screenHeight / 2));
        text.setFillColor(sf::Color::White);
        this->window->draw(text);
    }

    // Draws the cursor.
    sf::Sprite overlay;
    overlay.setTexture(*this->am.getTexture("cursor"));
    overlay.setPosition(sf::Vector2f(sf::Mouse::getPosition(*this->window).x, sf::Mouse::getPosition(*this->window).y));
    this->window->draw(overlay);

    this->window->display();
}

void Game::run()
{
    // sf::Clock clock;
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