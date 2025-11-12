#include "enemy.hpp"
#include <cmath>
#include <limits>

// Base Enemy


Enemy::Enemy(const std::vector<Node*>& path, float speed, int health)
    : path(path), baseSpeed(speed), health(health),
      currentNodeIndex(0), reachedGoal(false)
{
    if (!path.empty()) {
        position = sf::Vector2f(path[0]->x * 32.f + 16.f, path[0]->y * 32.f + 16.f);
    }

    shape.setRadius(12.f);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin({12.f, 12.f});
    shape.setPosition(position);
}

void Enemy::update(float deltaTime) {
    if (reachedGoal || path.empty()) return;

    Node* target = path[currentNodeIndex];
    sf::Vector2f targetPos(target->x * 32.f + 16.f, target->y * 32.f + 16.f);

    sf::Vector2f dir = targetPos - position;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (len < 1.f) {
        currentNodeIndex++;
        if (currentNodeIndex >= static_cast<int>(path.size())) {
            reachedGoal = true;
            return;
        }
        return;
    }

    dir /= len;
    float effectiveSpeed = getCurrentSpeed(target);
    position += dir * effectiveSpeed * deltaTime;
    shape.setPosition(position);
}

float Enemy::getCurrentSpeed(Node* node) const {
    // Grid uses slowMultiplier >= 1 to represent slowdown; higher means slower.
    // So actual speed should be divided by slowMultiplier.
    return baseSpeed / std::max(1.0f, node->slowMultiplier);
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Enemy::takeDamage(int dmg) {
    health -= dmg;
    if (health < 0) health = 0;
}

void Enemy::setPath(const std::vector<Node*>& newPath) {
    path = newPath;
    reachedGoal = false;

    if (path.empty()) {
        currentNodeIndex = 0;
        return;
    }

    // Find closest node on new path to current position
    int closestIdx = 0;
    float minDist = std::numeric_limits<float>::max();

    for (int i = 0; i < static_cast<int>(path.size()); ++i) {
        sf::Vector2f nodePos(path[i]->x * 32.f + 16.f, path[i]->y * 32.f + 16.f);
        sf::Vector2f diff = position - nodePos;
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        if (dist < minDist) {
            minDist = dist;
            closestIdx = i;
        }
    }

    currentNodeIndex = closestIdx;
}

bool Enemy::isDead() const { return health <= 0; }
bool Enemy::hasReachedGoal() const { return reachedGoal; }
const sf::Vector2f& Enemy::getPosition() const { return position; }

// Subclasses

NormalEnemy::NormalEnemy(const std::vector<Node*>& path)
    : Enemy(path, 100.f, 100) {
    shape.setFillColor(sf::Color(200, 50, 50));
}

FastEnemy::FastEnemy(const std::vector<Node*>& path)
    : Enemy(path, 160.f, 70) {
    shape.setFillColor(sf::Color(255, 150, 0));
}

TankEnemy::TankEnemy(const std::vector<Node*>& path)
    : Enemy(path, 60.f, 250) {
    shape.setFillColor(sf::Color(80, 80, 80));
}

ShieldEnemy::ShieldEnemy(const std::vector<Node*>& path)
    : Enemy(path, 90.f, 120), shield(80) {
    shape.setFillColor(sf::Color(0, 100, 255));
}

void ShieldEnemy::takeDamage(int dmg) {
    if (shield > 0) {
        shield -= dmg;
        if (shield < 0) {
            health += shield; // leftover damage applied to health
            shield = 0;
        }
    } else {
        Enemy::takeDamage(dmg);
    }
}
