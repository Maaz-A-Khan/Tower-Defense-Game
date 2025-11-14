#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "enemy.hpp"
#include "a_star_path_finder.hpp"
#include "grid.hpp"

class AssetManager;  // Forward declaration

class EnemyManager
{
private:
    Grid *grid;
    AStarPathfinder *pathfinder;
    AssetManager *assetManager;  // Add asset manager pointer
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<Node *> cachedPath;

    sf::Clock spawnClock;
    float spawnInterval;
    int enemiesToSpawn;
    int enemiesSpawned;
    
    int enemiesReachedGoal = 0;  // Track enemies that reached goal this frame

public:
    EnemyManager(Grid *grid, AStarPathfinder *pathfinder, AssetManager *assets = nullptr);

    void update(float deltaTime);
    void draw(sf::RenderWindow &window);

    void spawnEnemy(); // Spawns one enemy
    void spawnWave(int count, float interval);
    void clearDeadEnemies();
    void recalculatePaths(); // Repath all active enemies

    bool allEnemiesDefeated() const;
    const std::vector<std::unique_ptr<Enemy>> &getEnemies() const;
    int getReachedGoalCount();  // Get and reset count of enemies that reached goal
};
