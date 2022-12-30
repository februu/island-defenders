#include "headers/Settings.h"
#include "headers/Game.h"

int main()
{
    int settings[3];
    loadSettings(settings);

    // width, height, mapSize, tileScale, fullScreenMode
    Game game(settings[0], settings[1], 2.f, settings[2]);
    game.run();
    return 0;
}