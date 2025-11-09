#include "barrier_tower.hpp"
#include <iostream>

BarrierTower::BarrierTower(sf::Vector2f pos)
    : Tower(pos, 0.f, 0.f, 0.f) // Barrier does not attack
{
    shape.setSize(sf::Vector2f(40.f, 40.f));
    shape.setFillColor(sf::Color(139,69,19)); // Brown color
    shape.setOrigin(shape.getSize().x / 2, shape.getSize().y / 2);
    shape.setPosition(position);
}

void BarrierTower::attack() {
    // Barrier does not attack
}

void BarrierTower::upgrade() {
    // Maybe increase durability or size
    shape.setSize(shape.getSize() * 1.1f);
    std::cout << "Barrier Tower upgraded! New size: " << shape.getSize().x << std::endl;
}

void BarrierTower::draw(sf::RenderWindow &window) {
    window.draw(shape);
}
