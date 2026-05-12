#include "floaty.h"
#include <cmath>

Floaty::Floaty(float x, float y)
    : Enemy(x, y, "Floaty", 120.f, 120.f, 20),
    exitPoint(0.f, 0.f)
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

    // Fly in straight line toward exit (ignores path)
    float dx = exitPoint.x - x;
    float dy = exitPoint.y - y;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist < 5.f)
    {
        reachedEnd = true;
        active = false;
        return;
    }

    x += (dx / dist) * speed * dt;
    y += (dy / dist) * speed * dt;
    sprite.setPosition(x, y);

    updateHPBar();
}

void Floaty::render(sf::RenderWindow& window)
{
    if (!active) return;
    window.draw(sprite);
    renderHPBar(window);
}