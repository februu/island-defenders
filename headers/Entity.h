#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include <vector>

struct visitedTile
{
    sf::Vector2i vertex;
    sf::Vector2i origin;
};

class World;

class Entity
{
protected:
    int x, y;
    int dest_x = -1, dest_y = -1;
    int xOffset, yOffset;
    int damage;
    int health;
    double timeToNextMove = 0;
    std::string type;
    std::string spriteName;
    World *world;

public:
    Entity();
    ~Entity();
    void createEntity(int x, int y, std::string type, std::string spriteName, int xOffset, int yOffset, int health, World *world);
    std::string getSpriteName();
    std::string getType();
    int getXOffset();
    int getYOffset();
    virtual void performAction(double time);
    int getX();
    int getY();
    World *getWorld();
    virtual float getMoveX();
    virtual float getMoveY();
    int getHealth();
    virtual double getTimeToNextMove();
    virtual int getIsMoving();
    virtual int getDirection();
    void decreaseHealth(int dmg);
    void setTimeToNextMove(double time);
};

class Building : public Entity
{
private:
    int upgrades;

public:
    Building();
    ~Building();
    void createProjectile(int target_x, int target_y, Entity *enemy);
    void performAction(double time);
};

class Enemy : public Entity
{
private:
    bool isMoving = false;
    int direction = 0; // 0 - gora, 1 - prawo, 2 - dol, 3 - lewo
    float moveX = 0.f, moveY = 0.f;
    int damage = 0;
    std::vector<int> debuffs;
    std::vector<sf::Vector2i> pathBFS;
    void chooseNewTile(int *new_x, int *new_y);
    bool checkForNearBuildings();

public:
    Enemy();
    ~Enemy();
    void performAction(double time);
    float getMoveX();
    float getMoveY();
    int getDirection();
    int getIsMoving();
    void findPath();
};