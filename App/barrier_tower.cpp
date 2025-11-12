#include "barrier_tower.hpp"

BarrierTower::BarrierTower(sf::Vector2f pos)
    : Tower(pos, 0.f, 0.f, 50, true, TowerType::Barrier), durability(200)
{
    shape.setSize(sf::Vector2f(40.f, 40.f));
    shape.setFillColor(sf::Color(139, 69, 19)); // Brown color
    shape.setOrigin({shape.getSize().x / 2.f, shape.getSize().y / 2.f});
    shape.setPosition(position);
}

void BarrierTower::update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) {
    // Barrier towers are passive - they block enemy paths but don't attack
    // Could add degradation over time or self-repair logic here
    
    // Optional: Reduce durability over time or when enemies are nearby
    // durability -= deltaTime * degradationRate;
}

void BarrierTower::takeDamage(int damage) {
    durability -= damage;
    if (durability < 0) durability = 0;
    
    // Visual feedback based on durability
    float healthPercent = static_cast<float>(durability) / 200.f;
    if (healthPercent > 0.6f) {
        shape.setFillColor(sf::Color(139, 69, 19)); // Brown
    } else if (healthPercent > 0.3f) {
        shape.setFillColor(sf::Color(160, 82, 45)); // Sandy brown
    } else {
        shape.setFillColor(sf::Color(205, 133, 63)); // Peru (damaged)
    }
}

bool BarrierTower::isDestroyed() const {
    return durability <= 0;
}

void BarrierTower::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
