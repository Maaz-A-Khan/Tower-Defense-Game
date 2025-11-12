#pragma once
#include "tower.hpp"
#include "grid.hpp"

class FrostTower : public Tower {
private:
    float slowMultiplier;      // e.g. 0.7 means 30% slowdown
    int aoeRangeCells;         // radius in grid cells
    float pathCostIncrease;    // optional modifier for pathfinding
    sf::RectangleShape aoeVisual;
    Grid* grid;                // pointer for applying frost effects

public:
    FrostTower(sf::Vector2f pos, Grid* grid, float range, float fireRate,
               int cost, float slowMultiplier, int aoeRangeCells);

    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) override;
    void draw(sf::RenderWindow& window) override;

    void applyFrostEffects();   // triggers Grid::applyFrostEffects
};
