#include "Tower.hpp"
#include <cmath>

Tower::Tower(sf::Vector2f pos, float range, float fireRate, int cost, bool isBlocking, TowerType type)
    : position(pos),
      range(range),
      fireRate(fireRate),
      cooldown(0.0f),
      cost(cost),
      isBlocking(isBlocking),
      type(type),
      currentRotation(-90.f)
{}

bool Tower::canAttack(Enemy* enemy) {
    if (!enemy) return false;
    sf::Vector2f enemyPos = enemy->getPosition();
    float dx = position.x - enemyPos.x;
    float dy = position.y - enemyPos.y;
    float distanceSq = dx * dx + dy * dy;
    return distanceSq <= (range * range);
}

void Tower::setBaseTexture(sf::Texture& texture) {
    baseSprite.emplace(texture);
    
    sf::Vector2u texSize = texture.getSize();
    baseSprite->setOrigin({texSize.x / 2.f, texSize.y / 2.f});
    baseSprite->setPosition(position);
    
    float scaleX = 48.f / texSize.x;
    float scaleY = 48.f / texSize.y;
    baseSprite->setScale({scaleX, scaleY});
    // Base does NOT rotate
}

void Tower::setShooterTexture(sf::Texture& texture) {
    shooterSprite.emplace(texture);
    
    sf::Vector2u texSize = texture.getSize();
    shooterSprite->setOrigin({texSize.x / 2.f, texSize.y / 2.f});
    shooterSprite->setPosition(position);
    
    float scaleX = 48.f / texSize.x;
    float scaleY = 48.f / texSize.y;
    shooterSprite->setScale({scaleX, scaleY});
    
    // Apply initial rotation to shooter only
    shooterSprite->setRotation(sf::degrees(currentRotation));
}

void Tower::rotateToward(const sf::Vector2f& targetPos) {
    if (!shooterSprite.has_value()) return;
    
    float dx = targetPos.x - position.x;
    float dy = targetPos.y - position.y;
    float angle = std::atan2(dy, dx) * 180.f / 3.14159f;
    
    currentRotation = angle + 90.f;
    shooterSprite->setRotation(sf::degrees(currentRotation));
}
