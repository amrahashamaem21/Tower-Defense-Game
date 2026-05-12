#pragma once
#include "Enemy.h"
class Floaty : public Enemy
{
private:
    sf::Vector2f exitPoint;  

public:
    Floaty(float x, float y);
    void setExitPoint(float ex, float ey);
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
};