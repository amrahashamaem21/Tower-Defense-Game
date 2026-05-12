#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class Entity
{
protected:
    float x, y;
    bool active;
    sf::Sprite sprite;
    sf::Texture texture;
    std::string name;

public:
    Entity(float x, float y, const std::string& name);
    virtual ~Entity();

    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    float getX() const;
    float getY() const;
    bool isActive() const;
    std::string getName() const;

    sf::Vector2f getPosition() const;
    void setPosition(float nx, float ny);
    void setActive(bool a);

    bool operator==(const Entity& other) const;
    bool operator!=(const Entity& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Entity& e);
};