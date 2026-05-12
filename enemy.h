#pragma once
#include <SFML/Audio.hpp>
#include "Entity.h"
using namespace std;
class Enemy : public Entity
{
protected:
    float max_hp;
    float current_hp;
    float speed;
    int   gold_reward;     // gold player earns on kill
    int   path_index;      // which waypoint enemy is heading to
    bool  reachedEnd;     // did enemy reach the exit?
    sf::RectangleShape hp_bar_background;
    sf::RectangleShape hp_bar_fill;
    static const int max_waypoints = 20;
    sf::Vector2f* waypoints;
    int way_point_count;

public:
    Enemy(float x, float y,const string& name,float hp,float speed,int gold_reward);
    virtual ~Enemy();
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void take_damage(float damage);      
    void        move_along_path(float dt);        
    void        set_way_points(sf::Vector2f* pts, int count);
    void        update_hp_bar();
    void        render_hp_bar(sf::RenderWindow& window);
    bool        loadTexture(const std::string& path);
    float getHP() const 
    { 
        return current_hp;
    }
    float getmax_hp() const
    {
        return max_hp;
    }
    float getSpeed() const 
    {
        return speed; 
    }
    int   getgold_reward()const 
    { 
        return gold_reward; 
    }
    bool  hasReachedEnd()const 
    { 
        return reachedEnd;
    }
    static sf::SoundBuffer deathBuffer;
    static sf::Sound deathSound;
    bool  isDead()       const
    { 
        return current_hp <= 0; 
    }
    void setSpeed(float s) 
    { 
        speed = s; 
    }
};