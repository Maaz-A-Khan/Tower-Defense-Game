#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

class AssetManager;

class MainMenu {
private:
    sf::RenderWindow& window;
    AssetManager* assetManager;
    
    std::optional<sf::Sprite> backgroundSprite;
    sf::RectangleShape background;
    
    std::optional<sf::Sprite> startButtonSprite;
    std::optional<sf::Sprite> exitButtonSprite;
    sf::RectangleShape startButton;  // Fallback
    sf::RectangleShape exitButton;   // Fallback
    
    std::optional<sf::Text> titleText;
    std::optional<sf::Text> startText;
    std::optional<sf::Text> exitText;
    
    bool startButtonHovered;
    bool exitButtonHovered;
    
public:
    MainMenu(sf::RenderWindow& win, AssetManager* assets);
    
    void setBackgroundTexture(sf::Texture& texture);
    void setStartButtonTexture(sf::Texture& texture);
    void setExitButtonTexture(sf::Texture& texture);
    
    bool handleInput();
    void update();
    void render();
    
private:
    bool isMouseOverButton(const sf::RectangleShape& button) const;
    bool isMouseOverSprite(const sf::Sprite& sprite) const;
};