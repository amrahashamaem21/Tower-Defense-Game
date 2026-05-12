#pragma once
#include "Tower.h"
class IceTower : public Tower
{
private:
    float slowFactor;    // itne percent speed reduce hogi
    float slowDuration;  // kab tak effect last kere ga
public:
    IceTower(float x, float y);
    void update(float dt)                 override;
    void render(sf::RenderWindow& window) override;
    void attack(Enemy* target)            override;
    int  getUpgradeCost()                 override { return 90; }
    void upgrade()                        override;
};