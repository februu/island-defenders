#include <iostream>
#include <fstream>
#include <windows.h>

#include "headers/Settings.h"
#include "headers/Constants.h"
#include "headers/Entity.h"
#include "headers/World.h"

using namespace std;

// Creates new game.cfg file.
void createNewSettingsFile()
{
    ofstream file("game.cfg");
    string settings = "#   ______              ___                           __                 ____                 ___                     __                            \n#  /\\__  _\\            /\\_ \\                         /\\ \\               /\\  _`\\             /'___\\                   /\\ \\                           \n#  \\/_/\\ \\/      ____  \\//\\ \\       __       ___     \\_\\ \\              \\ \\ \\/\\ \\     __   /\\ \\__/    __     ___     \\_\\ \\      __    _ __    ____  \n#     \\ \\ \\     /',__\\   \\ \\ \\    /'__`\\   /' _ `\\   /'_` \\              \\ \\ \\ \\ \\  /'__`\\ \\ \\ ,__\\ /'__`\\ /' _ `\\   /'_` \\   /'__`\\ /\\`'__\\ /',__\\ \n#      \\_\\ \\__ /\\__, `\\   \\_\\ \\_ /\\ \\L\\.\\_ /\\ \\/\\ \\ /\\ \\L\\ \\              \\ \\ \\_\\ \\/\\  __/  \\ \\ \\_//\\  __/ /\\ \\/\\ \\ /\\ \\L\\ \\ /\\  __/ \\ \\ \\/ /\\__, `\\\n#      /\\_____\\\\/\\____/   /\\____\\\\ \\__/.\\_\\\\ \\_\\ \\_\\\\ \\___,_\\              \\ \\____/\\ \\____\\  \\ \\_\\ \\ \\____\\\\ \\_\\ \\_\\\\ \\___,_\\\\ \\____\\ \\ \\_\\ \\/\\____/\n#      \\/_____/ \\/___/    \\/____/ \\/__/\\/_/ \\/_/\\/_/ \\/__,_ /               \\/___/  \\/____/   \\/_/  \\/____/ \\/_/\\/_/ \\/__,_ / \\/____/  \\/_/  \\/___/                                                                                                      \n#\n#   Island Defenders v1.0\n#   Made by febru (Kacper Lutomski)\n#\n\n# Display Settings\nwidth=1920\nheight=1080\nfullscreenMode=true\n";
    file << settings;
    file.close();
    MessageBox(NULL, "game.cfg file doesn't exist or is broken. Generating new one. Please restart the game.", "Error", MB_ICONEXCLAMATION | MB_OK);
    exit(0);
}

// Loads settings from game.cfg file.
void loadSettings(int settings[3])
{
    // Variables (count - used to check if settings file contains all information).
    string text;
    int count = 0;

    ifstream file("game.cfg");

    // Check if file exists.
    if (file.fail())
    {
        cout << "ERROR: game.cfg doesn't exist. Creating new one...";
        createNewSettingsFile();
        return;
    }

    // Loops through all lines of the file.
    while (getline(file, text))
    {
        // Ignores blank lines and comments starting with '#' symbol.
        if (text[0] != '#' && text != "")

            // Assigns values to variables (Numbers in text.replace() represent length of the text to replace e.g "height=" - 7, "width=" - 6).
            if (!text.rfind("width", 0))
            {
                text.replace(0, 6, "");
                settings[0] = stoi(text);
                count++;
            }
            else if (!text.rfind("height", 0))
            {
                text.replace(0, 7, "");
                settings[1] = stoi(text);
                count++;
            }
            else if (!text.rfind("fullscreenMode", 0))
            {
                text.replace(0, 15, "");
                settings[2] = !text.rfind("true", 0);
                count++;
            }
    }

    file.close();

    if (count != 3)
    {
        cout << "ERROR: game.cfg is incomplete. Creating new one...";
        remove("game.cfg");
        createNewSettingsFile();
        return;
    }

    // Control line.
    // cout << "width=" << settings[0] << " height=" << settings[1] << " fullscreenMode=" << settings[2] << " debugMode=" << settings[3];

    return;
}

