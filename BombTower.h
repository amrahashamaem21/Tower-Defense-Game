#pragma once
#include "Tower.h"
class BombTower : public Tower
{
private:
    float explosionRadius;    
    float explosionTimer;     
    bool  exploding;          
    sf::CircleShape explosionCircle;
    float bulletX;  
    float bulletY; 
public:
    BombTower(float x, float y);
    void update(float dt)                         override;
    void render(sf::RenderWindow& window)         override;
    void attack(Enemy* target)                    override;
    void explode(Enemy** enemies, int count);    
    int  getUpgradeCost()                         override { return 120; }
    void upgrade()                                override;
};