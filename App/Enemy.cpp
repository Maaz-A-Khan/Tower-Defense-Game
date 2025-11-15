#include "enemy.hpp"
#include <cmath>
#include <limits>

// Base Enemy


Enemy::Enemy(const std::vector<Node*>& path, float speed, int health)
    : path(path), baseSpeed(speed), health(health), maxHealth(health),
      currentNodeIndex(0), reachedGoal(false), 
      currentDirection(Direction::East), currentFrame(0),
      animationTimer(0.f), frameTime(0.15f)
{
    if (!path.empty()) {
        position = sf::Vector2f(path[0]->x * 48.f + 24.f, path[0]->y * 48.f + 24.f);
    }

    shape.setRadius(12.f);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin({12.f, 12.f});
    shape.setPosition(position);
}

void Enemy::updateDirection(const sf::Vector2f& movement) {
    Direction previousDirection = currentDirection;
    
    // Determine direction based on movement vector
    if (std::abs(movement.x) > std::abs(movement.y)) {
        // Horizontal movement dominant
        currentDirection = (movement.x > 0) ? Direction::East : Direction::West;
    } else {
        // Vertical movement dominant
        currentDirection = (movement.y > 0) ? Direction::South : Direction::North;
    }
    
    // If direction changed, immediately update sprite texture
    if (previousDirection != currentDirection) {
        currentFrame = 0;  // Reset to first frame of new direction
        animationTimer = 0.f;  // Reset animation timer
        refreshSpriteTexture();  // Immediately show correct texture
    }
}

void Enemy::updateAnimation(float deltaTime) {
    if (directionTextures[currentDirection].empty()) return;
    
    animationTimer += deltaTime;
    
    if (animationTimer >= frameTime) {
        animationTimer = 0.f;
        currentFrame = (currentFrame + 1) % directionTextures[currentDirection].size();
        
        // Update sprite texture
        if (sprite && directionTextures[currentDirection].size() > 0) {
            sprite->setTexture(*directionTextures[currentDirection][currentFrame]);
        }
    }
}

void Enemy::update(float deltaTime) {
    if (reachedGoal || path.empty()) return;

    Node* target = path[currentNodeIndex];
    sf::Vector2f targetPos(target->x * 48.f + 24.f, target->y * 48.f + 24.f);

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
    
    // Update direction based on movement
    updateDirection(dir);
    
    float effectiveSpeed = getCurrentSpeed(target);
    sf::Vector2f movement = dir * effectiveSpeed * deltaTime;
    position += movement;
    shape.setPosition(position);
    
    // Update animation
    updateAnimation(deltaTime);
    
    if (sprite) {
        sprite->setPosition(position);
    }
}

void Enemy::setDirectionalTextures(Direction dir, sf::Texture& frame1, sf::Texture& frame2) {
    directionTextures[dir] = {&frame1, &frame2};
    
    // Initialize sprite with current direction's first frame if not already set
    if (!sprite.has_value()) {
        sprite.emplace(*directionTextures[dir][0]);
        
        sf::Vector2u texSize = frame1.getSize();
        sprite->setOrigin({texSize.x / 2.f, texSize.y / 2.f});
        sprite->setPosition(position);
        
        float scaleX = 48.f / texSize.x;
        float scaleY = 48.f / texSize.y;
        sprite->setScale({scaleX, scaleY});
    }
    
    // If this is the current direction, update sprite texture immediately
    if (dir == currentDirection && sprite.has_value()) {
        sprite->setTexture(*directionTextures[dir][0]);
        
        // Flip sprite horizontally if facing West
        sf::Vector2u texSize = frame1.getSize();
        float scaleX = 48.f / texSize.x;
        float scaleY = 48.f / texSize.y;
        if (dir == Direction::West) {
            sprite->setScale({-scaleX, scaleY});  // Negative X scale flips horizontally
        } else {
            sprite->setScale({scaleX, scaleY});
        }
    }
}

void Enemy::refreshSpriteTexture() {
    // Update sprite to show the correct texture for current direction
    if (sprite.has_value() && !directionTextures[currentDirection].empty()) {
        sprite->setTexture(*directionTextures[currentDirection][currentFrame]);
        
        // Apply horizontal flip for West direction
        if (!directionTextures[currentDirection].empty()) {
            sf::Vector2u texSize = directionTextures[currentDirection][0]->getSize();
            float scaleX = 48.f / texSize.x;
            float scaleY = 48.f / texSize.y;
            
            if (currentDirection == Direction::West) {
                sprite->setScale({-scaleX, scaleY});  // Flip horizontally for West
            } else {
                sprite->setScale({scaleX, scaleY});   // Normal scale for other directions
            }
        }
    }
}

