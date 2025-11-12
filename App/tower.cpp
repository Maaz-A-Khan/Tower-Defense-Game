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
