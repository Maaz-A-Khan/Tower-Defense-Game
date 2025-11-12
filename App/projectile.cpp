#include "projectile.hpp"
#include "enemy.hpp"
#include <cmath>

const float Projectile::radius = 5.f;

Projectile::Projectile(sf::Vector2f start, sf::Vector2f dir, float spd, int dmg, float aoe, Enemy* target)
    : position(start),
      direction(dir),
      speed(spd),
      damage(dmg),
      active(true),
      aoeRadius(aoe),
      target(target) 
{
    shape.setRadius(radius);
    shape.setFillColor(aoeRadius > 0 ? sf::Color::Yellow : sf::Color::Red);
    shape.setOrigin({radius, radius});
    shape.setPosition(position);
}

void Projectile::update(float deltaTime) {
    if (!active) return;

    if (target && !target->isDead()) {
        sf::Vector2f dirToTarget = target->getPosition() - position;
        float len = std::hypot(dirToTarget.x, dirToTarget.y);
        if (len != 0) {
            direction = dirToTarget / len;
        }
    }
    
    position += direction * speed * deltaTime;
    shape.setPosition(position);
    
    if (position.x < -50 || position.x > 1000 || position.y < -50 || position.y > 800) {
        active = false;
    }
}

void Projectile::draw(sf::RenderWindow& window) {
    if (active) {
        window.draw(shape);
    }
}

bool Projectile::checkCollision(Enemy* enemy) {
    if (!active || !enemy || enemy->isDead()) return false;
    
    float dist = std::hypot(enemy->getPosition().x - position.x, enemy->getPosition().y - position.y);
    float hitRange = radius + 12.f;
    
    if (dist <= hitRange) {
        active = false;
        return true;
    }
    return false;
}