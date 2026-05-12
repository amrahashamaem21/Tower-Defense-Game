#pragma once
#include "Enemy.h"
class Jelly : public Enemy
{
private:
    bool  is_mini;         
    bool  has_split;         

public:
    Jelly(float x, float y, bool is_mini = false);
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    void take_damage(float damage) override; 
    bool  shouldSplit() const 
    { 
        return !is_mini && !has_split && current_hp <= 0; 
    }
    bool  getis_mini()   const 
    {
        return is_mini;
    }
    void  mark_split() 
    {
        has_split = true;
    }
};
