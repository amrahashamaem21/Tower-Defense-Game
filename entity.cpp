#include "Entity.h"
using namespace std;
Entity::Entity(float x, float y, const string& name)
{
    this->x = x;
    this->y = y;
    this->name = name;
    active = true;
}
Entity::~Entity() {}

float Entity::get_x() const 
{
    return x;
}
float Entity::get_y() const 
{ 
    return y;
}
bool Entity::is_active() const 
{ 
    return active; 
}
string Entity::get_name() const
{
    return name;
}

sf::Vector2f Entity::getPosition() const
{
    return sf::Vector2f(x, y);
}

void Entity::setPosition(float nx, float ny)
{
    x = nx;
    y = ny;
}

void Entity::set_active(bool a)
{
    active = a;
}

bool Entity::operator==(const Entity& other) const
{
    return name == other.name;
}

bool Entity::operator!=(const Entity& other) const
{
    return name != other.name;
}

ostream& operator<<(ostream& os, const Entity& e)
{
    os << "[Entity] Name: " << e.name << " | Pos: (" << e.x << ", " << e.y << ")"<< " | Active: " << (e.active ? "Yes" : "No");
    return os;
}