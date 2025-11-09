#pragma once
#include <SFML/Graphics.hpp>

class Tower {
protected:
    sf::Vector2f position;   // Tower position on the map
    float range;             // Attack range
    float damage;            // Damage per shot
    float attackSpeed;       // Shots per second
    float lastAttackTime;    // Time since last attack

public:
    Tower(sf::Vector2f pos, float r, float d, float speed);
    virtual ~Tower() = default;

    // Pure virtual functions to be implemented by derived classes
    virtual void attack() = 0;               // Define attack behavior
    virtual void upgrade() = 0;              // Define upgrade behavior
    virtual void draw(sf::RenderWindow &window) = 0; // Render tower

    // Getters
    sf::Vector2f getPosition() const;
    float getRange() const;
    float getDamage() const;
};
