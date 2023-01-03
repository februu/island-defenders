#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <cmath>

#include "headers/Hud.h"
#include "headers/Game.h"
#include "headers/Constants.h"
#include "headers/Particle.h"
#include "headers/Settings.h"

Hud::Hud(Game *game)
{
    this->game = game;
    this->size = game->tileSize * game->tileScale * 2;
    this->smallSize = game->tileSize * game->tileScale / 2;
    this->offset = smallSize / 2;
}

Hud::~Hud()
{
}

// ================= Update Functions =================

void Hud::addHudElement(sf::FloatRect bounds, int id)
{
    hudElements.push_back({bounds, id});
};

int Hud::checkMouse(sf::Vector2f mousePos)
{
    for (auto i = hudElements.begin(); i != hudElements.end(); ++i)
    {
        if ((*i).bounds.contains(mousePos))
            return (*i).id;
    }
    return 0;
}

void Hud::executeMenuAction()
{
    switch (game->gameState)
    {
    case MAINMENU:
    {
        switch (game->selectedItem)
        {
        case 1:
            game->changeGameState(NEWGAME);
            break;
        case 2:
            if (loadSaveFile(game->world, &game->timePassed, &game->hearts, &game->crystals, &game->monstersKilled))
            {
                game->isStarted = false;
                game->changeGameState(GAME);
            }

            break;
        case 3:
            system("start https://febru.me/island-defenders/");
            break;
        case 4:
            game->changeGameState(CREDITS);
            break;
        case 5:
            exit(0);
        }
        break;
    }

    case NEWGAME:
    {
        switch (game->selectedItem)
        {
        case 1:
            difficultyChoice++;
            difficultyChoice = difficultyChoice % difficulties.size();
            break;
        case 2:
            elementChoice++;
            elementChoice = elementChoice % elements.size();
            break;
        case 3:
            game->changeGameState(GAME);
            game->isStarted = false;
            game->world->createNewWorld();
            game->hearts = 5;
            game->crystals = 1000;
            game->monstersKilled = 0;
            game->crystalsEarned = 0;
            game->world->difficulty = difficultyChoice;
            game->clearParticlesAndProjectiles();
            break;
        }
        break;
    }

    case SCORE:
    case CREDITS:
    {
        game->changeGameState(MAINMENU);
        break;
    }

    case GAME:
    {
        switch (game->selectedItem)
        {
        case -2:
            createSaveFile(game->world->tilemap, game->world->entities, game->timePassed, game->hearts, game->crystals, game->monstersKilled, game->world->difficulty);
            game->changeGameState(MAINMENU);
            break;
        case -1:
            game->isPaused = false;
            updateHudElements(GAME);
            break;
        case 1:
            if (game->crystals >= TURRETPRICE)
                if (game->world->placeNewBuilding(game->selectedTileX, game->selectedTileY, "turret", "turret", 7, 8, TURRETHEALTH))
                {
                    game->crystals -= TURRETPRICE;
                    int mapX = game->tileSize * game->selectedTileX - game->tileSize * game->selectedTileY - game->tileSize + game->mapXOffset + 13 * game->tileScale;
                    int mapY = (game->tileSize * game->selectedTileY + game->tileSize * game->selectedTileX) / 2 + game->mapYOffset - game->tileScale - 9 * game->tileScale;
                    Particle particle(mapX, mapY, "particles/poison", 5, 1);
                    game->addTurretParticle(particle, game->selectedTileX, game->selectedTileY);
                    game->selectedTileX = -1;
                    game->selectedTileY = -1;
                    updateHudElements(GAME);
                }

            break;
        case 2:
            if (game->crystals >= ICETURRETPRICE)
                if (game->world->placeNewBuilding(game->selectedTileX, game->selectedTileY, "turret", "turret_ice", 7, 8, ICETURRETHEALTH))
                {
                    game->crystals -= ICETURRETPRICE;
                    int mapX = game->tileSize * game->selectedTileX - game->tileSize * game->selectedTileY - game->tileSize + game->mapXOffset + 13 * game->tileScale;
                    int mapY = (game->tileSize * game->selectedTileY + game->tileSize * game->selectedTileX) / 2 + game->mapYOffset - game->tileScale - 9 * game->tileScale;
                    Particle particle(mapX, mapY, "particles/ice", 5, 1);
                    game->addTurretParticle(particle, game->selectedTileX, game->selectedTileY);
                    game->selectedTileX = -1;
                    game->selectedTileY = -1;
                    updateHudElements(GAME);
                }

            break;
        case 3:
            if (game->crystals >= FIRETURRETPRICE)
                if (game->world->placeNewBuilding(game->selectedTileX, game->selectedTileY, "turret", "turret_fire", 7, 8, FIRETURRETHEALTH))
                {
                    game->crystals -= FIRETURRETPRICE;
                    int mapX = game->tileSize * game->selectedTileX - game->tileSize * game->selectedTileY - game->tileSize + game->mapXOffset + 13 * game->tileScale;
                    int mapY = (game->tileSize * game->selectedTileY + game->tileSize * game->selectedTileX) / 2 + game->mapYOffset - game->tileScale - 9 * game->tileScale;
                    Particle particle(mapX, mapY, "particles/fire", 5, 1);
                    game->addTurretParticle(particle, game->selectedTileX, game->selectedTileY);
                    game->selectedTileX = -1;
                    game->selectedTileY = -1;
                    updateHudElements(GAME);
                }

            break;
        case 4:
            if (game->crystals >= MAGICTURRETPRICE)
                if (game->world->placeNewBuilding(game->selectedTileX, game->selectedTileY, "turret", "turret_magic", 7, 8, MAGICTURRETHEALTH))
                {
                    game->crystals -= MAGICTURRETPRICE;
                    int mapX = game->tileSize * game->selectedTileX - game->tileSize * game->selectedTileY - game->tileSize + game->mapXOffset + 13 * game->tileScale;
                    int mapY = (game->tileSize * game->selectedTileY + game->tileSize * game->selectedTileX) / 2 + game->mapYOffset - game->tileScale - 9 * game->tileScale;
                    Particle particle(mapX, mapY, "particles/magic", 5, 1);
                    game->addTurretParticle(particle, game->selectedTileX, game->selectedTileY);
                    game->selectedTileX = -1;
                    game->selectedTileY = -1;
                    updateHudElements(GAME);
                }

            break;
        case 5:
            if (game->crystals >= MINEPRICE)
            {
                if (game->world->tilemap[game->selectedTileX][game->selectedTileY] == MINETILE)
                {
                    game->world->placeNewBuilding(game->selectedTileX, game->selectedTileY, "mine", "mine", 3, 2, MINEHEALTH);
                    game->crystals -= MINEPRICE;
                }

                game->selectedTileX = -1;
                game->selectedTileY = -1;
                updateHudElements(GAME);
            }

            break;
        }
        break;
    }
    }
}

