#include <SFML\Graphics.hpp>
#include <iostream>
#include <cmath>
#include "headers\settings.h"

using namespace std;
using namespace sf;

int main()
{

    int settings[4];
    loadSettings(settings);

    // ### Constants ###
    const int mapSize = 20;
    const float tileScale = 3.f;
    const int tileSize = 16 * tileScale; // Actual sprite is 64x32.
    const int mapXOffset = settings[0] / 2;
    const int mapYOffset = (settings[1] - tileSize * mapSize) / 2;
    // #################

    RenderWindow window(VideoMode(settings[0], settings[1]), "Island Defenders", Style::Fullscreen);

    //-------------------------

    sf::Texture texture;
    if (!texture.loadFromFile("assets/tile.png"))
    {
        cout << "O nie! cos sie zajabeo.\n";
    }

    sf::Font font;
    font.loadFromFile("assets/callingcode.ttf");

    //--------------------------

    Clock clock;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed)
                if (event.key.code == Keyboard::Escape)
                    window.close();
        }
        window.clear();

        // MOUSE

        int x1 = sf::Mouse::getPosition().x - mapXOffset;        // translate to orgin here
        int y1 = (sf::Mouse::getPosition().y - mapYOffset) * -2; // translate to orgin here
        double xr = cos(M_PI / 4) * x1 - sin(M_PI / 4) * y1;
        double yr = sin(M_PI / 4) * x1 + cos(M_PI / 4) * y1;
        double diag = tileSize * sqrt(2);
        int x2 = floor(xr / diag);
        int y2 = floor(yr * -1 / diag);

        // MOUSE

        for (int i = 0; i < mapSize; i++) // height
        {
            for (int j = 0; j < mapSize; j++) // width
            {
                sf::Sprite sprite;
                sprite.setTexture(texture);
                sprite.setScale(sf::Vector2f(tileScale, tileScale));
                if (j == x2 && i == y2)
                    sprite.setColor(sf::Color::Red);
                int x = tileSize * j - tileSize * i - tileSize + mapXOffset;
                int y = (tileSize * i + tileSize * j) / 2 + mapYOffset;
                sprite.setPosition(sf::Vector2f(x, y));
                window.draw(sprite);
            }
        }

        sf::Text text("hoveredTile=" + to_string(x2) + "," + to_string(y2), font);
        text.setCharacterSize(15);
        text.setPosition(sf::Vector2f(10, 10));
        text.setFillColor(sf::Color::White);
        window.draw(text);

        sf::Text text2("mousePos=" + to_string(sf::Mouse::getPosition().x) + "," + to_string(sf::Mouse::getPosition().y), font);
        text2.setCharacterSize(15);
        text2.setPosition(sf::Vector2f(10, 25));
        text2.setFillColor(sf::Color::White);
        window.draw(text2);

        window.display();
    }
    return 0;
}