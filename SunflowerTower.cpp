#include "SunflowerTower.h"
SunflowerTower::SunflowerTower(float x, float y): Tower(x, y, "SunflowerTower", 12.f, 140.f, 1.0f, 100)
{
    loadTexture("Sprites/towers/sunflower.png");
    sprite.setScale(0.1f, 0.1f); 
    bullet.setFillColor(sf::Color::Yellow);
    bullet.setRadius(4.f);
}
void SunflowerTower::update(float dt)
{
    if (!isPlaced) return;
    fire_cooldown -= dt;
}
void SunflowerTower::attack(Enemy* target)
{
    // Single target version (required by base class)
    if (!target || fire_cooldown > 0.f) return;
    target->take_damage(damage);
    fire_cooldown = 1.f / fire_rate;
}
void SunflowerTower::attackAll(Enemy** enemies, int count)
{
    // Area attack - hits ALL enemies in range at once
    if (fire_cooldown > 0.f) return;

    bool attacked = false;
    for (int i = 0; i < count; i++)
    {
        if (enemies[i] && enemies[i]->is_active() && inRange(enemies[i]))
        {
            enemies[i]->take_damage(damage);
            attacked = true;
        }
    }
    if (attacked)
        fire_cooldown = 1.f / fire_rate;
}
void SunflowerTower::upgrade()
{
    if (upgrade_level >= 2) return;
    upgrade_level++;
    damage += 8.f;
    range += 25.f;
    rangeCircle.setRadius(range);
    rangeCircle.setOrigin(range, range);
}
void SunflowerTower::render(sf::RenderWindow& window)
{
    if (!isPlaced) return;
    window.draw(sprite);
}