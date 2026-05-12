#pragma once
#include "Enemy.h"
class Rocko : public Enemy
{
public:
    Rocko(float x, float y);
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
};