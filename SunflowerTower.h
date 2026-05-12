#pragma once
#include "Tower.h"
class SunflowerTower : public Tower
{
public:
    SunflowerTower(float x, float y);
    void update(float dt)                             override;
    void render(sf::RenderWindow& window)             override;
    void attack(Enemy* target)                        override;
    void attackAll(Enemy** enemies, int count);   // range main sabko hit
    int  getUpgradeCost()                             override { return 75; }
    void upgrade()                                    override;
};