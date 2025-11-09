#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "node.hpp"

class Enemy {
public:
    sf::Vector2f position;
    float speed;
    int health;
    std::vector<Node*> path;
    int currentNodeIndex;
    bool reachedGoal;

    Enemy(sf::Vector2f startPos);

    void setPath(const std::vector<Node*>& newPath);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

    bool isDead() const;
    void takeDamage(int dmg);
    void applySlow(float slowMultiplier); 
};
