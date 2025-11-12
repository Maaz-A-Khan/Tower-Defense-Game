#pragma once
#include "tower.hpp"
#include <SFML/Graphics.hpp>

class ArtilleryTower : public Tower {
private:
    sf::CircleShape shape;
    float damage;
    float splashRadius;

public:
    ArtilleryTower(sf::Vector2f pos);

    // Implement pure virtual functions from Tower
    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) override;
    void draw(sf::RenderWindow& window) override;
};
