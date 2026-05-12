#pragma once
#include "Tower.h"

// ============================================
// CandyBlaster - Rapid fire, medium damage
// Effective against swarms of enemies
// ============================================
class CandyBlaster : public Tower
{
public:
    CandyBlaster(float x, float y);
    void update(float dt)                 override;
    void render(sf::RenderWindow& window) override;
    void attack(Enemy* target)            override;
    int  getUpgradeCost()                 override { return 100; }
    void upgrade()                        override;
};
