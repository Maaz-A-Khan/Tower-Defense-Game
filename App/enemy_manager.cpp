#include "enemy_manager.hpp"
#include <cstdlib>
#include <algorithm>



EnemyManager::EnemyManager(Grid *grid, AStarPathfinder *pathfinder)
    : grid(grid), pathfinder(pathfinder),
      spawnInterval(2.0f), enemiesToSpawn(0), enemiesSpawned(0)
{
    // Compute initial global path
    Node *start = grid->getNode(grid->getStart().x, grid->getStart().y);
    Node *end = grid->getNode(grid->getEnd().x, grid->getEnd().y);
    if (start && end)
        cachedPath = pathfinder->findPath(start, end);
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
    if (cachedPath.empty())
        return;

    int type = std::rand() % 4;
    std::unique_ptr<Enemy> e;

    switch (type)
    {
    case 0:
        e = std::make_unique<NormalEnemy>(cachedPath);
        break;
    case 1:
        e = std::make_unique<FastEnemy>(cachedPath);
        break;
    case 2:
        e = std::make_unique<TankEnemy>(cachedPath);
        break;
    case 3:
        e = std::make_unique<ShieldEnemy>(cachedPath);
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
        int gridX = static_cast<int>(enemyPos.x / 32.0f);
        int gridY = static_cast<int>(enemyPos.y / 32.0f);
        
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
