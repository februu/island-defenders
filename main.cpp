#include "headers\settings.h"
#include "headers\Game.h"

int main()
{

    int settings[4];
    loadSettings(settings);

    // width, height, mapSize, tileScale, fullScreenMode, debugMode
    Game game(settings[0], settings[1], 16, 3.f, settings[2], settings[3]);
    game.run();

    return 0;
}