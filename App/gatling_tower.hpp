#pragma once
#include "tower.hpp"
#include <SFML/Graphics.hpp>

class GatlingTower : public Tower {
private:
    sf::CircleShape shape;  // Visual representation of the tower
public:
    GatlingTower(sf::Vector2f pos);

    // Implement pure virtual functions
    void attack() override;
    void upgrade() override;
    void draw(sf::RenderWindow &window) override;
};