// Creates save.cfg file.
void createSaveFile(int tilemap[MAPSIZE][MAPSIZE], Entity *entities[MAPSIZE * MAPSIZE], long double timePassed, int hearts, int crystals, int monstersKilled, int difficulty)
{
    std::string gameSave, stilemap = "map:", sentities;

    for (int i = 0; i < MAPSIZE; i++)
        for (int j = 0; j < MAPSIZE; j++)
            stilemap = stilemap + std::to_string(tilemap[i][j]);

    for (int i = 0; i < MAPSIZE; i++)
        for (int j = 0; j < MAPSIZE; j++)
            if (entities[i * MAPSIZE + j])
            {
                Entity *ent = entities[i * MAPSIZE + j];
                std::string entity = "e:" + std::to_string(ent->getX()) + " " + std::to_string(ent->getY()) + " " + ent->getType() + " " + ent->getSpriteName() + " " + std::to_string(ent->getHealth()) + " " + std::to_string(ent->getXOffset()) + " " + std::to_string(ent->getYOffset());
                sentities = sentities + entity + "\n";
            }

    gameSave = "#Map\n" + stilemap + "\n\n#Entities\n" + sentities + "\n\n#Game Variables\ntime:" + std::to_string(int(timePassed)) + "\nhearts:" + std::to_string(hearts) + "\ncrystals:" + std::to_string(crystals) + "\nmonsterskilled:" + std::to_string(monstersKilled) + "\ndifficulty:" + std::to_string(difficulty) + "\n";

    ofstream file("save.cfg");
    file << gameSave;
    file.close();
}

// Loads save from file.
bool loadSaveFile(World *world, long double *timePassed, int *hearts, int *crystals, int *monstersKilled)
{
    string text;
    ifstream file("save.cfg");

    // Check if file exists.
    if (file.fail())
    {
        return false;
    }

    // Clears entities array.
    for (int i = 0; i < MAPSIZE * MAPSIZE; i++)
        if (world->entities[i])
        {
            delete world->entities[i];
            world->entities[i] = nullptr;
        }

    // Loops through all lines of the file.
    while (getline(file, text))
    {
        // Ignores blank lines and comments starting with '#' symbol.
        if (text[0] != '#' && text != "")

            if (!text.rfind("e:", 0))
            {
                int propertyIndex = 0;
                std::string entityProperties[7];
                text.replace(0, 2, "");

                for (int i = 0; i < text.length(); i++)
                    if (text[i] != ' ')
                        entityProperties[propertyIndex] = entityProperties[propertyIndex] + text[i];
                    else
                        propertyIndex++;

                int x = stoi(entityProperties[0]), y = stoi(entityProperties[1]);
                if (entityProperties[2] == "wasp" || entityProperties[2] == "slime")
                {
                    Enemy *ent = new Enemy();
                    ent->createEntity(stoi(entityProperties[0]), stoi(entityProperties[1]), entityProperties[2], entityProperties[3], stoi(entityProperties[5]), stoi(entityProperties[6]), stoi(entityProperties[4]), world);
                    world->entities[x * MAPSIZE + y] = ent;
                    ent->setTimeToNextMove(double(rand() % 50 / 100));
                }
                else
                {
                    Building *ent = new Building();
                    ent->createEntity(stoi(entityProperties[0]), stoi(entityProperties[1]), entityProperties[2], entityProperties[3], stoi(entityProperties[5]), stoi(entityProperties[6]), stoi(entityProperties[4]), world);
                    world->entities[x * MAPSIZE + y] = ent;
                    if (entityProperties[2] == "turret")
                    {
                        std::string particleType;
                        if (entityProperties[3] == "turret_ice")
                            particleType = "particles/ice";
                        else if (entityProperties[3] == "turret_fire")
                            particleType = "particles/fire";
                        else if (entityProperties[3] == "turret_magic")
                            particleType = "particles/magic";
                        else
                            particleType = "particles/poison";

                        int mapX = world->game->tileSize * x - world->game->tileSize * y - world->game->tileSize + world->game->mapXOffset + 13 * world->game->tileScale;
                        int mapY = (world->game->tileSize * y + world->game->tileSize * x) / 2 + world->game->mapYOffset - world->game->tileScale - 9 * world->game->tileScale;
                        Particle particle(mapX, mapY, particleType, 5, 1);
                        world->game->addTurretParticle(particle, x, y);
                    }
                }
            }
            else if (!text.rfind("map:", 0))
            {
                text.replace(0, 4, "");
                for (int i = 0; i < MAPSIZE; i++)
                    for (int j = 0; j < MAPSIZE; j++)
                        world->tilemap[i][j] = text[i * MAPSIZE + j] - '0';
            }
            else if (!text.rfind("time:", 0))
            {
                text.replace(0, 5, "");
                *timePassed = stoi(text);
            }
            else if (!text.rfind("hearts:", 0))
            {
                text.replace(0, 7, "");
                *hearts = stoi(text);
            }
            else if (!text.rfind("crystals:", 0))
            {
                text.replace(0, 9, "");
                *crystals = stoi(text);
            }
            else if (!text.rfind("monstersKilled:", 0))
            {
                text.replace(0, 15, "");
                *monstersKilled = stoi(text);
            }
            else if (!text.rfind("difficulty:", 0))
            {
                text.replace(0, 11, "");
                world->difficulty = stoi(text);
            }
    }
    file.close();

    return true;
}