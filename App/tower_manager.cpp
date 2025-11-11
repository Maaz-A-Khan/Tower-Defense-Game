#include "tower_manager.hpp"
#include "grid.hpp"
#include "a_star_path_finder.hpp"
#include "enemy_manager.hpp"
#include "enemy.hpp"
#include "node.hpp"

#include "gatling_tower.hpp"
#include "frost_tower.hpp"
#include "artillery_tower.hpp"
#include "barrier_tower.hpp"

TowerManager::TowerManager(Grid* grid, AStarPathfinder* pathfinder, EnemyManager* enemyManager)
    : grid(grid), pathfinder(pathfinder), enemyManager(enemyManager) {
}

sf::Vector2f TowerManager::gridToWorld(sf::Vector2i gridPos) {
    return sf::Vector2f(gridPos.x * 32.f + 16.f, gridPos.y * 32.f + 16.f);
}

void TowerManager::update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) {
    for (auto& tower : towers) {
        tower->update(deltaTime, enemies);
    }
    projectileManager.update(deltaTime, enemies);
}

void TowerManager::draw(sf::RenderWindow& window) {
    for (auto& tower : towers) {
        tower->draw(window);
    }
    projectileManager.draw(window);
}

bool TowerManager::isOccupied(sf::Vector2i gridPos) {
    Node* node = grid->getNode(gridPos.x, gridPos.y);
    if (!node) {
        return true; 
    }
    return !node->walkable;
}

bool TowerManager::placeTower(TowerType type, sf::Vector2i gridPos) {
    if (isOccupied(gridPos)) {
        return false;
    }

    Node* node = grid->getNode(gridPos.x, gridPos.y);
    if (!node) {
        return false; 
    }

    grid->setObstacle(gridPos.x, gridPos.y, true);

    std::vector<Node*> path = pathfinder->findPath(grid->getNode(grid->startCell.x, grid->startCell.y), 
                                                grid->getNode(grid->endCell.x, grid->endCell.y));

    if (path.empty()) {
        grid->setObstacle(gridPos.x, gridPos.y, false);
        return false;
    }

    sf::Vector2f worldPos = gridToWorld(gridPos);

    switch (type) {
        case TowerType::Gatling:
            towers.push_back(std::make_unique<GatlingTower>(worldPos));
            break;
        case TowerType::Artillery:
            towers.push_back(std::make_unique<ArtilleryTower>(worldPos));
            break;
        case TowerType::Frost: {
            auto frostTower = std::make_unique<FrostTower>(worldPos, grid, 100.f, 0.f, 150, 0.5f, 2);
            frostTower->applyFrostEffects(); 
            towers.push_back(std::move(frostTower));
            break;
        }
        case TowerType::Barrier:
            towers.push_back(std::make_unique<BarrierTower>(worldPos));
            break;
    }

    if (enemyManager) {
        enemyManager->recalculatePaths();
    }
    
    return true;
}