#include "IceTower.h"
IceTower::IceTower(float x, float y) : Tower(x, y, "IceTower", 15.f, 120.f, 1.2f, 120),slowFactor(0.5f), slowDuration(2.f)
{
    loadTexture("Sprites/towers/ice_tower.png");
    bullet.setFillColor(sf::Color(100, 200, 255));
    bullet.setRadius(5.f);
}
void IceTower::update(float dt)
{
    if (!isPlaced) return;
    fire_cooldown -= dt;
    updateBullet(dt);
}
void IceTower::attack(Enemy* target)
{
    if (!target || fire_cooldown > 0.f) return;
    target->take_damage(damage);
   // reduce enemy speed temporarily
    float originalSpeed = target->getSpeed();
    target->setSpeed(originalSpeed * slowFactor);
    fire_cooldown = 1.f / fire_rate;
    bulletPos = sf::Vector2f(x, y);
    bulletTarget = sf::Vector2f(target->get_x(), target->get_y());
    bulletActive = true;
    bullet.setPosition(bulletPos);
}
void IceTower::upgrade()
{
    if (upgrade_level >= 2) return;
    upgrade_level++;
    slowFactor = 0.3f;   // slower than usual
    range += 15.f;
    rangeCircle.setRadius(range);
    rangeCircle.setOrigin(range, range);
}
void IceTower::render(sf::RenderWindow& window)
{
    if (!isPlaced) return;
    window.draw(sprite);
    renderBullet(window);
}