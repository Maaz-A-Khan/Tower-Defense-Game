#pragma once
#include "tower.hpp"
#include <SFML/Graphics.hpp>

class GatlingTower : public Tower {
private:
    sf::CircleShape shape;
    float damage;

public:
    GatlingTower(sf::Vector2f pos);

    // Implement pure virtual functions from Tower
    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) override;
    void draw(sf::RenderWindow& window) override;
};
