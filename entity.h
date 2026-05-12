#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
using namespace std;
class Entity
{
protected:
    float x, y;
    bool active;
    sf::Sprite sprite;
    sf::Texture texture;
    std::string name;

public:
    Entity(float x, float y, const string& name);
    virtual ~Entity();
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    float get_x() const;
    float get_y() const;
    bool is_active() const;
    std::string get_name() const;
    sf::Vector2f getPosition() const;
    void setPosition(float nx, float ny);
    void set_active(bool a);
    bool operator==(const Entity& other) const;
    bool operator!=(const Entity& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Entity& e);
};