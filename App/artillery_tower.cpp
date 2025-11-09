#include "artillery_tower.hpp"
#include <iostream>

ArtilleryTower::ArtilleryTower(sf::Vector2f pos)
    : Tower(pos, 150.f, 20.f, 0.5f), splashRadius(50.f) // range=150, damage=20, attackSpeed=0.5
{
    shape.setRadius(25.f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(position);
}

void ArtilleryTower::attack() {
    std::cout << "Artillery Tower attacking! Damage: " << damage << ", Splash radius: " << splashRadius << std::endl;
}

void ArtilleryTower::upgrade() {
    damage += 10.f;
    range += 20.f;
    splashRadius += 10.f;
    std::cout << "Artillery Tower upgraded! Damage: " << damage << ", Range: " << range << ", Splash: " << splashRadius << std::endl;
}

void ArtilleryTower::draw(sf::RenderWindow &window) {
    window.draw(shape);
}
