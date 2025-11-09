#include "tower_manager.hpp"

// Add new towers
void TowerManager::addGatlingTower(sf::Vector2f pos) {
    towers.push_back(std::make_shared<GatlingTower>(pos));
}

void TowerManager::addFrostTower(sf::Vector2f pos) {
    towers.push_back(std::make_shared<FrostTower>(pos));
}

void TowerManager::addBarrierTower(sf::Vector2f pos) {
    towers.push_back(std::make_shared<BarrierTower>(pos));
}

void TowerManager::addArtilleryTower(sf::Vector2f pos) {
    towers.push_back(std::make_shared<ArtilleryTower>(pos));
}

// Update all towers
void TowerManager::update() {
    for (auto &tower : towers) {
        tower->attack(); // Call each tower's attack logic
    }
}

// Draw all towers
void TowerManager::draw(sf::RenderWindow &window) {
    for (auto &tower : towers) {
        tower->draw(window);
    }
}
