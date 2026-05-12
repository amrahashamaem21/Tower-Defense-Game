#include "CannonTower.h"
CannonTower::CannonTower(float x, float y): Tower(x, y, "CannonTower", 50.f, 130.f, 0.8f, 100)
{
    loadTexture("Sprites/towers/cannon_tower.png");
    bullet.setFillColor(sf::Color(50, 50, 50));
    bullet.setRadius(6.f);
}
void CannonTower::update(float dt)
{
    if (!isPlaced) return;
    fire_cooldown -= dt;
    updateBullet(dt);
}
void CannonTower::attack(Enemy* target)
{
    if (!target || fire_cooldown > 0.f) return;
    target->take_damage(damage);
    fire_cooldown = 1.f / fire_rate;
    bulletPos = sf::Vector2f(x, y);
    bulletTarget = sf::Vector2f(target->get_x(), target->get_y());
    bulletActive = true;
    bullet.setPosition(bulletPos);
}
void CannonTower::upgrade()
{
    if (upgrade_level >= 2) return;
    upgrade_level++;
    damage *= 1.5f;
    range += 20.f;
    rangeCircle.setRadius(range);
    rangeCircle.setOrigin(range, range);
}
void CannonTower::render(sf::RenderWindow& window)
{
    if (!isPlaced) return;
    window.draw(sprite);
    renderBullet(window);
}