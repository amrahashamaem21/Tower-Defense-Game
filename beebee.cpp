#include "Beebee.h"
Beebee::Beebee(float x, float y): Enemy(x, y, "Beebee", 60.f, 160.f, 15)
{
    loadTexture("Sprites/enemies/beebee.png");
}
void Beebee::update(float dt)
{
    if (!active) return;
    move_along_path(dt);
    update_hp_bar();
}
void Beebee::render(sf::RenderWindow& window)
{
    if (!active) return;
    window.draw(sprite);
    render_hp_bar(window);
}