float Enemy::getCurrentSpeed(Node* node) const {
    // Grid uses slowMultiplier >= 1 to represent slowdown; higher means slower.
    // So actual speed should be divided by slowMultiplier.
    return baseSpeed / std::max(1.0f, node->slowMultiplier);
}

void Enemy::draw(sf::RenderWindow& window) {
    if (sprite) {
        window.draw(*sprite);
    } else {
        window.draw(shape);
    }
    drawHealthBar(window);
}

void Enemy::drawHealthBar(sf::RenderWindow& window) const {
    // Health bar dimensions
    float barWidth = 30.f;
    float barHeight = 4.f;
    float barOffsetY = -20.f;  // Above the enemy
    
    // Background (red - damaged portion)
    sf::RectangleShape bgBar({barWidth, barHeight});
    bgBar.setPosition({position.x - barWidth / 2.f, position.y + barOffsetY});
    bgBar.setFillColor(sf::Color(60, 60, 60));  // Dark gray background
    window.draw(bgBar);
    
    // Foreground (green - current health)
    float healthPercent = static_cast<float>(health) / static_cast<float>(maxHealth);
    sf::RectangleShape healthBar({barWidth * healthPercent, barHeight});
    healthBar.setPosition({position.x - barWidth / 2.f, position.y + barOffsetY});
    
    // Color based on health percentage
    if (healthPercent > 0.6f) {
        healthBar.setFillColor(sf::Color::Green);
    } else if (healthPercent > 0.3f) {
        healthBar.setFillColor(sf::Color::Yellow);
    } else {
        healthBar.setFillColor(sf::Color::Red);
    }
    
    window.draw(healthBar);
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
        sf::Vector2f nodePos(path[i]->x * 48.f + 24.f, path[i]->y * 48.f + 24.f);
        sf::Vector2f diff = position - nodePos;
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        if (dist < minDist) {
            minDist = dist;
            closestIdx = i;
        }
    }

    currentNodeIndex = closestIdx;
    
    // Update direction based on new path to ensure sprite shows correct direction
    if (currentNodeIndex < static_cast<int>(path.size()) - 1) {
        Node* nextNode = path[currentNodeIndex + 1];
        sf::Vector2f nextPos(nextNode->x * 48.f + 24.f, nextNode->y * 48.f + 24.f);
        sf::Vector2f direction = nextPos - position;
        
        // Normalize and update direction
        float len = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (len > 0.01f) {
            direction /= len;
            updateDirection(direction);  // This triggers refreshSpriteTexture if direction changes
        }
    }
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
    : Enemy(path, 90.f, 120), shield(80), maxShield(80) {
    shape.setFillColor(sf::Color(0, 100, 255));
}

void ShieldEnemy::draw(sf::RenderWindow& window) {
    if (sprite) {
        window.draw(*sprite);
    } else {
        window.draw(shape);
    }
    
    drawHealthBar(window);
    
    if (maxShield > 0) {
        float barWidth = 30.f;
        float barHeight = 4.f;
        float shieldOffsetY = -25.f;
        
        sf::RectangleShape bgBar({barWidth, barHeight});
        bgBar.setPosition({position.x - barWidth / 2.f, position.y + shieldOffsetY});
        bgBar.setFillColor(sf::Color(40, 40, 40));
        window.draw(bgBar);
        
        float shieldPercent = static_cast<float>(shield) / static_cast<float>(maxShield);
        sf::RectangleShape shieldBar({barWidth * shieldPercent, barHeight});
        shieldBar.setPosition({position.x - barWidth / 2.f, position.y + shieldOffsetY});
        shieldBar.setFillColor(sf::Color::Cyan);
        window.draw(shieldBar);
    }
}

void ShieldEnemy::takeDamage(int dmg) {
    if (shield > 0) {
        shield -= dmg;
        if (shield < 0) {
            health += shield;
            shield = 0;
        }
    } else {
        Enemy::takeDamage(dmg);
    }
}
