#include "frost_tower.hpp"
#include <iostream>

FrostTower::FrostTower(sf::Vector2f pos)
    : Tower(pos, 120.f, 5.f, 0.8f), slowAmount(0.5f) // range=120, damage=5, attackSpeed=0.8
{
    shape.setRadius(20.f);
    shape.setFillColor(sf::Color::Blue);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(position);
}

void FrostTower::attack() {
    std::cout << "Frost Tower attacking! Slowing enemies by: " << slowAmount << std::endl;
}

void FrostTower::upgrade() {
    damage += 2.f;
    range += 20.f;
    slowAmount += 0.1f;
    std::cout << "Frost Tower upgraded! Damage: " << damage << ", Range: " << range << ", Slow: " << slowAmount << std::endl;
}

void FrostTower::draw(sf::RenderWindow &window) {
    window.draw(shape);
}
