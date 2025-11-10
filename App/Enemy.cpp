#include "enemy.hpp"
#include <cmath>

Enemy::Enemy(sf::Vector2f startPos)
    : position(startPos), speed(120.f), health(100), currentNodeIndex(0), reachedGoal(false)
{
    slowTimer=0.f;
    originalSpeed=speed;
}

void Enemy::setPath(const std::vector<Node*>& newPath) {
    path = newPath;
    currentNodeIndex = 0;
}

void Enemy::update(float deltaTime) {

    // handle slow effect timer
    if (slowTimer > 0) {
        slowTimer -= deltaTime;
        if (slowTimer <= 0) {
            speed = originalSpeed; // restore normal speed
        }
    }

    if (currentNodeIndex < path.size()) {
        Node* target = path[currentNodeIndex];
        sf::Vector2f targetPos(target->x * 32.f, target->y * 32.f);

        sf::Vector2f dir = targetPos - position;
        float len = sqrt(dir.x * dir.x + dir.y * dir.y);

        if (len < 1.f) {
            currentNodeIndex++;
            if (currentNodeIndex >= path.size()) reachedGoal = true;
        } else {
            dir /= len;
            position += dir * speed * deltaTime;
        }
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    sf::CircleShape e(10.f);
    e.setPosition({position.x, position.y});
    window.draw(e);
}

bool Enemy::isDead() const {
    return health <= 0;
}

void Enemy::takeDamage(int dmg) {
    health -= dmg;
}

void Enemy::applySlow(float slowMultiplier) {
    speed=originalSpeed*slowMultiplier;
    slowTimer=3.0f;
}
