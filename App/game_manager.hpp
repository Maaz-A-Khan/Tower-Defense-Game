#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "grid.hpp"
#include "a_star_path_finder.hpp"
#include "enemy_manager.hpp"
#include "tower_manager.hpp"
#include "asset_manager.hpp"
#include "ui_manager.hpp"
#include "tower.hpp"

enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    WAVE_END,
    GAME_OVER,
    VICTORY
};

class GameManager {
private:
    // === Core systems ===
    sf::RenderWindow window;
    AssetManager assetManager;
    UIManager* uiManager;  // Pointer because it needs font from AssetManager
    
    Grid grid;
    AStarPathfinder pathfinder;
    EnemyManager enemyManager;
    TowerManager towerManager;

    // === Timing ===
    sf::Clock clock;
    float accumulator = 0.0f;
    const float FIXED_TIMESTEP = 1.0f / 60.0f;

    // === Game state ===
    GameState currentState = GameState::MENU;
    int currentWave = 0;
    float waveCompleteTimer = 0.0f;
    bool paused = false;

    // === Player resources ===
    int playerMoney = 1000;
    int playerLives = 20;
    const int TOWER_COSTS[4] = {50, 100, 150, 250};

    // === Tower selection ===
    TowerType selectedTower = TowerType::Barrier;

    // === Window dimensions ===
    static constexpr int GRID_WIDTH = 20;
    static constexpr int GRID_HEIGHT = 15;
    static constexpr int CELL_SIZE = 32;
    static constexpr int UI_PANEL_WIDTH = 200;

public:
    // === Constructor / lifecycle ===
    GameManager();
    ~GameManager();
    void run();

private:
    // === Core loop ===
    void handleInput();
    void update(float dt);
    void render();

    // === Logic helpers ===
    void startNextWave();
    void checkLivesLost();
    void checkWinLoss();
    void togglePause();

    // === Placement / interaction ===
    bool tryPlaceTower(TowerType type, sf::Vector2i gridPos);
    bool canAfford(TowerType type) const;
    int getTowerCost(TowerType type) const;

    // === Coordinate helpers ===
    sf::Vector2i screenToGrid(sf::Vector2i mousePos) const;
    sf::Vector2f gridToWorld(sf::Vector2i gridPos) const;
    sf::Vector2i worldToGrid(sf::Vector2f worldPos) const;

    // === Game state handling ===
    void changeState(GameState newState);
};
