#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

class Projectile;
class Enemy;

class ProjectileManager {
private:
    std::vector<std::unique_ptr<Projectile>> projectiles;

public:
    ProjectileManager();

    void spawnProjectile(sf::Vector2f start, sf::Vector2f dir, float speed, int dmg, float aoeRadius = 0.f, Enemy* target = nullptr);
    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies);
    void draw(sf::RenderWindow& window);

private:
    void applyAoEDamage(sf::Vector2f center, float radius, int damage, std::vector<std::unique_ptr<Enemy>>& enemies);
};