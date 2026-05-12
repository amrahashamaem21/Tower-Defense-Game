#include "Wave.h"
#include <iostream>

Wave::Wave(int* gold, int* lives) : enemy_count(0), spawn_count(0), spawn_index(0), spawn_timer(0.f), wave_number(0), wave_complete(false), wave_started(false), waypoints(nullptr), way_point_count(0), gold_ref(gold), lives_ref(lives)
{
    for (int i = 0; i < max_enemies; i++)
        enemies[i] = nullptr;
    for (int i = 0; i < max_spawns; i++)
    {
        spawn_queue_type[i] = 0;
        spawn_queue_delay[i] = 0.f;
    }
}
Wave::~Wave()
{
    reset();
}
void Wave::reset()
{
    for (int i = 0; i < max_enemies; i++)
    {
        delete enemies[i];
        enemies[i] = nullptr;
    }
    enemy_count = 0;
    spawn_index = 0;
    spawn_timer = 0.f;
    wave_complete = false;
    wave_started = false;
}
void Wave::set_way_points(sf::Vector2f* pts, int count, sf::Vector2f entry, sf::Vector2f exit)
{
    waypoints = pts;
    way_point_count = count;
    entryPoint = entry;
    exitPoint = exit;
}
void Wave::set_up_wave(int waveNum, int level)
{
    reset();
    wave_number = waveNum;
    spawn_count = 0;
    if (level == 1)
    {
        if (waveNum == 1)
        {
            for (int i = 0; i < 6; i++)
            {
                spawn_queue_type[spawn_count] = 0;
                spawn_queue_delay[spawn_count] = i * 1.5f;
                spawn_count++;
            }
        }
        else if (waveNum == 2)
        {
            for (int i = 0; i < 6; i++)//slime ke liye 
            {
                spawn_queue_type[spawn_count] = 0;
                spawn_queue_delay[spawn_count] = i * 1.4f;
                spawn_count++;
            }
            for (int i = 0; i < 4; i++)
            {
                spawn_queue_type[spawn_count] = 1;
                spawn_queue_delay[spawn_count] = 8.f + i * 1.0f;
                spawn_count++;
            }
        }
        else if (waveNum == 3)
        {
            for (int i = 0; i < 4; i++)
            {
                spawn_queue_type[spawn_count] = 1;
                spawn_queue_delay[spawn_count] = i * 1.0f;
                spawn_count++;
            }
            for (int i = 0; i < 3; i++)
            {
                spawn_queue_type[spawn_count] = 2;
                spawn_queue_delay[spawn_count] = 5.f + i * 3.f;
                spawn_count++;
            }
            for (int i = 0; i < 4; i++)
            {
                spawn_queue_type[spawn_count] = 0;
                spawn_queue_delay[spawn_count] = 15.f + i * 1.2f;
                spawn_count++;
            }
        }
        else if (waveNum == 4)
        {
            for (int i = 0; i < 4; i++)
            {
                spawn_queue_type[spawn_count] = 0;
                spawn_queue_delay[spawn_count] = i * 1.2f;
                spawn_count++;
            }
            for (int i = 0; i < 4; i++)
            {
                spawn_queue_type[spawn_count] = 3;
                spawn_queue_delay[spawn_count] = 5.f + i * 1.5f;
                spawn_count++;
            }
            for (int i = 0; i < 3; i++)
            {
                spawn_queue_type[spawn_count] = 2;
                spawn_queue_delay[spawn_count] = 12.f + i * 3.f;
                spawn_count++;
            }
        }
        else if (waveNum == 5)
        {
            for (int i = 0; i < 4; i++)
            {
                spawn_queue_type[spawn_count] = 1;
                spawn_queue_delay[spawn_count] = i * 0.8f;
                spawn_count++;
            }
            for (int i = 0; i < 3; i++)
            {
                spawn_queue_type[spawn_count] = 4;
                spawn_queue_delay[spawn_count] = 4.f + i * 2.f;
                spawn_count++;
            }
            for (int i = 0; i < 3; i++)
            {
                spawn_queue_type[spawn_count] = 2;
                spawn_queue_delay[spawn_count] = 10.f + i * 3.f;
                spawn_count++;
            }
            for (int i = 0; i < 3; i++)
            {
                spawn_queue_type[spawn_count] = 3;
                spawn_queue_delay[spawn_count] = 20.f + i * 1.5f;
                spawn_count++;
            }
        }
    }
    else if (level == 2)
    {
        if (waveNum == 1)
        {
            for (int i = 0; i < 5; i++)
            {
                spawn_queue_type[spawn_count] = 0;
                spawn_queue_delay[spawn_count] = i * 1.2f;
                spawn_count++;
            }
            for (int i = 0; i < 4; i++)
            {
                spawn_queue_type[spawn_count] = 1;
                spawn_queue_delay[spawn_count] = 6.f + i * 0.8f;
                spawn_count++;
            }
        }
        else if (waveNum == 2)
        {
            for (int i = 0; i < 5; i++)
            {
                spawn_queue_type[spawn_count] = 1;
                spawn_queue_delay[spawn_count] = i * 0.9f;
                spawn_count++;
            }
            for (int i = 0; i < 4; i++)
            {
                spawn_queue_type[spawn_count] = 2;
                spawn_queue_delay[spawn_count] = 5.f + i * 2.5f;
                spawn_count++;
            }
            for (int i = 0; i < 3; i++)
            {
                spawn_queue_type[spawn_count] = 0;
                spawn_queue_delay[spawn_count] = 15.f + i * 1.0f;
                spawn_count++;
            }
        }
        else if (waveNum == 3)
        {
            for (int i = 0; i < 5; i++)
            {
                spawn_queue_type[spawn_count] = 2;
                spawn_queue_delay[spawn_count] = i * 2.0f;
                spawn_count++;
            }
            for (int i = 0; i < 5; i++)
            {
                spawn_queue_type[spawn_count] = 3;
                spawn_queue_delay[spawn_count] = 2.f + i * 1.2f;
                spawn_count++;
            }
            for (int i = 0; i < 3; i++)
            {
                spawn_queue_type[spawn_count] = 1;
                spawn_queue_delay[spawn_count] = 14.f + i * 0.8f;
                spawn_count++;
            }
        }
        else if (waveNum == 4)
        {
            for (int i = 0; i < 4; i++)
            {
                spawn_queue_type[spawn_count] = 1;
                spawn_queue_delay[spawn_count] = i * 0.7f;
                spawn_count++;
            }
            for (int i = 0; i < 4; i++)
            {
                spawn_queue_type[spawn_count] = 4;
                spawn_queue_delay[spawn_count] = 4.f + i * 1.5f;
                spawn_count++;
            }
            for (int i = 0; i < 3; i++)
            {
                spawn_queue_type[spawn_count] = 2;
                spawn_queue_delay[spawn_count] = 12.f + i * 2.f;
                spawn_count++;
            }
            for (int i = 0; i < 3; i++)
            {
                spawn_queue_type[spawn_count] = 3;
                spawn_queue_delay[spawn_count] = 18.f + i * 1.f;
                spawn_count++;
            }
        }
        else if (waveNum == 5)
        {
            for (int i = 0; i < 5; i++)
            {
                spawn_queue_type[spawn_count] = 1;
                spawn_queue_delay[spawn_count] = i * 0.6f;
                spawn_count++;
            }
            for (int i = 0; i < 4; i++)
            {
                spawn_queue_type[spawn_count] = 4;
                spawn_queue_delay[spawn_count] = 3.f + i * 1.2f;
                spawn_count++;
            }
            for (int i = 0; i < 4; i++)
            {
                spawn_queue_type[spawn_count] = 2;
                spawn_queue_delay[spawn_count] = 9.f + i * 2.f;
                spawn_count++;
            }
            for (int i = 0; i < 4; i++)
            {
                spawn_queue_type[spawn_count] = 3;
                spawn_queue_delay[spawn_count] = 17.f + i * 1.f;
                spawn_count++;
            }
            for (int i = 0; i < 3; i++)
            {
                spawn_queue_type[spawn_count] = 0;
                spawn_queue_delay[spawn_count] = 23.f + i * 0.8f;
                spawn_count++;
            }
        }
    }
    else if (level == 3)
    {
        if (waveNum == 1)
        {
            for (int i = 0; i < 6; i++)
            {
                spawn_queue_type[spawn_count] = 1;
                spawn_queue_delay[spawn_count] = i * 0.7f;
                spawn_count++;
            }
            for (int i = 0; i < 3; i++)
            {
                spawn_queue_type[spawn_count] = 2;
                spawn_queue_delay[spawn_count] = 5.f + i * 2.f;
                spawn_count++;
            }
        }
        else if (waveNum == 2)
        {
            for (int i = 0; i < 5; i++)
            {
                spawn_queue_type[spawn_count] = 2;
                spawn_queue_delay[spawn_count] = i * 1.8f;
                spawn_count++;
            }
            for (int i = 0; i < 6; i++)
            {
                spawn_queue_type[spawn_count] = 3;
                spawn_queue_delay[spawn_count] = 2.f + i * 0.9f;
                spawn_count++;
            }
            for (int i = 0; i < 4; i++)
            {
                spawn_queue_type[spawn_count] = 1;
                spawn_queue_delay[spawn_count] = 14.f + i * 0.6f;
                spawn_count++;
            }
        }
        else if (waveNum == 3)
        {
            for (int i = 0; i < 6; i++)
            {
                spawn_queue_type[spawn_count] = 4;
                spawn_queue_delay[spawn_count] = i * 1.0f;
                spawn_count++;
            }
            for (int i = 0; i < 5; i++)
            {
                spawn_queue_type[spawn_count] = 3;
                spawn_queue_delay[spawn_count] = 6.f + i * 0.8f;
                spawn_count++;
            }
            for (int i = 0; i < 4; i++)
            {
                spawn_queue_type[spawn_count] = 2;
                spawn_queue_delay[spawn_count] = 12.f + i * 1.5f;
                spawn_count++;
            }
        }
        else if (waveNum == 4)
        {
            for (int i = 0; i < 5; i++)
            {
                spawn_queue_type[spawn_count] = 1;
                spawn_queue_delay[spawn_count] = i * 0.5f;
                spawn_count++;
            }
            for (int i = 0; i < 5; i++)
            {
                spawn_queue_type[spawn_count] = 4;
                spawn_queue_delay[spawn_count] = 2.5f + i * 0.9f;
                spawn_count++;
            }
            for (int i = 0; i < 5; i++)
            {
                spawn_queue_type[spawn_count] = 2;
                spawn_queue_delay[spawn_count] = 8.f + i * 1.5f;
                spawn_count++;
            }
            for (int i = 0; i < 5; i++)
            {
                spawn_queue_type[spawn_count] = 3;
                spawn_queue_delay[spawn_count] = 15.f + i * 0.7f;
                spawn_count++;
            }
        }
        else if (waveNum == 5)
        {
            for (int i = 0; i < 6; i++)
            {
                spawn_queue_type[spawn_count] = 1;
                spawn_queue_delay[spawn_count] = i * 0.4f;
                spawn_count++;
            }
            for (int i = 0; i < 6; i++)
            {
                spawn_queue_type[spawn_count] = 4;
                spawn_queue_delay[spawn_count] = 2.f + i * 0.8f;
                spawn_count++;
            }
            for (int i = 0; i < 5; i++)
            {
                spawn_queue_type[spawn_count] = 2;
                spawn_queue_delay[spawn_count] = 8.f + i * 1.2f;
                spawn_count++;
            }
            for (int i = 0; i < 6; i++)
            {
                spawn_queue_type[spawn_count] = 3;
                spawn_queue_delay[spawn_count] = 14.f + i * 0.6f;
                spawn_count++;
            }
            for (int i = 0; i < 4; i++)
            {
                spawn_queue_type[spawn_count] = 4;
                spawn_queue_delay[spawn_count] = 22.f + i * 0.9f;
                spawn_count++;
            }
        }
    }
}
void Wave::start()
{
    wave_started = true;
    spawn_timer = 0.f;
}
void Wave::spawn_enemy(int type)
{
    if (enemy_count >= max_enemies)
        return;

    Enemy* e = nullptr;
    float  ex = entryPoint.x;
    float  ey = entryPoint.y;

    switch (type)
    {
    case 0: 
    {
        e = new Slime(ex, ey);
        break;
    }
    case 1: 
    {
        e = new Beebee(ex, ey);
        break;
    }
    case 2:
    {
        e = new Rocko(ex, ey);
        break;
    }
    case 3:
    {
        Floaty* fe = new Floaty(ex, ey);
        fe->setExitPoint(exitPoint.x, exitPoint.y);
        e = fe;
        break;
    }
    case 4: 
    {
        e = new Jelly(ex, ey); 
        break;
    }
    default:
        return;
    }

    e->set_way_points(waypoints, way_point_count);
    enemies[enemy_count++] = e;
}
void Wave::handle_splits()
{
    for (int i = 0; i < enemy_count; i++)
    {
        Jelly* se = dynamic_cast<Jelly*>(enemies[i]);
        if (se && se->shouldSplit())
        {
            se->mark_split();
            for (int m = 0; m < 2 && enemy_count < max_enemies; m++)
            {
                Jelly* mini = new Jelly(se->get_x(), se->get_y(), true);
                mini->set_way_points(waypoints, way_point_count);
                enemies[enemy_count++] = mini;
            }
        }
    }
}
void Wave::update(float dt)
{
    if (!wave_started)
        return;
    spawn_timer += dt;
    while (spawn_index < spawn_count &&
        spawn_timer >= spawn_queue_delay[spawn_index]) 
    {
        spawn_enemy(spawn_queue_type[spawn_index]);     
        spawn_index++;
    }
    handle_splits();
    for (int i = 0; i < enemy_count; i++)
    {
        if (!enemies[i]) continue;

        enemies[i]->update(dt);
        if (enemies[i]->hasReachedEnd())
        {
            if (lives_ref && *lives_ref > 0)
                (*lives_ref)--;
            delete enemies[i];
            enemies[i] = nullptr;
        }
        else if (enemies[i]->isDead() && !enemies[i]->is_active())
        {
            if (gold_ref)
                *gold_ref += enemies[i]->getgold_reward();
            delete enemies[i];
            enemies[i] = nullptr;
        }
    }
    if (spawn_index >= spawn_count && alive_count() == 0)
        wave_complete = true;
}
void Wave::render(sf::RenderWindow& window)
{
    for (int i = 0; i < enemy_count; i++)
        if (enemies[i] && enemies[i]->is_active())
            enemies[i]->render(window);
}
int Wave::alive_count() const
{
    int count = 0;
    for (int i = 0; i < enemy_count; i++)
        if (enemies[i] && enemies[i]->is_active())
            count++;
    return count;
}