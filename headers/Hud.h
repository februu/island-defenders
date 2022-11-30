#pragma once

#include <map>
#include <string>
#include <SFML/Graphics.hpp>

#include "Game.h"

class Game;

class Hud
{
private:
    Game *game;
    enum gameStates
    {
        MAINMENU,
        NEWGAME,
        GAME,
        SCORE,
        CREDITS
    };
    struct hudElement
    {
        sf::FloatRect bounds;
        int id;
    };
    std::vector<hudElement> hudElements;
    void addHudElement(sf::FloatRect bounds, int id);
    int size, smallSize, offset;
    int difficultyChoice = 0; // 0 - easy, 1 - normal, 2 - hard
    const std::map<int, std::string> difficulties = {{0, "Easy"},
                                                     {1, "Normal"},
                                                     {2, "Hard"}};
    int elementChoice = 0; // 0 - water, 1 - fire, 2 - ground, 3 - air
    const std::map<int, std::string> elements = {{0, "Water"},
                                                 {1, "Fire"},
                                                 {2, "Ground"},
                                                 {3, "Air"}};
    const std::map<int, std::string> elementsDescriptions = {{0, "| Towers can freeze monsters and deal 1.25x to fire enemies. |"},
                                                             {1, "| When killed, enemies explode. Towers and do 1.25x to air enemies. |"},
                                                             {2, "| Towers can poison monsters and deal 1.25x to water enemies. |"},
                                                             {3, "| Towers can push back monsters and do 1.25x to ground enemies. |"}};

public:
    Hud(Game *game);
    virtual ~Hud();

    void updateHudElements(int newGameState);
    void executeMenuAction();
    int checkMouse(sf::Vector2f);
    void drawMainMenu();
    void drawNewGameScreen();
    void drawGameHud(int wave);
    void drawScore();
    void drawCredits();
    void drawPauseOverlay();
};
