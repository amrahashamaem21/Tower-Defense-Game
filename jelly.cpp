#include "Jelly.h"
Jelly::Jelly(float x, float y, bool is_mini): Enemy(x, y, is_mini ? "Mini_Jelly" : "Jelly",is_mini ? 60.f : 150.f,   is_mini ? 110.f : 90.f,   is_mini ? 0 : 30),is_mini(is_mini), has_split(false)
{
    loadTexture("Sprites/enemies/jelly.png");
    if (is_mini)
    {
        sprite.setScale(sprite.getScale().x * 0.6f, sprite.getScale().y * 0.6f);
    }
}
void Jelly::take_damage(float damage)
{
    current_hp -= damage;
    if (current_hp < 0) current_hp = 0;
    update_hp_bar();
    if (current_hp <= 0)
        active = false;
}
void Jelly::update(float dt)
{
    if (!active) return;
    move_along_path(dt);
    update_hp_bar();
}
void Jelly::render(sf::RenderWindow& window)
{
    if (!active) return;
    window.draw(sprite);
    render_hp_bar(window);
}