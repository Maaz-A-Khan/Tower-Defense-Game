#include "gatling_tower.hpp"
#include <iostream>

GatlingTower::GatlingTower(sf::Vector2f pos)
    : Tower(pos, 100.f, 10.f, 1.0f) // Example: range=100, damage=10, attackSpeed=1
{
    // Initialize the tower shape
    shape.setRadius(20.f);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(position);
}

// Simple attack function (can be expanded later)
void GatlingTower::attack() {
    // This is a placeholder
    std::cout << "Gatling Tower attacking with damage: " << damage << std::endl;
}

// Upgrade example (increase damage and range)
void GatlingTower::upgrade() {
    damage += 5.f;
    range += 20.f;
    std::cout << "Gatling Tower upgraded! Damage: " << damage << ", Range: " << range << std::endl;
}

// Draw the tower on window
void GatlingTower::draw(sf::RenderWindow &window) {
    window.draw(shape);
}
