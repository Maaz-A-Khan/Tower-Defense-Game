
#pragma once
#include <SFML/Graphics.hpp>
#include "enemy.hpp"
#include <memory>

class Projectile {
private:
    sf::Vector2f position;
    sf::Vector2f direction;
    float speed;
    int damage;
    bool active;
    float radius = 5.f; // simple circular hitbox

public:
    Projectile(sf::Vector2f start, sf::Vector2f dir, float spd, int dmg);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    bool checkCollision(Enemy& enemy);

    bool isActive() const { return active; }
    void deactivate() { active = false; }
};
