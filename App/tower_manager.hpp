#pragma once
#include <vector>
#include <memory>
#include "tower.hpp"
#include "gatling_tower.hpp"
#include "frost_tower.hpp"
#include "barrier_tower.hpp"
#include "artillery_tower.hpp"
#include <SFML/Graphics.hpp>

class TowerManager {
private:
    std::vector<std::shared_ptr<Tower>> towers; // Store all towers

public:
    TowerManager() = default;

    // Add towers
    void addGatlingTower(sf::Vector2f pos);
    void addFrostTower(sf::Vector2f pos);
    void addBarrierTower(sf::Vector2f pos);
    void addArtilleryTower(sf::Vector2f pos);

    // Update all towers (attack)
    void update();

    // Draw all towers
    void draw(sf::RenderWindow &window);
};