void Hud::updateHudElements(int newGameState)
{
    hudElements.clear();
    switch (newGameState)
    {
    case MAINMENU:
    {
        std::string items[] = {"New Game", "Load Game", "How to play", "Credits", "Exit"};
        for (int i = -2; i < 3; i++)
            addHudElement(game->drawText(game->mapXOffset, game->screenHeight / 2 + i * (smallSize + offset), items[i + 2], smallSize, sf::Color::White, true, "pixelmix", false).getGlobalBounds(), (i + 2) + 1);
        break;
    }
    case NEWGAME:
    {
        addHudElement(game->drawText(game->mapXOffset, game->screenHeight / 2 + -1 * (smallSize + offset), "> " + difficulties.at(difficultyChoice) + " <", smallSize, sf::Color::White, true, "pixelmix", false).getGlobalBounds(), 1);
        // addHudElement(game->drawText(game->mapXOffset, game->screenHeight / 2 + 1 * (smallSize + offset), "> " + elements.at(elementChoice) + " <", smallSize, sf::Color::White, true, "pixelmix", false).getGlobalBounds(), 2);
        addHudElement(game->drawText(game->mapXOffset, game->screenHeight / 2 + 4 * (smallSize + offset), "> OK <", smallSize, sf::Color(105, 105, 105), true, "pixelmix", false).getGlobalBounds(), 3);
        break;
    }
    case SCORE:
    {
        addHudElement(game->drawText(game->mapXOffset, game->screenHeight / 2 + 4 * (smallSize + offset), "> OK <", smallSize, sf::Color(105, 105, 105), true, "pixelmix", false).getGlobalBounds(), 1);
        break;
    }
    case CREDITS:
    {
        addHudElement(game->drawText(game->mapXOffset, game->screenHeight / 2 + 4 * (smallSize + offset), "> Back <", smallSize, sf::Color::White, true, "pixelmix", false).getGlobalBounds(), 1);
        break;
    }
    case GAME:
    {
        if (game->isPaused)
        {
            addHudElement(game->drawText(game->mapXOffset, game->screenHeight / 2 + 1 * (smallSize + offset), "Return to game", smallSize, sf::Color::White, true, "pixelmix", false).getGlobalBounds(), -1);
            addHudElement(game->drawText(game->mapXOffset, game->screenHeight / 2 + 2 * (smallSize + offset), "Save & Exit", smallSize, sf::Color::White, true, "pixelmix", false).getGlobalBounds(), -2);
        }
        else
        {
            if (game->checkIfValidTileSelected() && game->buildMode && game->world->tilemap[game->selectedTileX][game->selectedTileY] != WATERTILE && game->world->entities[game->selectedTileX * MAPSIZE + game->selectedTileY] == nullptr)
            {
                int x = game->tileSize * game->selectedTileX - game->tileSize * game->selectedTileY - game->tileSize + game->mapXOffset;
                int y = (game->tileSize * game->selectedTileY + game->tileSize * game->selectedTileX) / 2 + game->mapYOffset;
                x += 16 * game->tileScale - 184 / 2 * game->tileScale;
                if (game->selectedTileX > MAPSIZE - 5 && game->selectedTileY > MAPSIZE - 5)
                    y -= (40 + 2) * game->tileScale;
                else
                    y += (16 + 4) * game->tileScale;
                x += 4 * game->tileScale;
                y += 4 * game->tileScale;

                // Build Hud Options
                addHudElement(sf::FloatRect(x + (0 * 24 + 1 * 6) * game->tileScale, y, 24 * game->tileScale, 24 * game->tileScale), 1);
                addHudElement(sf::FloatRect(x + (1 * 24 + 2 * 6) * game->tileScale, y, 24 * game->tileScale, 24 * game->tileScale), 2);
                addHudElement(sf::FloatRect(x + (2 * 24 + 3 * 6) * game->tileScale, y, 24 * game->tileScale, 24 * game->tileScale), 3);
                addHudElement(sf::FloatRect(x + (3 * 24 + 4 * 6) * game->tileScale, y, 24 * game->tileScale, 24 * game->tileScale), 4);
                addHudElement(sf::FloatRect(x + (4 * 24 + 5 * 6) * game->tileScale, y, 24 * game->tileScale, 24 * game->tileScale), 5);

                // Build Hud
                addHudElement(sf::FloatRect(x, y, 152 * game->tileScale, 40 * game->tileScale), -3);
            }
        }
        break;
    }
    }
}

