#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

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
    std::optional<sf::Sprite> sprite;  // Optional sprite for textured projectiles
    static const float radius;

    Projectile(sf::Vector2f start, sf::Vector2f dir, float speed, int dmg, float aoe = 0.f, Enemy* target = nullptr);
    
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void setTexture(sf::Texture& texture);  // Method to set projectile texture
    
    bool checkCollision(Enemy* enemy);
};