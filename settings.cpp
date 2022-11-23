#include <iostream>
#include <fstream>
#include "headers\settings.h"

using namespace std;

// Creates new game.cfg file.
void createNewSettingsFile()
{
    ofstream file("game.cfg");
    string settings = "#   ______              ___                           __                 ____                 ___                     __                            \n#  /\\__  _\\            /\\_ \\                         /\\ \\               /\\  _`\\             /'___\\                   /\\ \\                           \n#  \\/_/\\ \\/      ____  \\//\\ \\       __       ___     \\_\\ \\              \\ \\ \\/\\ \\     __   /\\ \\__/    __     ___     \\_\\ \\      __    _ __    ____  \n#     \\ \\ \\     /',__\\   \\ \\ \\    /'__`\\   /' _ `\\   /'_` \\              \\ \\ \\ \\ \\  /'__`\\ \\ \\ ,__\\ /'__`\\ /' _ `\\   /'_` \\   /'__`\\ /\\`'__\\ /',__\\ \n#      \\_\\ \\__ /\\__, `\\   \\_\\ \\_ /\\ \\L\\.\\_ /\\ \\/\\ \\ /\\ \\L\\ \\              \\ \\ \\_\\ \\/\\  __/  \\ \\ \\_//\\  __/ /\\ \\/\\ \\ /\\ \\L\\ \\ /\\  __/ \\ \\ \\/ /\\__, `\\\n#      /\\_____\\\\/\\____/   /\\____\\\\ \\__/.\\_\\\\ \\_\\ \\_\\\\ \\___,_\\              \\ \\____/\\ \\____\\  \\ \\_\\ \\ \\____\\\\ \\_\\ \\_\\\\ \\___,_\\\\ \\____\\ \\ \\_\\ \\/\\____/\n#      \\/_____/ \\/___/    \\/____/ \\/__/\\/_/ \\/_/\\/_/ \\/__,_ /               \\/___/  \\/____/   \\/_/  \\/____/ \\/_/\\/_/ \\/__,_ / \\/____/  \\/_/  \\/___/                                                                                                      \n#\n#   Island Defenders v1.0\n#   Made by febru (Kacper Lutomski)\n#\n\n# Display Settings\nwidth=1920\nheight=1080\nfullscreenMode=true\n\n# Debug Mode\ndebugMode=false";
    file << settings;
    file.close();
    exit(0);
}

// TODO: Add values to settings array, add check if function fails.
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

// TODO: Loads game save from save.cfg file.
void loadSaveFile()
{
    return;
}