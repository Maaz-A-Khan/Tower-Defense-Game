#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>

class AssetManager {
private:
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::Font> fonts;

public:
    AssetManager() = default;

    // Texture management
    bool loadTexture(const std::string& key, const std::string& filePath);
    sf::Texture& getTexture(const std::string& key);
    bool hasTexture(const std::string& key) const;

    // Font management
    bool loadFont(const std::string& key, const std::string& filePath);
    sf::Font& getFont(const std::string& key);
    bool hasFont(const std::string& key) const;

    // Utility
    void loadAllAssets();  // Load all game assets at once
};