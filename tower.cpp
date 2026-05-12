#include "Tower.h"
#include <cmath>
#include <iostream>
using namespace std;
Tower::Tower(float x, float y,const std::string& name,float damage, float range,float fire_rate, int cost): Entity(x, y, name),damage(damage), range(range),fire_rate(fire_rate), fire_cooldown(0.f),cost(cost), upgrade_level(0),isPlaced(false), bulletActive(false),bulletSpeed(300.f), currentTarget(nullptr)
{
    rangeCircle.setRadius(range);
    rangeCircle.setOrigin(range, range);
    rangeCircle.setFillColor(sf::Color(255, 255, 255, 30));
    rangeCircle.setOutlineColor(sf::Color(255, 255, 255, 120));
    rangeCircle.setOutlineThickness(1.f);
    rangeCircle.setPosition(x, y);
    bullet.setRadius(5.f);
    bullet.setOrigin(5.f, 5.f);
    bullet.setFillColor(sf::Color::Yellow);
}
Tower::~Tower() {}
bool Tower::loadTexture(const std::string& path)
{
    if (!texture.loadFromFile(path))
    {
        std::cerr << "Failed to load tower texture: " << path << endl;
        return false;
    }
    std::cerr << "SUCCESS loading: " << path << endl;
    sprite.setTexture(texture);
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    sprite.setScale(0.3f, 0.3f);
    sprite.setPosition(x, y);
    return true;
}
void Tower::setPosition(float nx, float ny)
{
    x = nx; y = ny;
    sprite.setPosition(nx, ny);
    rangeCircle.setPosition(nx, ny);
}
bool Tower::inRange(Enemy* enemy) const
{
    if (!enemy || !enemy->is_active()) 
    {
        return false;
    }
    float dx = enemy->get_x() - x;
    float dy = enemy->get_y() - y;
    float dist = sqrt(dx * dx + dy * dy);
    return dist <= range;
}
Enemy* Tower::findTarget(Enemy** enemies, int count)
{
    Enemy* best = nullptr;
    for (int i = 0; i < count; i++)
    {
        if (!enemies[i] || !enemies[i]->is_active()) 
        {
            continue;
        }
        if (inRange(enemies[i]))
        {
            if (!best)
                best = enemies[i];
        }
    }
    return best;
}
void Tower::updateBullet(float dt)
{
    if (!bulletActive) return;

    float dx = bulletTarget.x - bulletPos.x;
    float dy = bulletTarget.y - bulletPos.y;
    float dist = sqrt(dx * dx + dy * dy);
    if (dist < 8.f)
    {
        bulletActive = false;
        return;
    }
    bulletPos.x += (dx / dist) * bulletSpeed * dt;
    bulletPos.y += (dy / dist) * bulletSpeed * dt;
    bullet.setPosition(bulletPos);
}
void Tower::renderBullet(sf::RenderWindow& window)
{
    if (bulletActive)
        window.draw(bullet);
}
void Tower::renderRange(sf::RenderWindow& window)
{
    window.draw(rangeCircle);
}