#include "Tower.hpp"
#include <cmath>

Tower::Tower(sf::Vector2f pos, float range, float fireRate, int cost, bool isBlocking, TowerType type)
    : position(pos),
      range(range),
      fireRate(fireRate),
      cooldown(0.0f),
      cost(cost),
      isBlocking(isBlocking),
      type(type)
{}

bool Tower::canAttack(Enemy* enemy) {
    if (!enemy) return false;
    sf::Vector2f enemyPos = enemy->getPosition();
    float dx = position.x - enemyPos.x;
    float dy = position.y - enemyPos.y;
    float distanceSq = dx * dx + dy * dy;
    return distanceSq <= (range * range);
}

void Tower::setTexture(sf::Texture& texture) {
    sprite.emplace(texture);
    
    // Center the sprite origin
    sf::Vector2u texSize = texture.getSize();
    sprite->setOrigin({texSize.x / 2.f, texSize.y / 2.f});
    sprite->setPosition(position);
    
    // Scale sprite to fit cell size (48x48)
    float scaleX = 48.f / texSize.x;
    float scaleY = 48.f / texSize.y;
    sprite->setScale({scaleX, scaleY});
}
