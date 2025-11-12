
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "projectile.hpp"
#include "enemy.hpp"

class ProjectileManager {
private:
    std::vector<std::unique_ptr<Projectile>> projectiles;

public:
    void spawnProjectile(sf::Vector2f start, sf::Vector2f dir, float speed, int dmg);
    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies);
    void draw(sf::RenderWindow& window);
};
