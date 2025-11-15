#include "enemy_manager.hpp"
#include "asset_manager.hpp"
#include <cstdlib>
#include <algorithm>
#include <iostream>


EnemyManager::EnemyManager(Grid *grid, AStarPathfinder *pathfinder, AssetManager *assets)
    : grid(grid), pathfinder(pathfinder), assetManager(assets),
      spawnInterval(2.0f), enemiesToSpawn(0), enemiesSpawned(0)
{
    // Compute initial global path
    Node *start = grid->getNode(grid->getStart().x, grid->getStart().y);
    Node *end = grid->getNode(grid->getEnd().x, grid->getEnd().y);
    if (start && end) {
        cachedPath = pathfinder->findPath(start, end);
        std::cout << "Initial path calculated. Path size: " << cachedPath.size() << std::endl;
    } else {
        std::cout << "Error: Start or End node is null!" << std::endl;
    }
}

void EnemyManager::update(float deltaTime)
{
    if (enemiesToSpawn > 0 && spawnClock.getElapsedTime().asSeconds() >= spawnInterval)
    {
        spawnEnemy();
        spawnClock.restart();
    }

    for (auto &enemy : enemies)
        enemy->update(deltaTime);

    clearDeadEnemies();
}

void EnemyManager::draw(sf::RenderWindow &window)
{
    for (auto &enemy : enemies)
        enemy->draw(window);
}

void EnemyManager::spawnWave(int count, float interval)
{
    enemiesToSpawn = count;
    spawnInterval = interval;
    enemiesSpawned = 0;
    spawnClock.restart();
}

void EnemyManager::spawnEnemy()
{
    // Don't spawn if we don't have a valid path
    if (cachedPath.empty())
    {
        std::cout << "Warning: Cannot spawn enemy - no valid path! Recalculating..." << std::endl;
        
        // Try to recalculate path
        Node *start = grid->getNode(grid->getStart().x, grid->getStart().y);
        Node *end = grid->getNode(grid->getEnd().x, grid->getEnd().y);
        if (start && end) {
            cachedPath = pathfinder->findPath(start, end);
            std::cout << "Recalculated path. Path size: " << cachedPath.size() << std::endl;
        }
        
        // If still no path, we can't spawn
        if (cachedPath.empty()) {
            std::cout << "Still no path available. Cannot spawn enemies." << std::endl;
            return;  // Don't decrement enemiesToSpawn - try again next frame
        }
    }

    int type = std::rand() % 4;
    std::unique_ptr<Enemy> e;

    switch (type)
    {
    case 0:
        e = std::make_unique<NormalEnemy>(cachedPath);
        if (assetManager) {
            e->setDirectionalTextures(Direction::North, 
                assetManager->getTexture("normal_north_1"), 
                assetManager->getTexture("normal_north_2"));
            e->setDirectionalTextures(Direction::South, 
                assetManager->getTexture("normal_south_1"), 
                assetManager->getTexture("normal_south_2"));
            e->setDirectionalTextures(Direction::East, 
                assetManager->getTexture("normal_east_1"), 
                assetManager->getTexture("normal_east_2"));
            e->setDirectionalTextures(Direction::West, 
                assetManager->getTexture("normal_west_1"), 
                assetManager->getTexture("normal_west_2"));
        }
        break;
    case 1:
        e = std::make_unique<FastEnemy>(cachedPath);
        if (assetManager) {
            e->setDirectionalTextures(Direction::North, 
                assetManager->getTexture("fast_north_1"), 
                assetManager->getTexture("fast_north_2"));
            e->setDirectionalTextures(Direction::South, 
                assetManager->getTexture("fast_south_1"), 
                assetManager->getTexture("fast_south_2"));
            e->setDirectionalTextures(Direction::East, 
                assetManager->getTexture("fast_east_1"), 
                assetManager->getTexture("fast_east_2"));
            e->setDirectionalTextures(Direction::West, 
                assetManager->getTexture("fast_west_1"), 
                assetManager->getTexture("fast_west_2"));
        }
        break;
    case 2:
        e = std::make_unique<TankEnemy>(cachedPath);
        if (assetManager) {
            e->setDirectionalTextures(Direction::North, 
                assetManager->getTexture("tank_north_1"), 
                assetManager->getTexture("tank_north_2"));
            e->setDirectionalTextures(Direction::South, 
                assetManager->getTexture("tank_south_1"), 
                assetManager->getTexture("tank_south_2"));
            e->setDirectionalTextures(Direction::East, 
                assetManager->getTexture("tank_east_1"), 
                assetManager->getTexture("tank_east_2"));
            e->setDirectionalTextures(Direction::West, 
                assetManager->getTexture("tank_west_1"), 
                assetManager->getTexture("tank_west_2"));
        }
        break;
    case 3:
        e = std::make_unique<ShieldEnemy>(cachedPath);
        if (assetManager) {
            e->setDirectionalTextures(Direction::North, 
                assetManager->getTexture("normal_north_1"), 
                assetManager->getTexture("normal_north_2"));
            e->setDirectionalTextures(Direction::South, 
                assetManager->getTexture("normal_south_1"), 
                assetManager->getTexture("normal_south_2"));
            e->setDirectionalTextures(Direction::East, 
                assetManager->getTexture("normal_east_1"), 
                assetManager->getTexture("normal_east_2"));
            e->setDirectionalTextures(Direction::West, 
                assetManager->getTexture("normal_west_1"), 
                assetManager->getTexture("normal_west_2"));
        }
        break;
    }

    enemies.push_back(std::move(e));
    enemiesToSpawn--;
    enemiesSpawned++;
}

void EnemyManager::clearDeadEnemies()
{
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                 [this](const std::unique_ptr<Enemy> &e)
                                 {
                                     if (e->hasReachedGoal()) {
                                         enemiesReachedGoal++;  // Count before removing
                                         return true;
                                     }
                                     return e->isDead();
                                 }),
                  enemies.end());
}

void EnemyManager::recalculatePaths()
{
    Node *end = grid->getNode(grid->getEnd().x, grid->getEnd().y);
    if (!end)
        return;

    // Recalculate the cached path from start to end (for new spawns)
    Node *start = grid->getNode(grid->getStart().x, grid->getStart().y);
    if (start)
        cachedPath = pathfinder->findPath(start, end);

    // For each existing enemy, recalculate path from THEIR current position
    for (auto &enemy : enemies)
    {
        // Get enemy's current grid position
        sf::Vector2f enemyPos = enemy->getPosition();
        int gridX = static_cast<int>(enemyPos.x / 48.0f);
        int gridY = static_cast<int>(enemyPos.y / 48.0f);
        
        Node *enemyNode = grid->getNode(gridX, gridY);
        if (!enemyNode)
            continue;

        // Find path from enemy's current position to the end
        std::vector<Node*> newPath = pathfinder->findPath(enemyNode, end);
        
        if (!newPath.empty())
            enemy->setPath(newPath);
    }
}

bool EnemyManager::allEnemiesDefeated() const
{
    return enemies.empty() && enemiesToSpawn == 0;
}

const std::vector<std::unique_ptr<Enemy>> &EnemyManager::getEnemies() const
{
    return enemies;
}

int EnemyManager::getReachedGoalCount()
{
    int count = enemiesReachedGoal;
    enemiesReachedGoal = 0;  // Reset after reading
    return count;
}
