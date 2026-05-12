#include <SFML/Graphics.hpp>
#include "slime.h"
Slime::Slime(float x, float y): Enemy(x, y, "basic_enemy", 100.f, 80.f, 10)
{
    loadTexture("Sprites/enemies/slime.png");
}
void Slime::update(float dt)
{
    if (!active) 
    {
        return;
    }
    move_along_path(dt);
    update_hp_bar();
}
void Slime::render(sf::RenderWindow& window)
{
    if (!active) return;
    window.draw(sprite);
    render_hp_bar(window);
}
