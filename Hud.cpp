#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "headers/Hud.h"
#include "headers/Game.h"
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
            game->changeGameState(GAME);
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
            game->isPaused = !game->isPaused;
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
        // TODO: Add more elements
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
    game->drawText(game->mapXOffset, 48, "Wave " + std::to_string(wave), smallSize, sf::Color::White, true);
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