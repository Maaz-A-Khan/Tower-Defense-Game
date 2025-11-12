#pragma once
#include <SFML/Graphics.hpp>

class Enemy;

class Projectile {
public:
    sf::Vector2f position;
    sf::Vector2f direction;
    float speed;
    int damage;
    bool active;
    
    float aoeRadius;
    Enemy* target;

    sf::CircleShape shape;
    static const float radius;

    Projectile(sf::Vector2f start, sf::Vector2f dir, float speed, int dmg, float aoe = 0.f, Enemy* target = nullptr);
    
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    
    bool checkCollision(Enemy* enemy);
};