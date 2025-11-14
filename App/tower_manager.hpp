#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "tower.hpp"
#include "projectile_manager.hpp"

class Grid;
class AStarPathfinder;
class EnemyManager;
class Enemy;
class AssetManager;

class TowerManager {
private:
    std::vector<std::unique_ptr<Tower>> towers;
    Grid* grid;
    AStarPathfinder* pathfinder;
    EnemyManager* enemyManager;
    ProjectileManager projectileManager;
    AssetManager* assetManager;
    sf::Vector2f gridToWorld(sf::Vector2i gridPos);

public:
    TowerManager(Grid* grid, AStarPathfinder* pathfinder, EnemyManager* enemyManager, AssetManager* assets = nullptr);

    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies);
    void draw(sf::RenderWindow& window);

    bool isOccupied(sf::Vector2i gridPos);
    bool placeTower(TowerType type, sf::Vector2i gridPos);
};