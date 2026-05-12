#pragma once
#include "Enemy.h"
class Slime : public Enemy
{
public:
   Slime(float x, float y);
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
};