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
    GRASSTILE,
    ROCKTILE,
    PLANTTILE,
    MINETILE,
    WATERTILE,
    TREETILE,
};

// Prices
enum prices
{
    TURRETPRICE = 200,
    FIRETURRETPRICE = 350,
    ICETURRETPRICE = 100,
    MAGICTURRETPRICE = 200,
    MINEPRICE = 200,
};