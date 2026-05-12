#pragma once
#include "Enemy.h"
class Beebee : public Enemy
{
public:
    Beebee(float x, float y);
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
};
