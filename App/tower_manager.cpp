#include "tower_manager.hpp"
#include "asset_manager.hpp"
#include "grid.hpp"
#include "a_star_path_finder.hpp"
#include "enemy_manager.hpp"
#include "enemy.hpp"
#include "node.hpp"

#include "gatling_tower.hpp"
#include "frost_tower.hpp"
#include "artillery_tower.hpp"
#include "barrier_tower.hpp"

TowerManager::TowerManager(Grid* grid, AStarPathfinder* pathfinder, EnemyManager* enemyManager, AssetManager* assets)
    : grid(grid), pathfinder(pathfinder), enemyManager(enemyManager), assetManager(assets), projectileManager(assets) {
}

sf::Vector2f TowerManager::gridToWorld(sf::Vector2i gridPos) {
    return sf::Vector2f(gridPos.x * 48.f + 24.f, gridPos.y * 48.f + 24.f);
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
    // Check if this cell already has a tower
    if (occupiedCells.count({gridPos.x, gridPos.y})) {
        return true;
    }
    
    Node* node = grid->getNode(gridPos.x, gridPos.y);
    if (!node) {
        return true; 
    }
    
    // Check if the node is blocked by something else (like a barrier)
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

    // Determine if this tower type is blocking
    bool isBlockingTower = (type == TowerType::Barrier || type == TowerType::Gatling || type == TowerType::Artillery);
    
    // Only block the grid cell if the tower is blocking
    if (isBlockingTower) {
        grid->setObstacle(gridPos.x, gridPos.y, true);

        // Validate that a path still exists from start to end
        std::vector<Node*> path = pathfinder->findPath(grid->getNode(grid->startCell.x, grid->startCell.y), 
                                                    grid->getNode(grid->endCell.x, grid->endCell.y));

        if (path.empty()) {
            grid->setObstacle(gridPos.x, gridPos.y, false);
            return false;
        }
    }
    
    // Mark cell as occupied by a tower
    occupiedCells.insert({gridPos.x, gridPos.y});

    sf::Vector2f worldPos = gridToWorld(gridPos);

    switch (type) {
        case TowerType::Gatling: {
            auto tower = std::make_unique<GatlingTower>(worldPos);
            tower->setProjectileManager(&projectileManager);
            if (assetManager && assetManager->hasTexture("gatling_tower")) {
                tower->setTexture(assetManager->getTexture("gatling_tower"));
            }
            towers.push_back(std::move(tower));
            break;
        }
        case TowerType::Artillery: {
            auto tower = std::make_unique<ArtilleryTower>(worldPos);
            tower->setProjectileManager(&projectileManager);
            if (assetManager && assetManager->hasTexture("artillery_tower")) {
                tower->setTexture(assetManager->getTexture("artillery_tower"));
            }
            towers.push_back(std::move(tower));
            break;
        }
        case TowerType::Frost: {
            auto frostTower = std::make_unique<FrostTower>(worldPos, grid, 100.f, 0.f, 150, 0.5f, 2);
            if (assetManager && assetManager->hasTexture("frost_tower")) {
                frostTower->setTexture(assetManager->getTexture("frost_tower"));
            }
            frostTower->applyFrostEffects(); 
            towers.push_back(std::move(frostTower));
            break;
        }
        case TowerType::Barrier: {
            auto tower = std::make_unique<BarrierTower>(worldPos);
            if (assetManager && assetManager->hasTexture("barrier_tower")) {
                tower->setTexture(assetManager->getTexture("barrier_tower"));
            }
            towers.push_back(std::move(tower));
            break;
        }
    }

    if (enemyManager) {
        enemyManager->recalculatePaths();
    }
    
    return true;
}