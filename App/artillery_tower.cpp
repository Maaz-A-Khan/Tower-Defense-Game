#include "artillery_tower.hpp"
#include "projectile_manager.hpp"
#include "enemy.hpp"
#include <cmath>

ArtilleryTower::ArtilleryTower(sf::Vector2f pos)
    : Tower(pos, 200.f, 0.5f, 250, false, TowerType::Artillery),
      damage(50.f),
      aoeRadius(60.f),
      bulletSpeed(250.f),
      projectileManager(nullptr)
{
    shape.setRadius(25.f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin({shape.getRadius(), shape.getRadius()});
    shape.setPosition(position);
}

void ArtilleryTower::update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (cooldown > 0) {
        cooldown -= deltaTime;
        return;
    }
    
    if (!projectileManager) return; // Safety check

    Enemy* target = nullptr;
    float minDistance = range * range;
    
    for (auto& enemy : enemies) {
        if (enemy->isDead()) continue;
        
        sf::Vector2f enemyPos = enemy->getPosition();
        float dx = position.x - enemyPos.x;
        float dy = position.y - enemyPos.y;
        float distanceSq = dx * dx + dy * dy;
        
        if (distanceSq <= minDistance) {
            minDistance = distanceSq;
            target = enemy.get();
        }
    }

    if (target) {
        sf::Vector2f direction = target->getPosition() - position;
        
        projectileManager->spawnProjectile(
            position, 
            direction, 
            bulletSpeed, 
            static_cast<int>(damage), 
            aoeRadius,
            target
        );
        
        cooldown = 1.0f / fireRate;
    }
}

void ArtilleryTower::draw(sf::RenderWindow& window) {
    if (sprite) {
        window.draw(*sprite);
    } else {
        window.draw(shape);
    }
}