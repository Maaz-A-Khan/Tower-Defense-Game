#include "game_manager.hpp"
#include <algorithm>
#include <iostream>

GameManager::GameManager()
    : window(sf::VideoMode({GRID_WIDTH * CELL_SIZE + UI_PANEL_WIDTH,
                           GRID_HEIGHT * CELL_SIZE}),
             "Tower Defense"),
      assetManager(),
      uiManager(nullptr),
      grid(GRID_WIDTH, GRID_HEIGHT),
      pathfinder(&grid),
      enemyManager(&grid, &pathfinder, &assetManager),
      towerManager(&grid, &pathfinder, &enemyManager) {

    // Load all assets
    assetManager.loadAllAssets();

    // Create UI Manager with the loaded font
    uiManager = new UIManager(assetManager.getFont("main_font"));

    // Set grid texture if available
    if (assetManager.hasTexture("grid_tile")) {
        grid.setTexture(assetManager.getTexture("grid_tile"));
    }

    // Set start/end points for pathfinding
    grid.setStartEnd({0, GRID_HEIGHT / 2}, {GRID_WIDTH - 1, GRID_HEIGHT / 2});
    
    // Recalculate paths now that start/end are set
    enemyManager.recalculatePaths();

    currentState = GameState::PLAYING;
}

GameManager::~GameManager() {
    delete uiManager;
}

void GameManager::run() {
    while (window.isOpen()) {
        handleInput();

        float deltaTime = clock.restart().asSeconds();
        accumulator += deltaTime;

        while (accumulator >= FIXED_TIMESTEP) {
            if (currentState == GameState::PLAYING)
                update(FIXED_TIMESTEP);
            accumulator -= FIXED_TIMESTEP;
        }

        render();
    }
}

void GameManager::handleInput() {
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::Num1) selectedTower = TowerType::Barrier;
            if (keyPressed->code == sf::Keyboard::Key::Num2) selectedTower = TowerType::Gatling;
            if (keyPressed->code == sf::Keyboard::Key::Num3) selectedTower = TowerType::Frost;
            if (keyPressed->code == sf::Keyboard::Key::Num4) selectedTower = TowerType::Artillery;
            if (keyPressed->code == sf::Keyboard::Key::Space) startNextWave();
            if (keyPressed->code == sf::Keyboard::Key::Escape) togglePause();
        }

        if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2i gridPos = screenToGrid(mousePos);
                tryPlaceTower(selectedTower, gridPos);
            }
        }
    }
}

void GameManager::update(float dt) {
    grid.resetFrostEffects();       // Prevent stacking frost
    
    const auto& enemies = enemyManager.getEnemies();
    towerManager.update(dt, const_cast<std::vector<std::unique_ptr<Enemy>>&>(enemies));
    
    enemyManager.update(dt);        // Move enemies, handle deaths

    checkLivesLost();
    checkWinLoss();

    // Only auto-start next wave if we've actually started playing (currentWave > 0)
    if (currentWave > 0 && enemyManager.allEnemiesDefeated()) {
        waveCompleteTimer += dt;
        if (waveCompleteTimer > 3.0f) {
            startNextWave();
            waveCompleteTimer = 0.0f;
        }
    }

    // Update UI with current game state
    int selectedTowerCost = TOWER_COSTS[static_cast<int>(selectedTower)];
    uiManager->update(playerMoney, playerLives, currentWave, selectedTower, selectedTowerCost);
}

void GameManager::render() {
    window.clear(sf::Color(50, 50, 50));

    grid.draw(window);
    towerManager.draw(window);
    enemyManager.draw(window);

    // Draw UI
    uiManager->draw(window);
    uiManager->drawInstructions(window);
    
    // Draw tower preview
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2i gridPos = screenToGrid(mousePos);
    bool canPlace = !towerManager.isOccupied(gridPos) && canAfford(selectedTower);
    uiManager->drawTowerPreview(window, gridPos, canPlace);

    window.display();
}

// === Logic Helpers ===
void GameManager::startNextWave() {
    // Don't start a new wave if enemies are still active or being spawned
    if (!enemyManager.allEnemiesDefeated()) {
        return;
    }
    
    currentWave++;
    int enemyCount = 5 + (currentWave * 3);
    float spawnInterval = std::max(0.5f, 2.0f - (currentWave * 0.1f));
    enemyManager.spawnWave(enemyCount, spawnInterval);
    playerMoney += 100;
    waveCompleteTimer = 0.0f;  // Reset timer when manually starting wave
}

void GameManager::checkLivesLost() {
    int lost = enemyManager.getReachedGoalCount();
    if (lost > 0) {
        playerLives -= lost;
        if (playerLives < 0) playerLives = 0;
    }
}

void GameManager::checkWinLoss() {
    if (playerLives <= 0) changeState(GameState::GAME_OVER);
}

void GameManager::togglePause() {
    paused = !paused;
    changeState(paused ? GameState::PAUSED : GameState::PLAYING);
}

// === Placement Logic ===
bool GameManager::tryPlaceTower(TowerType type, sf::Vector2i gridPos) {
    int cost = TOWER_COSTS[static_cast<int>(type)];
    if (playerMoney < cost) return false;

    if (towerManager.placeTower(type, gridPos)) {
        playerMoney -= cost;
        return true;
    }
    return false;
}

// === Coordinate Conversion ===
sf::Vector2i GameManager::screenToGrid(sf::Vector2i mousePos) const {
    return sf::Vector2i(mousePos.x / CELL_SIZE, mousePos.y / CELL_SIZE);
}

sf::Vector2f GameManager::gridToWorld(sf::Vector2i gridPos) const {
    return sf::Vector2f(gridPos.x * CELL_SIZE + CELL_SIZE / 2.0f,
                        gridPos.y * CELL_SIZE + CELL_SIZE / 2.0f);
}

sf::Vector2i GameManager::worldToGrid(sf::Vector2f worldPos) const {
    return sf::Vector2i(worldPos.x / CELL_SIZE, worldPos.y / CELL_SIZE);
}

// === Game state handling ===
void GameManager::changeState(GameState newState) {
    currentState = newState;
}

bool GameManager::canAfford(TowerType type) const {
    int cost = TOWER_COSTS[static_cast<int>(type)];
    return playerMoney >= cost;
}

int GameManager::getTowerCost(TowerType type) const {
    return TOWER_COSTS[static_cast<int>(type)];
}
