#pragma once

enum constants
{
    MAPSIZE = 24
};

// Game states
enum gameStates
{
    MAINMENU,
    NEWGAME,
    GAME,
    SCORE,
    CREDITS
};

// Tile values
enum tiles
{
    REGULARTILE,
    TREETILE,
    GRASSTILE,
    ROCKTILE,
    PLANTTILE,
    MINETILE,
    WATERTILE,

};

// Prices
enum prices
{
    TURRETPRICE = 250,
    WALLPRICE = 100,
    MINEPRICE = 400,
};