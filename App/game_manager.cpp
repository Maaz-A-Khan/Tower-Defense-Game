#include "game_manager.hpp"
#include <algorithm>
#include <iostream>

GameManager::GameManager()
    : window(sf::VideoMode({GRID_WIDTH * CELL_SIZE + UI_PANEL_WIDTH,
                           GRID_HEIGHT * CELL_SIZE}),
             "Tower Defense"),
      grid(GRID_WIDTH, GRID_HEIGHT),
      pathfinder(&grid),
      enemyManager(&grid, &pathfinder),
      towerManager(&grid, &pathfinder, &enemyManager) {

    // Load UI font FIRST
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Warning: Could not load font!" << std::endl;
    }

    // NOW initialize Text objects with loaded font
    moneyText.emplace(font);
    livesText.emplace(font);
    waveText.emplace(font);

    // Set start/end points for pathfinding
    grid.setStartEnd({0, GRID_HEIGHT / 2}, {GRID_WIDTH - 1, GRID_HEIGHT / 2});

    // UI setup
    moneyText->setCharacterSize(20);
    moneyText->setFillColor(sf::Color::Yellow);
    moneyText->setPosition({10.f, 10.f});

    livesText->setCharacterSize(20);
    livesText->setFillColor(sf::Color::Red);
    livesText->setPosition({10.f, 40.f});

    waveText->setCharacterSize(20);
    waveText->setFillColor(sf::Color::Cyan);
    waveText->setPosition({10.f, 70.f});

    currentState = GameState::PLAYING;
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

    if (enemyManager.allEnemiesDefeated()) {
        waveCompleteTimer += dt;
        if (waveCompleteTimer > 3.0f) {
            startNextWave();
            waveCompleteTimer = 0.0f;
        }
    }

    // Update UI text
    moneyText->setString("Money: " + std::to_string(playerMoney));
    livesText->setString("Lives: " + std::to_string(playerLives));
    waveText->setString("Wave: " + std::to_string(currentWave));
}

void GameManager::render() {
    window.clear(sf::Color(50, 50, 50));

    grid.draw(window);
    towerManager.draw(window);
    enemyManager.draw(window);

    drawUI();
    drawTowerPreview();

    window.display();
}

// === Logic Helpers ===
void GameManager::startNextWave() {
    currentWave++;
    int enemyCount = 5 + (currentWave * 3);
    float spawnInterval = std::max(0.5f, 2.0f - (currentWave * 0.1f));
    enemyManager.spawnWave(enemyCount, spawnInterval);
    playerMoney += 100;
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

// === UI ===
void GameManager::drawUI() const {
    // Need to cast away const because SFML 3.0.2 draw is non-const
    auto& nonConstWindow = const_cast<sf::RenderWindow&>(window);
    nonConstWindow.draw(*moneyText);
    nonConstWindow.draw(*livesText);
    nonConstWindow.draw(*waveText);
}

void GameManager::drawTowerPreview() const {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2i gridPos = screenToGrid(mousePos);
    sf::RectangleShape preview({CELL_SIZE - 2.f, CELL_SIZE - 2.f});
    preview.setPosition({gridPos.x * CELL_SIZE + 1.f, gridPos.y * CELL_SIZE + 1.f});
    preview.setFillColor(sf::Color(0, 255, 0, 100));
    auto& nonConstWindow = const_cast<sf::RenderWindow&>(window);
    nonConstWindow.draw(preview);
}

// === State Handling ===
void GameManager::changeState(GameState newState) {
    currentState = newState;
}
