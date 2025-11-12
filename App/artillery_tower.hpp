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
    
public:
    ArtilleryTower(sf::Vector2f pos);
    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) override;
    void draw(sf::RenderWindow& window) override;
    
    void setProjectileManager(ProjectileManager* pm) { projectileManager = pm; }

protected:
    ProjectileManager* projectileManager;
};