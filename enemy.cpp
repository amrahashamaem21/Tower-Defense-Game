#include "Enemy.h"
#include <cmath>
#include <iostream>
sf::SoundBuffer Enemy::deathBuffer;
sf::Sound Enemy::deathSound;
Enemy::Enemy(float x, float y,const std::string& name,float hp,float speed,int gold_reward): Entity(x, y, name),max_hp(hp), current_hp(hp), speed(speed), gold_reward(gold_reward),path_index(1), reachedEnd(false),waypoints(nullptr), way_point_count(0)
{
    hp_bar_background.setSize(sf::Vector2f(48.f, 6.f));
    hp_bar_background.setFillColor(sf::Color(180, 0, 0));
    hp_bar_fill.setSize(sf::Vector2f(48.f, 6.f));
    hp_bar_fill.setFillColor(sf::Color(0, 220, 0));
    static bool soundLoaded = false;
    if (!soundLoaded)
    {
        if (!deathBuffer.loadFromFile("Sounds/enemy_died.wav"))
        {
            std::cout << "Failed to load enemy death sound\n";
        }

        deathSound.setBuffer(deathBuffer);
        soundLoaded = true;
    }
}
Enemy::~Enemy()
{
    delete[] waypoints;
}
void Enemy::set_way_points(sf::Vector2f* pts, int count)
{
    delete[] waypoints;
    waypoints = new sf::Vector2f[count];
    way_point_count = count;
    for (int i = 0; i < count; i++)
        waypoints[i] = pts[i];
    x = waypoints[0].x;
    y = waypoints[0].y;
}
void Enemy::move_along_path(float dt)
{
    if (path_index >= way_point_count)
    {
        reachedEnd = true;
        active = false;
        return;
    }
    sf::Vector2f target = waypoints[path_index];
    float dx = target.x - x;
    float dy = target.y - y;
    float dist = sqrt(dx * dx + dy * dy);
    if (dist < 3.f)
    {
        path_index++;
    }
    else
    {
        x += (dx / dist) * speed * dt;
        y += (dy / dist) * speed * dt;
    }
    sprite.setPosition(x, y);
}
void Enemy::take_damage(float damage)
{
    current_hp -= damage;
    if (current_hp <= 0)
    {
        current_hp = 0;
        deathSound.play();
        active = false;
    }
    update_hp_bar();
}
void Enemy::update_hp_bar()
{
    float ratio = current_hp / max_hp;
    if (ratio < 0.f) 
    {
        ratio = 0.f;
    }
    hp_bar_fill.setSize(sf::Vector2f(48.f * ratio, 6.f));
    if (ratio > 0.5f)
    {
        hp_bar_fill.setFillColor(sf::Color(0, 220, 0));
    }
    else if (ratio > 0.25f)
    {
        hp_bar_fill.setFillColor(sf::Color(255, 200, 0));
    }
    else
    {
        hp_bar_fill.setFillColor(sf::Color(220, 0, 0));
    }
}
void Enemy::render_hp_bar(sf::RenderWindow& window)
{
    float barX = x - 24.f;
    float barY = y - 40.f;
    hp_bar_background.setPosition(barX, barY);
    hp_bar_fill.setPosition(barX, barY);
    window.draw(hp_bar_background);
    window.draw(hp_bar_fill);
}
bool Enemy::loadTexture(const string& path)
{
    if (!texture.loadFromFile(path))
    {
        std::cerr << "Failed to load enemy texture: " << path << std::endl;
        return false;
    }
    sprite.setTexture(texture);
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height * 0.65f);
    float scaleX = 48.f / bounds.width;
    float scaleY = 48.f / bounds.height;
    sprite.setScale(scaleX, scaleY);
    return true;
}