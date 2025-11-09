#include "tower.hpp"

Tower::Tower(sf::Vector2f pos, float r, float d, float speed)
    : position(pos), range(r), damage(d), attackSpeed(speed), lastAttackTime(0.0f)
{
}

// Getters
sf::Vector2f Tower::getPosition() const {
    return position;
}

float Tower::getRange() const {
    return range;
}

float Tower::getDamage() const {
    return damage;
}
