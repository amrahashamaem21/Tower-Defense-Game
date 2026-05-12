#pragma once
#include "Entity.h"
#include "Enemy.h"
using namespace std;
class Tower : public Entity
{
protected:
    float  damage;
    float  range;
    float  fire_rate;       
    float  fire_cooldown;   
    int    cost;
    int    upgrade_level;   
    bool   isPlaced;
    sf::CircleShape rangeCircle;
    sf::CircleShape bullet;
    sf::Vector2f    bulletPos;
    sf::Vector2f    bulletTarget;
    bool            bulletActive;
    float           bulletSpeed;
    Enemy* currentTarget;  
public:
    Tower(float x, float y,
        const std::string& name,
        float damage,
        float range,
        float fire_rate,
        int cost);
    virtual ~Tower();
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void attack(Enemy* target) = 0;
    virtual int  getUpgradeCost() = 0;
    virtual void upgrade() = 0;
    Enemy* findTarget(Enemy** enemies, int count);  
    void   updateBullet(float dt);
    void   renderBullet(sf::RenderWindow& window);
    void   renderRange(sf::RenderWindow& window);
    bool   loadTexture(const std::string& path);
    bool   inRange(Enemy* enemy) const;
    float getDamage()   const { return damage; }
    float getRange()    const { return range; }
    float getfire_rate() const { return fire_rate; }
    int   getCost()     const { return cost; }
    int   getLevel()    const { return upgrade_level; }
    bool  getIsPlaced() const { return isPlaced; }
    bool isBulletActive() const { return bulletActive; }
    void  setPlaced(bool p) { isPlaced = p; }
    void  setPosition(float nx, float ny);
};