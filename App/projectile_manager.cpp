// ProjectileManager.cpp
#include "projectile_manager.hpp"
#include <algorithm>
#include <cmath>

void ProjectileManager::spawnProjectile(sf::Vector2f start, sf::Vector2f dir, float speed, int dmg) {
    // Normalize direction vector
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len != 0) dir /= len;
    projectiles.push_back(std::make_unique<Projectile>(start, dir, speed, dmg));
}

void ProjectileManager::update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) {
    for (auto& projectile : projectiles) {
        projectile->update(deltaTime);
        for (auto& enemy : enemies) {
            if (projectile->checkCollision(*enemy)) break;
        }
    }

    // remove inactive projectiles
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
                       [](const std::unique_ptr<Projectile>& p) { return !p->isActive(); }),
        projectiles.end());
}

void ProjectileManager::draw(sf::RenderWindow& window) {
    for (auto& projectile : projectiles)
        projectile->draw(window);
}
