#include "projectile_manager.hpp"
#include "asset_manager.hpp"
#include "projectile.hpp"
#include "enemy.hpp"
#include <algorithm>
#include <cmath>

ProjectileManager::ProjectileManager(AssetManager* assets) : assetManager(assets) {}

void ProjectileManager::spawnProjectile(sf::Vector2f start, sf::Vector2f dir, float speed, int dmg, float aoeRadius, Enemy* target, ProjectileType type) {
    if (target == nullptr) { 
        float len = std::hypot(dir.x, dir.y);
        if (len != 0) dir /= len;
    }
    auto projectile = std::make_unique<Projectile>(start, dir, speed, dmg, aoeRadius, target);
    
    projectile->projectileType = type;

    // Apply texture if available
    if (assetManager) {
        if (type == ProjectileType::Gatling && assetManager->hasTexture("gatling_bullet")) {
            projectile->setTexture(assetManager->getTexture("gatling_bullet"));
        } else if (type == ProjectileType::Artillery && assetManager->hasTexture("artillery_bullet")) {
            projectile->setTexture(assetManager->getTexture("artillery_bullet"));
        } else if (assetManager->hasTexture("projectile")) {
            // Fallback texture
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
                // Create explosion for Artillery projectiles BEFORE applying damage
                if (projectile->projectileType == ProjectileType::Artillery) {
                    explosions.push_back(Explosion(enemy->getPosition()));
                }
                if (projectile->aoeRadius > 0) {
                    applyAoEDamage(enemy->getPosition(), projectile->aoeRadius, projectile->damage, enemies);
                } else {
                    enemy->takeDamage(projectile->damage);
                }
                break;
            }
        }
    }

    const float frameTime = 0.1f; // 0.1 seconds per frame
    for (auto& explosion : explosions) {
        explosion.elapsed += deltaTime;
        explosion.currentFrame = static_cast<int>(explosion.elapsed / frameTime);
        
        if (explosion.currentFrame >= 5) { // 5 frames total
            explosion.active = false;
        }
    }

    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
                       [](const std::unique_ptr<Projectile>& p) { return !p->active; }),
        projectiles.end());
    explosions.erase(
        std::remove_if(explosions.begin(), explosions.end(),
                       [](const Explosion& e) { return !e.active; }),
        explosions.end());
}

void ProjectileManager::draw(sf::RenderWindow& window) {
    for (auto& projectile : projectiles) {
        projectile->draw(window);
    }
    
    // Draw explosions
    if (assetManager) {
        for (const auto& explosion : explosions) {
            std::string textureName = "explosion" + std::to_string(explosion.currentFrame + 1);
            
            if (assetManager->hasTexture(textureName)) {
                sf::Sprite explosionSprite(assetManager->getTexture(textureName));
                
                sf::Vector2u texSize = assetManager->getTexture(textureName).getSize();
                explosionSprite.setOrigin({texSize.x / 2.f, texSize.y / 2.f});  // Use braces for Vector2f
                explosionSprite.setPosition(explosion.position);
                
                // Scale to appropriate size
                float scale = 60.f / texSize.x; // 60 pixels wide
                explosionSprite.setScale({scale, scale});  // Use braces for Vector2f
                
                window.draw(explosionSprite);
            }
        }
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