#include "asset_manager.hpp"

bool AssetManager::loadTexture(const std::string& key, const std::string& filePath) {
    sf::Texture texture;
    
    if (!texture.loadFromFile(filePath)) {
        std::cerr << "❌ Failed to load texture: " << filePath << std::endl;
        return false;
    }
    
    textures[key] = std::move(texture);
    std::cout << "✅ Loaded texture: " << key << " from " << filePath 
              << " (Size: " << textures[key].getSize().x << "x" << textures[key].getSize().y << ")" << std::endl;
    return true;
}

sf::Texture& AssetManager::getTexture(const std::string& key) {
    if (!hasTexture(key)) {
        std::cerr << "Warning: Texture '" << key << "' not found! Returning first available texture." << std::endl;
        return textures.begin()->second;
    }
    return textures.at(key);
}

bool AssetManager::hasTexture(const std::string& key) const {
    return textures.find(key) != textures.end();
}

bool AssetManager::loadFont(const std::string& key, const std::string& filePath) {
    sf::Font font;
    
    if (!font.openFromFile(filePath)) {
        std::cerr << "Failed to load font: " << filePath << std::endl;
        return false;
    }
    
    fonts[key] = std::move(font);
    std::cout << "Loaded font: " << key << " from " << filePath << std::endl;
    return true;
}

sf::Font& AssetManager::getFont(const std::string& key) {
    if (!hasFont(key)) {
        std::cerr << "Warning: Font '" << key << "' not found! Returning first available font." << std::endl;
        return fonts.begin()->second;
    }
    return fonts.at(key);
}

bool AssetManager::hasFont(const std::string& key) const {
    return fonts.find(key) != fonts.end();
}

void AssetManager::loadAllAssets() {
    std::cout << "\n=== Loading All Assets ===" << std::endl;
    
    // Load fonts
    loadFont("main_font", "C:/Windows/Fonts/arial.ttf");
    
    // Load grid texture
    loadTexture("grid_tile", "../Assets/grid/grid.png.png");
    
    // Load tower textures (if you create them later)
    // loadTexture("gatling_tower", "../Assets/towers/gatling.png");
    loadTexture("frost_tower", "../Assets/towers/frost.png");
    // loadTexture("artillery_tower", "../Assets/towers/artillery.png");
    // loadTexture("barrier_tower", "../Assets/towers/barrier.png");
    
    // Load enemy textures (if you create them later)
    loadTexture("normal_enemy", "../Assets/enemies/normal.png");
    // loadTexture("fast_enemy", "../Assets/enemies/fast.png");
    // loadTexture("tank_enemy", "../Assets/enemies/tank.png");
    // loadTexture("shield_enemy", "../Assets/enemies/shield.png");
    
    std::cout << "=== Asset Loading Complete ===" << std::endl;
    std::cout << "Textures loaded: " << textures.size() << std::endl;
    std::cout << "Fonts loaded: " << fonts.size() << std::endl << std::endl;
}