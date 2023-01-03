#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "headers/Game.h"
#include "headers/AssetManager.h"
#include "headers/World.h"
#include "headers/Hud.h"
#include "headers/Constants.h"
#include "headers/Entity.h"
#include "headers/Projectile.h"

Game::Game(int width, int height, float tileScale, bool fullScreenMode)
{
    // Variables
    this->screenWidth = width;
    this->screenHeight = height;
    this->tileScale = tileScale;
    this->fullscreenMode = fullScreenMode;
    this->debugMode = false;
    this->tileSize = 16 * tileScale;
    this->mapXOffset = width / 2;
    this->mapYOffset = (height - this->tileSize * MAPSIZE) / 2;
    this->createWindow();
    this->world = new World(this);
    this->world->createNewWorld();
    this->hud = new Hud(this);
    hud->updateHudElements(0);
}

Game::~Game()
{
    delete window;
    delete hud;
    delete world;
}

void Game::handleEvents()
{
    while (window->pollEvent(sfEvent))
    {

        if (sfEvent.type == sf::Event::MouseButtonPressed)
            if (sfEvent.mouseButton.button == sf::Mouse::Left)
                if (selectedItem != 0)
                {
                    hud->executeMenuAction();
                }
                else if (gameState == GAME)
                {
                    selectedTileX = hoveredTileX;
                    selectedTileY = hoveredTileY;
                    hud->updateHudElements(GAME);
                    buildMode = false;
                }
        if (sfEvent.mouseButton.button == sf::Mouse::Right)
            if (gameState == GAME)
            {
                selectedTileX = hoveredTileX;
                selectedTileY = hoveredTileY;
                hud->updateHudElements(GAME);
                buildMode = true;
            }

        if (sfEvent.type == sf::Event::KeyPressed)
            if (sfEvent.key.code == sf::Keyboard::LShift)
                debugMode = true;
        if (sfEvent.type == sf::Event::KeyReleased)
        {
            if (sfEvent.key.code == sf::Keyboard::LShift)
                debugMode = false;
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
    timePassed += deltaTime;
    spawnTimer += deltaTime;

    // Updates the game.
    if (!isPaused && gameState == GAME)
    {
        // Calculates hovered tile.
        mouseTohoveredTile();

        // Performs actions of enemies and turrets.
        for (int y = 0; y < MAPSIZE; y++)     // height
            for (int x = 0; x < MAPSIZE; x++) // width
            {
                if (world->getEntity(y, x))
                {
                    world->getEntity(y, x)->performAction(deltaTime);
                }
            }

        // Updates particles.
        for (auto particle = begin(particles); particle != end(particles); ++particle)
            if (particle->update(deltaTime))
                particles.erase(particle--);

        // Updates turret particles.
        for (auto turretParticle = begin(turretParticles); turretParticle != end(turretParticles); ++turretParticle)
            turretParticle->particle.update(deltaTime);

        // Calculates positions of projectiles.
        for (auto projectile = begin(projectiles); projectile != end(projectiles); ++projectile)
            if (projectile->update(deltaTime))
                projectiles.erase(projectile--);

        // Spawns new wave of enemies if current one is over.
        callNewWave();

        // Checks if hp is below 5.
        if (hearts <= 0)
            changeGameState(SCORE);
    }

    // Sets selected item.
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
        drawProjectiles();
        drawParticles();
        hud->drawGameHud(20);
        if (timePassed < 5)
            drawText(screenWidth / 2, tileScale * 16, "Starting in " + std::to_string(int(5 - timePassed)) + "...", tileScale * 8 * 2, sf::Color::White, true);
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
    window->setFramerateLimit(240);
    sf::Image icon = sf::Image();
    icon.loadFromFile("assets/icon.png");
    window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void Game::changeGameState(int newGameState)
{
    screenTransition[0] = 1;
    if (newGameState == GAME)
        timePassed = 0;
    screenTransition[3] = newGameState;
}

void Game::clearParticlesAndProjectiles()
{
    projectiles.clear();
    particles.clear();
    turretParticles.clear();
}
// ================= Update Functions =================

// Calculates hovered tile.
void Game::mouseTohoveredTile()
{
    int x1 = sf::Mouse::getPosition(*window).x - mapXOffset;
    int y1 = (sf::Mouse::getPosition(*window).y - mapYOffset) * -2;
    double xr = cos(M_PI / 4) * x1 - sin(M_PI / 4) * y1;
    double yr = sin(M_PI / 4) * x1 + cos(M_PI / 4) * y1;
    double diag = tileSize * sqrt(2);
    hoveredTileX = floor(xr / diag);
    hoveredTileY = floor(yr * -1 / diag);
}

bool Game::checkIfValidTileSelected()
{
    return selectedTileX >= 0 && selectedTileX < MAPSIZE && selectedTileY >= 0 && selectedTileY < MAPSIZE;
}

bool Game::checkIfValidTileHovered()
{
    return hoveredTileX >= 0 && hoveredTileX < MAPSIZE && hoveredTileY >= 0 && hoveredTileY < MAPSIZE;
}

void Game::callNewWave()
{

    if (timePassed > 5)
    {

        if (!isStarted)
        {
            for (int i = 0; i < MAPSIZE * MAPSIZE; i++)
                if (world->entities[i])
                    if (world->entities[i]->checkIfPathEmpty())
                        world->entities[i]->findPath();
            isStarted = true;
        }

        // Runs every 1 second.
        if (spawnTimer > 2 - int(monstersKilled / 20) / 10)
        {
            spawnTimer = 0;

            // Spawns new wave of enemies.

            std::string enemyTypes[] = {"slime", "wasp"};
            std::string enemyColors[] = {"red", "green", "blue"};
            std::string enemyType = enemyTypes[rand() % 2];
            sf::Vector2i coords = randomizeSpawnTile();
            Enemy *r_wasp = new Enemy();
            if (enemyType == "wasp")
                r_wasp->createEntity(coords.x, coords.y, enemyType, "wasp_" + enemyColors[rand() % 3], 5, 7, 5 + world->difficulty + int(monstersKilled / 10), world);
            else
                r_wasp->createEntity(coords.x, coords.y, enemyType, "slime_" + enemyColors[rand() % 3], 9, 3, 7 + world->difficulty + int(monstersKilled / 10), world);
            world->entities[coords.x * MAPSIZE + coords.y] = r_wasp;
            r_wasp->setTimeToNextMove(double(rand() % 50 / 100));
            r_wasp->findPath();
            // sf::Thread thread(std::bind(&Entity::findPath, &(*r_wasp)));
            // thread.launch();
        }
    }
}

sf::Vector2i Game::randomizeSpawnTile()
{
    sf::Vector2i coords;
    int side, tile;
    do
    {
        side = (rand() % 2) * (MAPSIZE - 1);
        tile = rand() % MAPSIZE;
        if (rand() % 2)
            coords = sf::Vector2i(side, tile);
        else
            coords = sf::Vector2i(tile, side);
    } while (world->getEntity(coords.x, coords.y) || world->tilemap[coords.x][coords.y] == WATERTILE);
    return coords;
}

// ================= Draw Functions =================

// Draws sprite.
sf::Sprite Game::drawSprite(float x, float y, std::string spriteName, float scaleX, float scaleY, bool draw, bool reversed)
{
    sf::Sprite sprite = *am.getSprite(spriteName);
    sprite.setScale(sf::Vector2f(scaleX, scaleY));
    sprite.setPosition(sf::Vector2f(x, y));
    if (reversed)
    {
        sf::IntRect bounds = sprite.getTextureRect();
        bounds.left = bounds.width;
        bounds.width = -bounds.width;
        sprite.setTextureRect(bounds);
    }

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

// Adds particle to vector.
void Game::addParticle(Particle particle)
{
    particles.push_back(particle);
}

// Adds turret particle to vector.
void Game::addTurretParticle(Particle particle, int x, int y)
{
    turretParticles.push_back({sf::Vector2i(x, y), particle});
}

void Game::destroyTurretParticle(int x, int y)
{
    for (auto turretParticle = begin(turretParticles); turretParticle != end(turretParticles); ++turretParticle)
        if (turretParticle->coords.x == x && turretParticle->coords.y == y)
            turretParticles.erase(turretParticle--);
}

// Adds projectile to vector.
void Game::addProjectile(Projectile projectile)
{
    projectiles.push_back(projectile);
}

// Draws the map.
void Game::drawMap()
{
    for (int y = 0; y < MAPSIZE; y++)     // height
        for (int x = 0; x < MAPSIZE; x++) // width
        {
            int mapX = tileSize * x - tileSize * y - tileSize + mapXOffset;
            int mapY = (tileSize * y + tileSize * x) / 2 + mapYOffset;

            switch (world->tilemap[x][y])
            {
            case REGULARTILE:
                drawSprite(mapX, mapY, "tile", tileScale, tileScale);
                break;
            case TREETILE:
                drawSprite(mapX, mapY, "tile", tileScale, tileScale);
                break;
            case GRASSTILE:
                drawSprite(mapX, mapY, "tile_grass", tileScale, tileScale);
                break;
            case ROCKTILE:
                drawSprite(mapX, mapY, "tile_rocks", tileScale, tileScale);
                break;
            case PLANTTILE:
                drawSprite(mapX, mapY, "tile_plants", tileScale, tileScale);
                break;
            case MINETILE:
                drawSprite(mapX, mapY, "tile_mine", tileScale, tileScale);
                break;
            case WATERTILE:
                drawSprite(mapX, mapY, "water", tileScale, tileScale);

                // Draws waterfalls.
                if (y == MAPSIZE - 1) // 36 is offset of water texture!
                {
                    for (int i = 0; i < (screenHeight - (mapY + 19 * tileScale)) / 36; i++)
                        drawSprite(mapX, mapY + 19 * tileScale + i * 36, "water_left", tileScale, tileScale);
                }
                if (x == MAPSIZE - 1)
                {
                    for (int i = 0; i < (screenHeight - (mapY + 19 * tileScale)) / 36; i++)
                        drawSprite(mapX + 16 * tileScale, mapY + 19 * tileScale + i * 36, "water_right", tileScale, tileScale);
                }
                break;
            }

            // Draws hovered tile.
            if (x == hoveredTileX && y == hoveredTileY && !isPaused && world->tilemap[x][y] != WATERTILE && selectedItem == 0)
                drawSprite(mapX, mapY, "tile_hovered", tileScale, tileScale);
        }

    int mapX = tileSize * selectedTileX - tileSize * selectedTileY - tileSize + mapXOffset;
    int mapY = (tileSize * selectedTileY + tileSize * selectedTileX) / 2 + mapYOffset;

    if (checkIfValidTileSelected() && !isPaused) // FIXME: && world->tilemap[selectedTileX][selectedTileY] != WATERTILE
        drawSprite(mapX - tileScale, mapY - tileScale, "tile_selected", tileScale, tileScale);
}

// Draws Entities aka Game Objects.
void Game::drawEntities()
{

    for (int y = 0; y < MAPSIZE; y++)     // height
        for (int x = 0; x < MAPSIZE; x++) // width
        {
            int mapX = tileSize * x - tileSize * y - tileSize + mapXOffset;
            int mapY = (tileSize * y + tileSize * x) / 2 + mapYOffset; //  + sin(timePassed * 2) * tileScale;
            if (world->getEntity(x, y))
            {
                Entity *ent = world->getEntity(x, y);
                std::string spriteName = ent->getSpriteName();
                int spriteOffsetX = ent->getXOffset() * tileScale;
                int spriteOffsetY = ent->getYOffset() * tileScale;
                float spriteMoveOffsetX = ent->getMoveX() * tileScale;
                float spriteMoveOffsetY = ent->getMoveY() * tileScale;
                if (ent->getDirection() > 1)
                    drawSprite(mapX + spriteOffsetX + spriteMoveOffsetX, mapY - spriteOffsetY + spriteMoveOffsetY, spriteName, tileScale, tileScale);
                else
                    drawSprite(mapX + spriteOffsetX + spriteMoveOffsetX, mapY - spriteOffsetY + spriteMoveOffsetY, spriteName, tileScale, tileScale, true, true);
            }
        }
}

// Draws particles.
void Game::drawParticles()
{
    for (auto turretParticle = begin(turretParticles); turretParticle != end(turretParticles); ++turretParticle)
    {
        float x = turretParticle->particle.getPosition().x;
        float y = turretParticle->particle.getPosition().y;
        drawSprite(x, y, turretParticle->particle.getSpriteName(), turretParticle->particle.getScale() * tileScale, turretParticle->particle.getScale() * tileScale);
    }

    for (auto particle = begin(particles); particle != end(particles); ++particle)
    {
        float x = particle->getPosition().x;
        float y = particle->getPosition().y;
        drawSprite(x, y, particle->getSpriteName(), particle->getScale() * tileScale, particle->getScale() * tileScale);
    }
}

// Draws projectiles.
void Game::drawProjectiles()
{
    for (auto projectile = begin(projectiles); projectile != end(projectiles); ++projectile)
    {
        float x = projectile->getPosition().x - 2.5 * tileScale;
        float y = projectile->getPosition().y - 2.5 * tileScale;
        drawSprite(x, y, projectile->getSpriteName(), tileScale, tileScale);
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
    drawText(10, 40, "hoveredTile=" + std::to_string(hoveredTileX) + "," + std::to_string(hoveredTileY), 15);
    // game state
    drawText(10, 55, "gameState=" + std::to_string(gameState), 15);
    // screen transition value
    drawText(10, 70, "selectedItem=" + std::to_string(selectedItem), 15);
    // screen transition value
    drawText(10, 85, "screenTransitionValue=" + std::to_string(screenTransition[1]), 15);
    // delta time
    drawText(10, 100, "crystals=" + std::to_string(crystals), 15);
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