#include "projectile_manager.hpp"
#include "asset_manager.hpp"
#include "projectile.hpp"
#include "enemy.hpp"
#include <algorithm>
#include <cmath>

ProjectileManager::ProjectileManager(AssetManager* assets) : assetManager(assets) {}

void ProjectileManager::spawnProjectile(sf::Vector2f start, sf::Vector2f dir, float speed, int dmg, float aoeRadius, Enemy* target) {
    if (target == nullptr) { 
        float len = std::hypot(dir.x, dir.y);
        if (len != 0) dir /= len;
    }
    auto projectile = std::make_unique<Projectile>(start, dir, speed, dmg, aoeRadius, target);
    
    // Apply texture if available
    if (assetManager) {
        if (aoeRadius > 0 && assetManager->hasTexture("aoe_projectile")) {
            projectile->setTexture(assetManager->getTexture("aoe_projectile"));
        } else if (assetManager->hasTexture("projectile")) {
            projectile->setTexture(assetManager->getTexture("projectile"));
        }
    }
    
    projectiles.push_back(std::move(projectile));
}

void ProjectileManager::update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) {
    for (auto& projectile : projectiles) {
        if (!projectile->active) continue;

        projectile->update(deltaTime);

        for (auto& enemy : enemies) {
            if (enemy->isDead()) continue;

            if (projectile->checkCollision(enemy.get())) {
                if (projectile->aoeRadius > 0) {
                    applyAoEDamage(enemy->getPosition(), projectile->aoeRadius, projectile->damage, enemies);
                } else {
                    enemy->takeDamage(projectile->damage);
                }
                break;
            }
        }
    }

    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
                       [](const std::unique_ptr<Projectile>& p) { return !p->active; }),
        projectiles.end());
}

void ProjectileManager::draw(sf::RenderWindow& window) {
    for (auto& projectile : projectiles) {
        projectile->draw(window);
    }
}

void ProjectileManager::applyAoEDamage(sf::Vector2f center, float radius, int damage, std::vector<std::unique_ptr<Enemy>>& enemies) {
    float radiusSq = radius * radius;
    for (auto& enemy : enemies) {
        if (enemy->isDead()) continue;

        sf::Vector2f enemyPos = enemy->getPosition();
        float dx = center.x - enemyPos.x;
        float dy = center.y - enemyPos.y;
        float distanceSq = dx * dx + dy * dy;

        if (distanceSq <= radiusSq) {
            float distance = std::sqrt(distanceSq);
            float damageMultiplier = 1.0f - (distance / radius);
            int finalDamage = static_cast<int>(damage * damageMultiplier);
            enemy->takeDamage(finalDamage);
        }
    }
}