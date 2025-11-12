#pragma once
#include "tower.hpp"

class ProjectileManager;
class Enemy;

class GatlingTower : public Tower {
private:
    float damage;
    float bulletSpeed;
    sf::CircleShape shape;
    ProjectileManager* projectileManager; 

public:
    GatlingTower(sf::Vector2f pos, ProjectileManager* projManager);
    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) override;
    void draw(sf::RenderWindow& window) override;
};