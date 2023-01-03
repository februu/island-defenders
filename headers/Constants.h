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

// Prices of turrets
enum prices
{
    TURRETPRICE = 200,
    FIRETURRETPRICE = 350,
    ICETURRETPRICE = 50,
    MAGICTURRETPRICE = 150,
    MINEPRICE = 200,
};

// HP of turrets
enum healthpoints
{
    TURRETHEALTH = 4,
    FIRETURRETHEALTH = 2,
    ICETURRETHEALTH = 3,
    MAGICTURRETHEALTH = 3,
    MINEHEALTH = 3,
};