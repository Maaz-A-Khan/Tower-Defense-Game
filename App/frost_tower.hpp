#pragma once
#include "tower.hpp"
#include <SFML/Graphics.hpp>

class FrostTower : public Tower {
private:
    sf::CircleShape shape;
    float slowAmount; // How much it slows enemies
public:
    FrostTower(sf::Vector2f pos);

    void attack() override;
    void upgrade() override;
    void draw(sf::RenderWindow &window) override;
};