// ================= Draw Functions =================

void Hud::drawMainMenu()
{
    game->drawSprite(0, 0, "overlay", game->screenWidth, game->screenHeight);
    std::string items[] = {"New Game", "Load Game", "How to play", "Credits", "Exit"};
    game->drawText(game->mapXOffset, game->screenHeight / 2 - (size / 2) - 2 * (smallSize + offset), "ISLAND DEFENDERS", size, sf::Color::White, true, "arcade");
    for (int i = -2; i < 3; i++)
    {
        game->drawText(game->mapXOffset, game->screenHeight / 2 + i * (smallSize + offset), items[i + 2], smallSize, sf::Color::White, true);
        if (game->selectedItem == (i + 2) + 1)
            game->drawText(game->mapXOffset, game->screenHeight / 2 + i * (smallSize + offset), "> " + items[i + 2] + " <", smallSize, sf::Color(105, 105, 105), true);
    }

    // addHudElement(   .getGlobalBounds(), "loadgame");
}

void Hud::drawNewGameScreen()
{
    game->drawSprite(0, 0, "overlay", game->screenWidth, game->screenHeight);
    game->drawText(game->mapXOffset, game->screenHeight / 2 - (size / 2) - 2 * (smallSize + offset), "NEW GAME", size, sf::Color::White, true, "arcade");
    game->drawText(game->mapXOffset, game->screenHeight / 2 + -2 * (smallSize + offset), "Difficulty:", smallSize, sf::Color(169, 169, 169), true);
    game->drawText(game->mapXOffset, game->screenHeight / 2 + -1 * (smallSize + offset), difficulties.at(difficultyChoice), smallSize, sf::Color::White, true);
    // game->drawText(game->mapXOffset, game->screenHeight / 2, "Element:", smallSize, sf::Color(169, 169, 169), true);
    // game->drawText(game->mapXOffset, game->screenHeight / 2 + 1 * (smallSize + offset), elements.at(elementChoice), smallSize, sf::Color::White, true);
    // game->drawText(game->mapXOffset, game->screenHeight / 2 + 2 * (smallSize + offset), elementsDescriptions.at(elementChoice), smallSize / 2, sf::Color(169, 169, 169), true);
    game->drawText(game->mapXOffset, game->screenHeight / 2 + 4 * (smallSize + offset), "OK", smallSize, sf::Color::White, true);
    switch (game->selectedItem)
    {
    case 1:
        game->drawText(game->mapXOffset, game->screenHeight / 2 + -1 * (smallSize + offset), "> " + difficulties.at(difficultyChoice) + " <", smallSize, sf::Color(105, 105, 105), true);
        break;
    case 2:
        game->drawText(game->mapXOffset, game->screenHeight / 2 + 1 * (smallSize + offset), "> " + elements.at(elementChoice) + " <", smallSize, sf::Color(105, 105, 105), true);
        break;
    case 3:
        game->drawText(game->mapXOffset, game->screenHeight / 2 + 4 * (smallSize + offset), "> OK <", smallSize, sf::Color(105, 105, 105), true);
        break;
    }
}

