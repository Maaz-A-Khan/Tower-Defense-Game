#include "frost_tower.hpp"
#include <cmath>

FrostTower::FrostTower(sf::Vector2f pos, Grid* grid, float range, float fireRate,
                       int cost, float slowMultiplier, int aoeRangeCells)
    : Tower(pos, range, fireRate, cost, true, TowerType::Frost),
      slowMultiplier(slowMultiplier),
      aoeRangeCells(aoeRangeCells),
      pathCostIncrease(0.0f),
      grid(grid)
{
    aoeVisual.setSize(sf::Vector2f(aoeRangeCells * 2 * 32, aoeRangeCells * 2 * 32)); // assuming 32px cell
    aoeVisual.setOrigin(aoeVisual.getSize() / 2.0f);
    aoeVisual.setPosition(position);
    aoeVisual.setFillColor(sf::Color(0, 150, 255, 60));
}

void FrostTower::update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) {
    // Apply AoE slow effect every frame (since frost towers are passive)
    if (grid) {
        applyFrostEffects();
    }

    // Update cooldown (even though FrostTower doesn't shoot)
    if (cooldown > 0)
        cooldown -= deltaTime;
}

void FrostTower::draw(sf::RenderWindow& window) {
    // Draw AoE visualization
    window.draw(aoeVisual);

    // Optionally, draw tower core (simple circle)
    sf::CircleShape towerBase(16);
    towerBase.setOrigin({16.f, 16.f});
    towerBase.setPosition(position);
    towerBase.setFillColor(sf::Color(100, 180, 255));
    window.draw(towerBase);
}

void FrostTower::applyFrostEffects() {
    if (!grid) return;

    // Convert world position to grid cell indices
    int centerX = static_cast<int>(position.x / 32);
    int centerY = static_cast<int>(position.y / 32);

    grid->applyFrostEffect(centerX, centerY, aoeRangeCells, slowMultiplier);
}
