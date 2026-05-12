#pragma once
#include "Enemy.h"

// ============================================
// Floaty - Floaty
// Moves in straight line ignoring path/ground.
// Flies directly from entry to exit.
// ============================================
class Floaty : public Enemy
{
private:
    sf::Vector2f exitPoint;   // flies straight toward exit

public:
    Floaty(float x, float y);
    void setExitPoint(float ex, float ey);
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
};