void Hud::drawGameHud(int wave)
{
    // Draws crystals.
    game->drawSprite(game->tileScale * 8, game->tileScale * 8, "gem", 2 * game->tileScale, 2 * game->tileScale);
    game->drawText(game->tileScale * 8 + 12 * 2 * game->tileScale, game->tileScale * 9, std::to_string(game->crystals), game->tileScale * 8 * 2);

    // Draws hearts.
    for (int i = 0; i < game->hearts; i++)
        game->drawSprite(game->screenWidth - (game->tileScale * 8) - (2 * game->tileScale * 11) - (i * (game->tileScale * (4 + 2 * 11))), game->tileScale * 8, "heart", 2 * game->tileScale, 2 * game->tileScale);

    // Draws Info Panel
    if (game->checkIfValidTileHovered())
    {
        // Selects Item to display.
        if (game->world->entities[game->hoveredTileX * MAPSIZE + game->hoveredTileY])
        {
            std::string type = game->world->entities[game->hoveredTileX * MAPSIZE + game->hoveredTileY]->getType();
            if (type == "base" || type == "turret" || type == "mine" || type == "tree")
            {
                // Draws Back Panel.
                int ix = game->tileScale * 8;
                int iy = game->screenHeight - (game->tileScale * (8 + 1.5 * 32));
                game->drawSprite(ix, iy, "info_hud", 1.5 * game->tileScale, 1.5 * game->tileScale);

                // Draws Entity Info.
                Entity *ent = game->world->getEntity(game->hoveredTileX, game->hoveredTileY);
                if (type == "base")
                {
                    game->drawSprite(ix + 4 * game->tileScale, iy + 6 * game->tileScale, "main_base", 0.75 * game->tileScale, 0.75 * game->tileScale);
                    game->drawText(ix + (8 + 40) * game->tileScale, iy + (6 * 1.5 * game->tileScale) * 0.75, "DARK ALTAR", 6 * game->tileScale);
                    game->drawText(ix + (8 + 40) * game->tileScale, iy + (6 * 1.5 * game->tileScale) * 1.75, "Filled with unharnessed dark", 6 * game->tileScale);
                    game->drawText(ix + (8 + 40) * game->tileScale, iy + (6 * 1.5 * game->tileScale) * 2.75, "power coming from creatures", 6 * game->tileScale);
                    game->drawText(ix + (8 + 40) * game->tileScale, iy + (6 * 1.5 * game->tileScale) * 3.75, "out of this world.", 6 * game->tileScale);
                }
                else if (type == "turret")
                {
                    int descSwitch = 9;
                    std::string desc[] = {"ICE TURRET", "Reliable way to deal with monsters.", "Effect: Slows enemies.", "FIRE TURRET", "Reliable way to deal with monsters.", "Effect: Does double the damage.", "MAGIC TURRET", "Reliable way to deal with monsters.", "Effect: Stuns enemies.", "BASIC TURRET", "Reliable way to deal with monsters.", ""};
                    std::string turretType = game->world->getEntity(game->hoveredTileX, game->hoveredTileY)->getSpriteName();
                    int turretHealth = TURRETHEALTH;

                    if (turretType == "turret_ice")
                    {
                        descSwitch = 0;
                        turretHealth = ICETURRETHEALTH;
                    }
                    else if (turretType == "turret_fire")
                    {
                        descSwitch = 3;
                        turretHealth = FIRETURRETHEALTH;
                    }
                    else if (turretType == "turret_magic")
                    {
                        descSwitch = 6;
                        turretHealth = MAGICTURRETHEALTH;
                    }

                    game->drawSprite(ix + 10 * game->tileScale, iy + 8 * game->tileScale, ent->getSpriteName(), 1.5 * game->tileScale, 1.5 * game->tileScale);
                    game->drawText(ix + (8 + 40) * game->tileScale, iy + (6 * 1.5 * game->tileScale) * 0.75, desc[descSwitch], 6 * game->tileScale);
                    game->drawText(ix + (8 + 40) * game->tileScale, iy + (6 * 1.5 * game->tileScale) * 1.75, desc[descSwitch + 1], 6 * game->tileScale);
                    game->drawText(ix + (8 + 40) * game->tileScale, iy + (6 * 1.5 * game->tileScale) * 2.75, desc[descSwitch + 2], 6 * game->tileScale);
                    game->drawText(ix + (8 + 40) * game->tileScale, iy + (6 * 1.5 * game->tileScale) * 3.75, "HP: " + std::to_string(ent->getHealth()) + "/" + std::to_string(turretHealth), 6 * game->tileScale);
                }
                else if (type == "mine")
                {
                    game->drawSprite(ix + 6 * game->tileScale, iy + 10 * game->tileScale, "mine", 1.5 * game->tileScale, 1.5 * game->tileScale);
                    game->drawText(ix + (8 + 40) * game->tileScale, iy + (6 * 1.5 * game->tileScale) * 0.75, "MINE", 6 * game->tileScale);
                    game->drawText(ix + (8 + 40) * game->tileScale, iy + (6 * 1.5 * game->tileScale) * 1.75, "Mines 20 crystals every 5 seconds.", 6 * game->tileScale);
                    game->drawText(ix + (8 + 40) * game->tileScale, iy + (6 * 1.5 * game->tileScale) * 2.75, "Next delivery in " + std::to_string(5 - int(ent->getTimeToNextMove())) + " seconds...", 6 * game->tileScale);
                    game->drawText(ix + (8 + 40) * game->tileScale, iy + (6 * 1.5 * game->tileScale) * 3.75, "HP: " + std::to_string(ent->getHealth()) + "/" + std::to_string(MINEHEALTH), 6 * game->tileScale);
                }
                else if (type == "tree")
                {
                    game->drawSprite(ix + 14 * game->tileScale, iy + 4 * game->tileScale, "tree", 1.5 * game->tileScale, 1.5 * game->tileScale);
                    game->drawText(ix + (8 + 40) * game->tileScale, iy + (6 * 1.5 * game->tileScale) * 0.75, "TREE", 6 * game->tileScale);
                    game->drawText(ix + (8 + 40) * game->tileScale, iy + (6 * 1.5 * game->tileScale) * 1.75, "Weird looking tree.", 6 * game->tileScale);
                }
            }
        }
    }

    // Draws Build Menu.
    if (game->checkIfValidTileSelected() && game->buildMode && game->world->tilemap[game->selectedTileX][game->selectedTileY] != WATERTILE && !game->world->entities[game->selectedTileX * MAPSIZE + game->selectedTileY])
    {
        int x = game->tileSize * game->selectedTileX - game->tileSize * game->selectedTileY - game->tileSize + game->mapXOffset;
        int y = (game->tileSize * game->selectedTileY + game->tileSize * game->selectedTileX) / 2 + game->mapYOffset;
        x += 16 * game->tileScale - 184 / 2 * game->tileScale;
        if (game->selectedTileX > MAPSIZE - 5 && game->selectedTileY > MAPSIZE - 5)
            y -= (40 + 2) * game->tileScale;
        else
            y += (16 + 4) * game->tileScale;
        game->drawSprite(x, y, "build_hud", game->tileScale, game->tileScale);

        // Highlights hovered option.
        if (game->selectedItem > 0 && game->selectedItem < 6)
        {
            game->drawSprite(x + (4 + (game->selectedItem - 1) * 24 + (game->selectedItem - 1) * 6) * game->tileScale, y + 4 * game->tileScale, "build_hud_selected", game->tileScale, game->tileScale);

            // Shows price.
            int price = 0;
            switch (game->selectedItem)
            {
            case 1:
                price = TURRETPRICE;
                break;
            case 2:
                price = ICETURRETPRICE;
                break;
            case 3:
                price = FIRETURRETPRICE;
                break;
            case 4:
                price = MAGICTURRETPRICE;
                break;
            case 5:
                price = MINEPRICE;
                break;
            }
            if (game->crystals >= price)
                game->drawText(x + 24 * game->tileScale, y + 31 * game->tileScale, std::to_string(price), 5 * game->tileScale);
            else
                game->drawText(x + 24 * game->tileScale, y + 31 * game->tileScale, std::to_string(price), 5 * game->tileScale, sf::Color::Red);
        }

        if (game->world->tilemap[game->selectedTileX][game->selectedTileY] != MINETILE)
            game->drawSprite(x + (4 + 4 * 24 + 4 * 6) * game->tileScale, y + 4 * game->tileScale, "build_hud_disabled", game->tileScale, game->tileScale);

        // Draws buildings relatively to build hud.
        game->drawSprite(x + (7 + 0 * 30) * game->tileScale, y + 5 * game->tileScale, "turret", game->tileScale, game->tileScale);
        game->drawSprite(x + (7 + 1 * 30) * game->tileScale, y + 5 * game->tileScale, "turret_ice", game->tileScale, game->tileScale);
        game->drawSprite(x + (7 + 2 * 30) * game->tileScale, y + 5 * game->tileScale, "turret_fire", game->tileScale, game->tileScale);
        game->drawSprite(x + (7 + 3 * 30) * game->tileScale, y + 5 * game->tileScale, "turret_magic", game->tileScale, game->tileScale);
        game->drawSprite(x + ((5 + 4 * 30) - 1) * game->tileScale, y + 7 * game->tileScale, "mine", game->tileScale, game->tileScale);
    }
}

