#include "gatling_tower.hpp"
#include <cmath>

GatlingTower::GatlingTower(sf::Vector2f pos)
    : Tower(pos, 120.f, 1.5f, 100, false, TowerType::Gatling), damage(15.f)
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

    // Find nearest enemy in range
    Enemy* target = nullptr;
    float minDistance = range * range; // squared distance for efficiency
    
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

    // Attack if target found
    if (target) {
        target->takeDamage(static_cast<int>(damage));
        cooldown = 1.0f / fireRate; // Reset cooldown based on fire rate
    }
}

void GatlingTower::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
