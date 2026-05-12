#include "floaty.h"
#include <cmath>
using namespace std;
Floaty::Floaty(float x, float y): Enemy(x, y, "Floaty", 120.f, 120.f, 20),exitPoint(0.f, 0.f)
{
    loadTexture("Sprites/enemies/floaty.png");
}
void Floaty::setExitPoint(float ex, float ey)
{
    exitPoint = sf::Vector2f(ex, ey);
}
void Floaty::update(float dt)
{
    if (!active) return;
    float dx = exitPoint.x - x;
    float dy = exitPoint.y - y;
    float dist =sqrt(dx * dx + dy * dy);

    if (dist < 5.f)
    {
        reachedEnd = true;
        active = false;
        return;
    }
    x += (dx / dist) * speed * dt;
    y += (dy / dist) * speed * dt;
    sprite.setPosition(x, y);
    update_hp_bar();
}
void Floaty::render(sf::RenderWindow& window)
{
    if (!active) return;
    window.draw(sprite);
    render_hp_bar(window);
}