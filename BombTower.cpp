#include "BombTower.h"
#include <cmath>
BombTower::BombTower(float x, float y) : Tower(x, y, "BombTower", 80.f, 110.f, 0.4f, 200),explosionRadius(70.f), explosionTimer(0.f), exploding(false), bulletX(0.f),bulletY(0.f)   
{
    loadTexture("Sprites/towers/bomb.png"); 
    bullet.setFillColor(sf::Color(50, 50, 50));
    bullet.setRadius(8.f);
    explosionCircle.setRadius(explosionRadius);
    explosionCircle.setOrigin(explosionRadius, explosionRadius);
    explosionCircle.setFillColor(sf::Color(255, 100, 0, 120));
    explosionCircle.setOutlineColor(sf::Color(255, 50, 0, 200));
    explosionCircle.setOutlineThickness(2.f);
}
void BombTower::update(float dt)
{
    if (!isPlaced) return;
    fire_cooldown -= dt;
    updateBullet(dt);
    bulletX = bulletPos.x;  
    bulletY = bulletPos.y;  
    if (exploding)
    {
        explosionTimer -= dt;
        sf::Color c = explosionCircle.getFillColor();
        c.a = (sf::Uint8)(120.f * (explosionTimer / 0.4f));
        explosionCircle.setFillColor(c);
        if (explosionTimer <= 0.f)
            exploding = false;
    }
}
void BombTower::attack(Enemy* target)
{
    if (!target || fire_cooldown > 0.f) return;
    fire_cooldown = 1.f / fire_rate;
    bulletPos = sf::Vector2f(x, y);
    bulletTarget = sf::Vector2f(target->get_x(), target->get_y());
    bulletActive = true;
    bullet.setPosition(bulletPos);

    bulletX = x;  
    bulletY = y;  
}
void BombTower::explode(Enemy** enemies, int count)
{
    if (!enemies || count <= 0) return;
    float ex = bulletX;  
    float ey = bulletY;
    float radius = explosionRadius;
    for (int i = 0; i < count; i++)
    {
        if (!enemies[i] || !enemies[i]->is_active()) continue;
        float dx = enemies[i]->get_x() - ex;
        float dy = enemies[i]->get_y() - ey;
        float dist = std::sqrt(dx * dx + dy * dy);
        if (dist <= radius)
            enemies[i]->take_damage(damage); 
    }
    exploding = true;
    explosionTimer = 0.3f;  
    explosionCircle.setRadius(radius);
    explosionCircle.setOrigin(radius, radius);
    explosionCircle.setPosition(ex, ey);
    explosionCircle.setFillColor(sf::Color(255, 165, 0, 200));
}
void BombTower::upgrade()
{
    if (upgrade_level >= 2) return;
    upgrade_level++;
    damage += 40.f;
    explosionRadius += 20.f;
}
void BombTower::render(sf::RenderWindow& window)
{
    if (!isPlaced) return;
    window.draw(sprite);
    renderBullet(window);
    if (exploding)
        window.draw(explosionCircle);
}