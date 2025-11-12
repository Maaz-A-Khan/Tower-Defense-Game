#pragma once
#include "tower.hpp"

class ProjectileManager;
class Enemy;

class ArtilleryTower : public Tower {
private:
    float damage;
    float aoeRadius;
    float bulletSpeed;
    sf::CircleShape shape;
    ProjectileManager* projectileManager;
public:
    ArtilleryTower(sf::Vector2f pos, ProjectileManager* projManager);
    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) override;
    void draw(sf::RenderWindow& window) override;
};