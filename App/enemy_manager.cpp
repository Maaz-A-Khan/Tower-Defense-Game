#include "enemy_manager.hpp"
#include <cstdlib>

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
                                 [](const std::unique_ptr<Enemy> &e)
                                 {
                                     return e->isDead() || e->hasReachedGoal();
                                 }),
                  enemies.end());
}

void EnemyManager::recalculatePaths()
{
    Node *start = grid->getNode(grid->getStart().x, grid->getStart().y);
    Node *end = grid->getNode(grid->getEnd().x, grid->getEnd().y);
    if (!start || !end)
        return;

    cachedPath = pathfinder->findPath(start, end);

    for (auto &enemy : enemies)
        enemy->setPath(cachedPath);
}

bool EnemyManager::allEnemiesDefeated() const
{
    return enemies.empty() && enemiesToSpawn == 0;
}

const std::vector<std::unique_ptr<Enemy>> &EnemyManager::getEnemies() const
{
    return enemies;
}
