#pragma once
#include"tower.h"
class CannonTower : public Tower
{
public:
    CannonTower(float x, float y);
    void update(float dt)                 override;
    void render(sf::RenderWindow& window) override;
    void attack(Enemy* target)            override;
    int  getUpgradeCost()                 override { return 80; }
    void upgrade()                        override;
};