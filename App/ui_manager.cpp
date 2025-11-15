#include "ui_manager.hpp"

UIManager::UIManager(sf::Font& font) {
    // Initialize text objects with the font
    moneyText.emplace(font);
    livesText.emplace(font);
    waveText.emplace(font);
    selectedTowerText.emplace(font);
    instructionsText.emplace(font);

    // UI elements positioned on the right side (grid is 960px wide with 48px cells)
    float uiX = 960.f;  // Grid ends at 960px (20 cells * 48px)
    float uiWidth = 288.f;  // UI panel width
    float uiHeight = 720.f;  // Full height
    
    uiPanelBackground.setSize({uiWidth, uiHeight});
    uiPanelBackground.setPosition({uiX, 0.f});
    uiPanelBackground.setFillColor(sf::Color(40, 40, 40, 230));  // Dark semi-transparent
    uiPanelBackground.setOutlineColor(sf::Color(100, 100, 100));
    uiPanelBackground.setOutlineThickness(2.f);

    float textX = uiX + 60.f;  // 20px padding inside panel

    // Setup money text
    moneyText->setCharacterSize(20);
    moneyText->setFillColor(sf::Color::Yellow);
    moneyText->setPosition({textX + 30.f, 78.f});

    // Setup lives text
    livesText->setCharacterSize(20);
    livesText->setFillColor(sf::Color::Red);
    livesText->setPosition({textX, 146.f});

    // Setup wave text
    waveText->setCharacterSize(20);
    waveText->setFillColor(sf::Color::Cyan);
    waveText->setPosition({textX, 210.f});

    // Setup selected tower text
    selectedTowerText->setCharacterSize(18);
    selectedTowerText->setFillColor(sf::Color::White);
    selectedTowerText->setPosition({textX, 280.f});

    // Setup instructions text
    instructionsText->setCharacterSize(16);
    instructionsText->setFillColor(sf::Color(200, 200, 200));
    instructionsText->setPosition({textX, 370.f});
    instructionsText->setString("Controls:\n1-4: Select Tower\nSpace: Start Wave\nEsc: Pause");

    // Setup tower preview shape
    towerPreview.setSize({48.f - 2.f, 48.f - 2.f});
}

void UIManager::setUIPanelTexture(sf::Texture& texture) {
    uiPanelSprite.emplace(texture);
    
    // Position and scale the UI panel texture
    uiPanelSprite->setPosition({960.f, 0.f});
    
    sf::Vector2u texSize = texture.getSize();
    float scaleX = 288.f / texSize.x;  // Scale to panel width
    float scaleY = 720.f / texSize.y;  // Scale to panel height
    uiPanelSprite->setScale({scaleX, scaleY});
}

void UIManager::update(int money, int lives, int wave, TowerType selectedTower, int selectedTowerCost) {
    // Update text strings with current game state
    moneyText->setString("Money: $" + std::to_string(money));
    livesText->setString("Lives: " + std::to_string(lives));
    waveText->setString("Wave: " + std::to_string(wave));
    
    std::string towerInfo = "Selected: " + getTowerName(selectedTower) + 
                           " ($" + std::to_string(selectedTowerCost) + ")";
    selectedTowerText->setString(towerInfo);
}

void UIManager::draw(sf::RenderWindow& window) const {
    
    auto& nonConstWindow = const_cast<sf::RenderWindow&>(window);
    
    // Draw UI panel background first
    if (uiPanelSprite) {
        nonConstWindow.draw(*uiPanelSprite);
    } else {
        nonConstWindow.draw(uiPanelBackground);
    }
    
    nonConstWindow.draw(*moneyText);
    nonConstWindow.draw(*livesText);
    nonConstWindow.draw(*waveText);
    nonConstWindow.draw(*selectedTowerText);
}

void UIManager::drawTowerPreview(sf::RenderWindow& window, sf::Vector2i gridPos, bool canPlace) const {
    auto& nonConstWindow = const_cast<sf::RenderWindow&>(window);
    
    // Create a temporary preview (we can't modify member in const function)
    sf::RectangleShape preview = towerPreview;
    preview.setPosition({gridPos.x * 48.f + 1.f, gridPos.y * 48.f + 1.f});
    
    // Green if can place, red if cannot
    if (canPlace) {
        preview.setFillColor(sf::Color(0, 255, 0, 100));  // Green transparent
    } else {
        preview.setFillColor(sf::Color(255, 0, 0, 100));  // Red transparent
    }
    
    nonConstWindow.draw(preview);
}

void UIManager::drawInstructions(sf::RenderWindow& window) const {
    auto& nonConstWindow = const_cast<sf::RenderWindow&>(window);
    nonConstWindow.draw(*instructionsText);
}

int UIManager::getTowerCost(TowerType type) const {
    const int TOWER_COSTS[4] = {50, 100, 150, 250};
    return TOWER_COSTS[static_cast<int>(type)];
}

std::string UIManager::getTowerName(TowerType type) const {
    switch (type) {
        case TowerType::Barrier:   return "Barrier";
        case TowerType::Gatling:   return "Gatling";
        case TowerType::Frost:     return "Frost";
        case TowerType::Artillery: return "Artillery";
        default:                   return "Unknown";
    }
}