void Hud::drawScore()
{

    float difficultyMultiplier = 0.75 + game->world->difficulty * 0.25;
    std::string difficultyMultiplierString;
    switch (game->world->difficulty)
    {
    case 0:
        difficultyMultiplierString = "0.75x";
        break;
    case 1:
        difficultyMultiplierString = "1x";
        break;
    case 2:
        difficultyMultiplierString = "1.25x";
        break;
    }

    std::string items[] = {"Stats", "Crystals earned: " + std::to_string(game->crystalsEarned), "Monsters killed: " + std::to_string(game->monstersKilled), "Difficulty multiplier: " + difficultyMultiplierString, "Total score: " + std::to_string(int(10 * difficultyMultiplier * (0.66 * game->monstersKilled + 0.33 * game->crystalsEarned)))};
    game->drawSprite(0, 0, "overlay", game->screenWidth, game->screenHeight);
    game->drawText(game->mapXOffset, game->screenHeight / 2 - (size / 2) - 2 * (smallSize + offset), "GAME OVER", size, sf::Color::White, true, "arcade");
    for (int i = -2; i < 3; i++)
        game->drawText(game->mapXOffset, game->screenHeight / 2 + i * (smallSize + offset), items[i + 2], smallSize, sf::Color(169, 169, 169), true);
    game->drawText(game->mapXOffset, game->screenHeight / 2 + 4 * (smallSize + offset), "OK", smallSize, sf::Color::White, true);
    if (game->selectedItem == 1)
        game->drawText(game->mapXOffset, game->screenHeight / 2 + 4 * (smallSize + offset), "> OK <", smallSize, sf::Color(105, 105, 105), true);
}

