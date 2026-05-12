#include "CandyBlaster.h"
CandyBlaster::CandyBlaster(float x, float y): Tower(x, y, "CandyBlaster", 20.f, 115.f, 2.5f, 150)
{
    loadTexture("Sprites/towers/candy_blaster.png");
    bullet.setFillColor(sf::Color(255, 100, 180));//pink
    bullet.setRadius(4.f);
}
void CandyBlaster::update(float dt)
{
    if (!isPlaced) return;
    fire_cooldown -= dt;
    updateBullet(dt);
}
void CandyBlaster::attack(Enemy* target)
{
    if (!target || fire_cooldown > 0.f) return;
    target->take_damage(damage);
    fire_cooldown = 1.f / fire_rate;
    bulletPos = sf::Vector2f(x, y);
    bulletTarget = sf::Vector2f(target->get_x(), target->get_y());
    bulletActive = true;
    bullet.setPosition(bulletPos);
}
void CandyBlaster::upgrade()
{
    if (upgrade_level >= 2) 
    {
        return;
    }
    upgrade_level++;
    damage += 10.f;
    fire_rate += 1.0f;
}
void CandyBlaster::render(sf::RenderWindow& window)
{
    if (!isPlaced) return;
    window.draw(sprite);
    renderBullet(window);
}