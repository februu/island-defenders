#pragma once

#include <string>
#include <vector>

class World;

class Entity
{
protected:
    int x, y;
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
    virtual float getMoveX();
    virtual float getMoveY();
    virtual int getDirection();
};

class Building : public Entity
{
private:
    int upgrades;

public:
    Building();
    ~Building();
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
    void chooseNewTile(int *new_x, int *new_y);

public:
    Enemy();
    ~Enemy();
    void performAction(double time);
    float getMoveX();
    float getMoveY();
    int getDirection();
};