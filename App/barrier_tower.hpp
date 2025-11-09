#pragma once
#include "tower.hpp"
#include <SFML/Graphics.hpp>

class BarrierTower : public Tower {
private:
    sf::RectangleShape shape;
public:
    BarrierTower(sf::Vector2f pos);

    void attack() override;
    void upgrade() override;
    void draw(sf::RenderWindow &window) override;
};
