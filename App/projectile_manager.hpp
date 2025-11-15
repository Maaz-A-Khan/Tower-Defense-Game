#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "projectile.hpp"

class Enemy;
class Projectile;
class AssetManager;

struct Explosion {
    sf::Vector2f position;
    float elapsed;
    int currentFrame;
    bool active;
    
    Explosion(sf::Vector2f pos) 
        : position(pos), elapsed(0.f), currentFrame(0), active(true) {}
};

class ProjectileManager {
private:
    std::vector<std::unique_ptr<Projectile>> projectiles;
    std::vector<Explosion> explosions;  // Add this line
    AssetManager* assetManager;

    void applyAoEDamage(sf::Vector2f center, float radius, int damage, std::vector<std::unique_ptr<Enemy>>& enemies);

public:
    ProjectileManager(AssetManager* assets = nullptr);

    void spawnProjectile(sf::Vector2f start, sf::Vector2f dir, float speed, int dmg, float aoeRadius = 0.f, Enemy* target = nullptr, ProjectileType type = ProjectileType::Gatling);
    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies);
    void draw(sf::RenderWindow& window);
};