#pragma once
#include "tower.hpp"
#include <SFML/Graphics.hpp>

class ArtilleryTower : public Tower {
private:
    sf::CircleShape shape;
    float splashRadius; // Explosive range
public:
    ArtilleryTower(sf::Vector2f pos);

    void attack() override;
    void upgrade() override;
    void draw(sf::RenderWindow &window) override;
};
