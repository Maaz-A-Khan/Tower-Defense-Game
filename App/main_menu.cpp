#include "main_menu.hpp"
#include "asset_manager.hpp"

MainMenu::MainMenu(sf::RenderWindow& win, AssetManager* assets)
    : window(win), assetManager(assets), startButtonHovered(false), exitButtonHovered(false) {
    
    // Background
    background.setSize({1248.f, 720.f});
    background.setFillColor(sf::Color(20, 20, 30));
    
    // Button dimensions (fallback)
    float buttonWidth = 300.f;
    float buttonHeight = 80.f;
    float centerX = 1248.f / 2.f - buttonWidth / 2.f;
    
    // Start button fallback
    startButton.setSize({buttonWidth, buttonHeight});
    startButton.setPosition({centerX, 300.f});
    startButton.setFillColor(sf::Color(50, 150, 50));
    startButton.setOutlineColor(sf::Color::White);
    startButton.setOutlineThickness(3.f);
    
    // Exit button fallback
    exitButton.setSize({buttonWidth, buttonHeight});
    exitButton.setPosition({centerX, 420.f});
    exitButton.setFillColor(sf::Color(150, 50, 50));
    exitButton.setOutlineColor(sf::Color::White);
    exitButton.setOutlineThickness(3.f);
    
    // Text setup
    if (assetManager && assetManager->hasFont("main_font")) {
        sf::Font& font = assetManager->getFont("main_font");
        
        titleText.emplace(font);
        titleText->setString("TOWER DEFENSE");
        titleText->setCharacterSize(60);
        titleText->setFillColor(sf::Color::White);
        titleText->setPosition({centerX - 50.f, 150.f});
        
        startText.emplace(font);
        startText->setString("START");
        startText->setCharacterSize(40);
        startText->setFillColor(sf::Color::White);
        startText->setPosition({centerX + 90.f, 315.f});
        
        exitText.emplace(font);
        exitText->setString("EXIT");
        exitText->setCharacterSize(40);
        exitText->setFillColor(sf::Color::White);
        exitText->setPosition({centerX + 105.f, 435.f});
    }
}

void MainMenu::setBackgroundTexture(sf::Texture& texture) {
    backgroundSprite.emplace(texture);
    backgroundSprite->setPosition({0.f, 0.f});
    
    sf::Vector2u texSize = texture.getSize();
    float scaleX = 1248.f / texSize.x;
    float scaleY = 720.f / texSize.y;
    backgroundSprite->setScale({scaleX, scaleY});
}

void MainMenu::setStartButtonTexture(sf::Texture& texture) {
    startButtonSprite.emplace(texture);
    
    float buttonWidth = 300.f;
    float buttonHeight = 80.f;
    float centerX = 1248.f / 2.f - buttonWidth / 2.f;
    
    startButtonSprite->setPosition({centerX, 300.f});
    
    sf::Vector2u texSize = texture.getSize();
    float scaleX = buttonWidth / texSize.x;
    float scaleY = buttonHeight / texSize.y;
    startButtonSprite->setScale({scaleX, scaleY});
    
    // Update button bounds for collision
    startButton.setSize({buttonWidth, buttonHeight});
    startButton.setPosition({centerX, 300.f});
}

void MainMenu::setExitButtonTexture(sf::Texture& texture) {
    exitButtonSprite.emplace(texture);
    
    float buttonWidth = 300.f;
    float buttonHeight = 80.f;
    float centerX = 1248.f / 2.f - buttonWidth / 2.f;
    
    exitButtonSprite->setPosition({centerX, 420.f});
    
    sf::Vector2u texSize = texture.getSize();
    float scaleX = buttonWidth / texSize.x;
    float scaleY = buttonHeight / texSize.y;
    exitButtonSprite->setScale({scaleX, scaleY});
    
    // Update button bounds for collision
    exitButton.setSize({buttonWidth, buttonHeight});
    exitButton.setPosition({centerX, 420.f});
}

bool MainMenu::handleInput() {
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            return false;
        }
        
        if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left) {
                if (isMouseOverButton(startButton)) {
                    return true;  // Start game
                }
                if (isMouseOverButton(exitButton)) {
                    window.close();
                    return false;
                }
            }
        }
    }
    return false;
}

void MainMenu::update() {
    startButtonHovered = isMouseOverButton(startButton);
    exitButtonHovered = isMouseOverButton(exitButton);
    
    // Button hover effects (for sprites, apply color tint)
    if (startButtonSprite) {
        if (startButtonHovered) {
            startButtonSprite->setColor(sf::Color(200, 255, 200)); // Brighter
        } else {
            startButtonSprite->setColor(sf::Color::White);
        }
    } else {
        if (startButtonHovered) {
            startButton.setFillColor(sf::Color(70, 200, 70));
        } else {
            startButton.setFillColor(sf::Color(50, 150, 50));
        }
    }
    
    if (exitButtonSprite) {
        if (exitButtonHovered) {
            exitButtonSprite->setColor(sf::Color(255, 200, 200)); // Brighter
        } else {
            exitButtonSprite->setColor(sf::Color::White);
        }
    } else {
        if (exitButtonHovered) {
            exitButton.setFillColor(sf::Color(200, 70, 70));
        } else {
            exitButton.setFillColor(sf::Color(150, 50, 50));
        }
    }
}

void MainMenu::render() {
    window.clear();
    
    // Draw background
    if (backgroundSprite) {
        window.draw(*backgroundSprite);
    } else {
        window.draw(background);
    }
    
    // Draw buttons (use sprites if available, otherwise fallback to rectangles)
    if (startButtonSprite) {
        window.draw(*startButtonSprite);
    } else {
        window.draw(startButton);
    }
    
    if (exitButtonSprite) {
        window.draw(*exitButtonSprite);
    } else {
        window.draw(exitButton);
    }
    
    // Draw text
    if (titleText) window.draw(*titleText);
    if (startText) window.draw(*startText);
    if (exitText) window.draw(*exitText);
    
    window.display();
}

bool MainMenu::isMouseOverButton(const sf::RectangleShape& button) const {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::FloatRect buttonBounds = button.getGlobalBounds();
    return buttonBounds.contains(static_cast<sf::Vector2f>(mousePos));
}

bool MainMenu::isMouseOverSprite(const sf::Sprite& sprite) const {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    return spriteBounds.contains(static_cast<sf::Vector2f>(mousePos));
}