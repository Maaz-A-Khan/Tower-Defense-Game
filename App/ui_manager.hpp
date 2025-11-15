#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <memory>
#include "tower.hpp"
#include "asset_manager.hpp"

class UIManager {
private:
    // UI Text elements
    std::optional<sf::Text> moneyText;
    std::optional<sf::Text> livesText;
    std::optional<sf::Text> waveText;
    std::optional<sf::Text> selectedTowerText;
    std::optional<sf::Text> instructionsText;
    
    // Panel background
    std::unique_ptr<sf::Sprite> panelBackground;
    
    // Tower preview
    sf::RectangleShape towerPreview;
    
    // Tower cost lookup
    int getTowerCost(TowerType type) const;
    std::string getTowerName(TowerType type) const;

public:
    UIManager(sf::Font& font, AssetManager& assets);

    // Update UI with current game state
    void update(int money, int lives, int wave, TowerType selectedTower, int selectedTowerCost);
    
    // Draw UI elements
    void draw(sf::RenderWindow& window) const;
    void drawTowerPreview(sf::RenderWindow& window, sf::Vector2i gridPos, bool canPlace) const;
    void drawInstructions(sf::RenderWindow& window) const;
};