#pragma once
#include "Enemy.h"
#include "slime.h"
#include "beebee.h"
#include "rocko.h"
#include "floaty.h"
#include "jelly.h"
#include <SFML/Graphics.hpp>
class Wave
{
public:
    static const int max_enemies = 60;
    static const int max_spawns = 30;
private:
    Enemy* enemies[max_enemies];
    int    enemy_count;
    int   spawn_queue_type[max_spawns];
    float spawn_queue_delay[max_spawns];
    int   spawn_count;
    int   spawn_index;        // next spawn jo process kerna hai
    float spawn_timer;
    int  wave_number;
    bool wave_complete;
    bool wave_started;
    sf::Vector2f* waypoints;
    int           way_point_count;
    sf::Vector2f  entryPoint;
    sf::Vector2f  exitPoint;
    int* gold_ref;            // game.cpp se pointer hai
    int* lives_ref;       
public:
    Wave(int* gold, int* lives);
    ~Wave();
    void set_way_points(sf::Vector2f* pts, int count,sf::Vector2f entry, sf::Vector2f exit);
    void set_up_wave(int waveNum, int level = 1);
    void start();
    void update(float dt);
    void render(sf::RenderWindow& window);
    void reset();
    bool is_complete()   const 
    { 
        return wave_complete;
    }
    bool has_started()   const 
    {
        return wave_started;
    }
    int  get_wave_num()   const 
    {
        return wave_number;
    }
    int  alive_count()   const;
    Enemy** get_enemies() 
    {
        return enemies; 
    }
    int     getenemy_count() 
    {
        return enemy_count; 
    }
private:
    void spawn_enemy(int type);
    void handle_splits();
};