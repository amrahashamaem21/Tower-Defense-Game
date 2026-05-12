#include "rocko.h"

Rocko::Rocko(float x, float y): Enemy(x, y, "Rocko", 400.f, 40.f, 25)
{
    loadTexture("Sprites/enemies/rocko.png");
}
void Rocko::update(float dt)
{
    if (!active) return;
    move_along_path(dt);
    update_hp_bar();
}
void Rocko::render(sf::RenderWindow& window)
{
    if (!active) return;
    window.draw(sprite);
    render_hp_bar(window);
}