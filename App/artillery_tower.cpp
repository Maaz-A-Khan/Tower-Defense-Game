#include "artillery_tower.hpp"
#include <cmath>

ArtilleryTower::ArtilleryTower(sf::Vector2f pos)
    : Tower(pos, 200.f, 0.5f, 250, false, TowerType::Artillery), 
      damage(50.f), splashRadius(60.f)
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

    // Find target enemy
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

    // Splash attack if target found
    if (target) {
        sf::Vector2f explosionCenter = target->getPosition();
        
        // Apply damage to all enemies within splash radius
        for (auto& enemy : enemies) {
            if (enemy->isDead()) continue;
            
            sf::Vector2f enemyPos = enemy->getPosition();
            float dx = explosionCenter.x - enemyPos.x;
            float dy = explosionCenter.y - enemyPos.y;
            float distance = std::sqrt(dx * dx + dy * dy);
            
            if (distance <= splashRadius) {
                // Damage falloff with distance
                float damageMultiplier = 1.0f - (distance / splashRadius);
                int finalDamage = static_cast<int>(damage * damageMultiplier);
                enemy->takeDamage(finalDamage);
            }
        }
        
        cooldown = 1.0f / fireRate;
    }
}

void ArtilleryTower::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
