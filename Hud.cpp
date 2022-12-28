#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "headers/Hud.h"
#include "headers/Game.h"
#include "headers/Constants.h"
#include <iostream>

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
            game->changeGameState(GAME); // TODO: Load existing game.
            break;
        case 3:
            system("start https://github.com/februu");
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
            game->world->createNewWorld();
            // TODO: Pass difficulty and element choice to World.
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
            // TODO: Add Game saving here
            game->changeGameState(MAINMENU);
            break;
        case -1:
            game->isPaused = false;
            updateHudElements(GAME);
            break;
        case 1:
            game->world->placeNewBuilding(game->selectedTileX, game->selectedTileY, "turret", "turret_fire_left", 5, 7, 10);
            game->selectedTileX = -1;
            game->selectedTileY = -1;
            updateHudElements(GAME);
            break;
        case 2:
            game->world->placeNewBuilding(game->selectedTileX, game->selectedTileY, "turret", "turret_fire_right", 5, 7, 10);
            game->selectedTileX = -1;
            game->selectedTileY = -1;
            updateHudElements(GAME);
            break;
        case 3:
            game->world->placeNewBuilding(game->selectedTileX, game->selectedTileY, "turret", "turret_fire_up", 5, 7, 10);
            game->selectedTileX = -1;
            game->selectedTileY = -1;
            updateHudElements(GAME);
            break;
        case 4:
            game->world->placeNewBuilding(game->selectedTileX, game->selectedTileY, "turret", "turret_fire_down", 5, 7, 10);
            game->selectedTileX = -1;
            game->selectedTileY = -1;
            updateHudElements(GAME);
            break;
        case 5:
            game->world->placeNewBuilding(game->selectedTileX, game->selectedTileY, "turret", "turret_fire_right", 5, 7, 10);
            game->selectedTileX = -1;
            game->selectedTileY = -1;
            updateHudElements(GAME);
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
        addHudElement(game->drawText(game->mapXOffset, game->screenHeight / 2 + 1 * (smallSize + offset), "> " + elements.at(elementChoice) + " <", smallSize, sf::Color::White, true, "pixelmix", false).getGlobalBounds(), 2);
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
        // TODO: Add pause button.
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
    game->drawText(game->mapXOffset, game->screenHeight / 2, "Element:", smallSize, sf::Color(169, 169, 169), true);
    game->drawText(game->mapXOffset, game->screenHeight / 2 + 1 * (smallSize + offset), elements.at(elementChoice), smallSize, sf::Color::White, true);
    game->drawText(game->mapXOffset, game->screenHeight / 2 + 2 * (smallSize + offset), elementsDescriptions.at(elementChoice), smallSize / 2, sf::Color(169, 169, 169), true);
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
    // game->drawText(game->mapXOffset, 48, "Wave " + std::to_string(wave), smallSize, sf::Color::White, true);

    // Draws Info Panel
    if (game->checkIfValidTileHovered())
    {
        // Selects Item to display.
        if (game->world->entities[game->hoveredTileX * MAPSIZE + game->hoveredTileY])
        {
            std::string type = game->world->entities[game->hoveredTileX * MAPSIZE + game->hoveredTileY]->getType();
            if (type == "base" || type == "turret" || type == "mine")
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
                    game->drawSprite(ix + 8 * game->tileScale, iy + 8 * game->tileScale, ent->getSpriteName(), 1.5 * game->tileScale, 1.5 * game->tileScale);
                    game->drawText(ix + (8 + 40) * game->tileScale, iy + (6 * 1.5 * game->tileScale) * 0.75, "TURRET", 6 * game->tileScale);
                    game->drawText(ix + (8 + 40) * game->tileScale, iy + (6 * 1.5 * game->tileScale) * 1.75, "Quick and reliable way", 6 * game->tileScale);
                    game->drawText(ix + (8 + 40) * game->tileScale, iy + (6 * 1.5 * game->tileScale) * 2.75, "deal with monsters.", 6 * game->tileScale);
                    game->drawText(ix + (8 + 40) * game->tileScale, iy + (6 * 1.5 * game->tileScale) * 3.75, "Health: " + std::to_string(ent->getHealth()), 6 * game->tileScale);
                }
                else
                {
                    // TODO: Add Mines.
                }
            }
        }
    }

    // Draws Build Menu.
    if (game->checkIfValidTileSelected() && game->buildMode && game->world->tilemap[game->selectedTileX][game->selectedTileY] != WATERTILE && game->world->entities[game->selectedTileX * MAPSIZE + game->selectedTileY] == nullptr)
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
            game->drawSprite(x + (4 + (game->selectedItem - 1) * 24 + (game->selectedItem - 1) * 6) * game->tileScale, y + 4 * game->tileScale, "build_hud_selected", game->tileScale, game->tileScale);

        // Draws buildings relatively to build hud.
        game->drawSprite(x + (5 + 0 * 30) * game->tileScale, y + 6 * game->tileScale, "turret_fire_right", game->tileScale, game->tileScale);
        game->drawSprite(x + (5 + 1 * 30) * game->tileScale, y + 6 * game->tileScale, "turret_ice_right", game->tileScale, game->tileScale);
        game->drawSprite(x + (5 + 2 * 30) * game->tileScale, y + 6 * game->tileScale, "turret_poison_right", game->tileScale, game->tileScale);
        game->drawSprite(x + (5 + 3 * 30) * game->tileScale, y + 6 * game->tileScale, "turret_poison_right", game->tileScale, game->tileScale);
        game->drawSprite(x + (5 + 4 * 30) * game->tileScale, y + 6 * game->tileScale, "turret_poison_right", game->tileScale, game->tileScale);
        // TODO: Add turets and mines.
    }
}

void Hud::drawScore()
{
    std::string items[] = {"Stats", "Wave: ", "Monsters killed: ", "Difficulty multiplier: ", "Total score: "};
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
    std::string items[] = {"Island Defenders", "sp. z.o.o.", "", "nie no jk", "made by febru"};
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
