#include "Entity.h"

Entity::Entity(float x, float y, const std::string& name)
{
    this->x = x;
    this->y = y;
    this->name = name;
    active = true;
}

Entity::~Entity() {}

float Entity::getX() const 
{
    return x;
}
float Entity::getY() const 
{ 
    return y;
}
bool Entity::isActive() const 
{ 
    return active; 
}
std::string Entity::getName() const
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

void Entity::setActive(bool a)
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

std::ostream& operator<<(std::ostream& os, const Entity& e)
{
    os << "[Entity] Name: " << e.name << " | Pos: (" << e.x << ", " << e.y << ")"<< " | Active: " << (e.active ? "Yes" : "No");
    return os;
}