void Hud::drawCredits()
{
    std::string items[] = {
        "coding:",
        "@febru",
        "graphics:",
        "@febru & @smok18krolczarownic aka mlodymichal",
        "febru.me/island-defenders/",
    };
    game->drawSprite(0, 0, "overlay", game->screenWidth, game->screenHeight);
    game->drawText(game->mapXOffset, game->screenHeight / 2 - (size / 2) - 2 * (smallSize + offset), "CREDITS", size, sf::Color::White, true, "arcade");
    for (int i = -2; i < 3; i++)
    {
        game->drawText(game->mapXOffset, game->screenHeight / 2 + i * (smallSize + offset), items[i + 2], smallSize, sf::Color(169, 169, 169), true);
    }
    game->drawText(game->mapXOffset, game->screenHeight / 2 + 4 * (smallSize + offset), "OK", smallSize, sf::Color::White, true);
    if (game->selectedItem == 1)
        game->drawText(game->mapXOffset, game->screenHeight / 2 + 4 * (smallSize + offset), "> OK <", smallSize, sf::Color(105, 105, 105), true);
}

void Hud::drawPauseOverlay()
{

    game->drawSprite(0, 0, "overlay", game->screenWidth, game->screenHeight);
    game->drawSprite(0, 0, "overlay", game->screenWidth, game->screenHeight);
    game->drawText(game->mapXOffset, game->screenHeight / 2 - (size / 2), "Paused", size, sf::Color::White, true, "arcade");
    game->drawText(game->mapXOffset, game->screenHeight / 2 + 1 * (smallSize + offset), "Return to game", smallSize, sf::Color::White, true);
    game->drawText(game->mapXOffset, game->screenHeight / 2 + 2 * (smallSize + offset), "Save & Exit", smallSize, sf::Color::White, true);
    if (game->selectedItem == -1)
        game->drawText(game->mapXOffset, game->screenHeight / 2 + 1 * (smallSize + offset), "> Return to game <", smallSize, sf::Color(105, 105, 105), true);
    if (game->selectedItem == -2)
        game->drawText(game->mapXOffset, game->screenHeight / 2 + 2 * (smallSize + offset), "> Save & Exit <", smallSize, sf::Color(105, 105, 105), true);
}

//
