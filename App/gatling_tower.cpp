#include "gatling_tower.hpp"
#include "projectile_manager.hpp"
#include "enemy.hpp"
#include <cmath>

GatlingTower::GatlingTower(sf::Vector2f pos)
    : Tower(pos, 120.f, 1.5f, 100, false, TowerType::Gatling),
      damage(15.f),
      bulletSpeed(400.f),
      projectileManager(nullptr)
{
    shape.setRadius(20.f);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin({shape.getRadius(), shape.getRadius()});
    shape.setPosition(position);
}

void GatlingTower::update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) {
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
            0.f,      // aoeRadius = 0
            nullptr   // target = nullptr (fire-and-forget, not homing)
        );

        cooldown = 1.0f / fireRate;
    }
}

void GatlingTower::draw(sf::RenderWindow& window) {
    window.draw(shape);
}