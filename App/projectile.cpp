
#include "projectile.hpp"
#include <cmath>

Projectile::Projectile(sf::Vector2f start, sf::Vector2f dir, float spd, int dmg)
    : position(start), direction(dir), speed(spd), damage(dmg), active(true) {}

void Projectile::update(float deltaTime) {
    if (!active) return;
    position += direction * speed * deltaTime;
    
    // Deactivate if projectile goes off screen (simple bounds check)
    if (position.x < -50 || position.x > 1000 || position.y < -50 || position.y > 800) {
        active = false;
    }
}

void Projectile::draw(sf::RenderWindow& window) {
    if (!active) return;
    sf::CircleShape shape(radius);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin({radius, radius});
    shape.setPosition(position);
    window.draw(shape);
}

bool Projectile::checkCollision(Enemy& enemy) {
    if (!active) return false;
    float dist = std::hypot(enemy.getPosition().x - position.x, enemy.getPosition().y - position.y);
    float hitRange = radius + 12.f; // Assume enemy radius is 12.f (from Enemy class)
    if (dist <= hitRange) {
        enemy.takeDamage(damage);
        active = false;
        return true;
    }
    return false;
}
