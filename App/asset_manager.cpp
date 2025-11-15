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
    
    loadTexture("ui_panel", "../Assets/sidebar/sidebar.png");

    // Load grid texture
    loadTexture("grid_tile", "../Assets/grid/grid.png.png");
    loadTexture("start_tile", "../Assets/grid/start-point.png");
    loadTexture("end_tile", "../Assets/grid/end-point.png");
    
    // Load UI textures
    loadTexture("ui_panel", "../Assets/ui/panel_background.png");
    
    // Load tower textures (if you create them later)
    loadTexture("gatling_tower_base", "../Assets/towers/gatling-base.png");
    loadTexture("gatling_tower_shooter", "../Assets/towers/gatling-shooter.png");

    loadTexture("frost_tower", "../Assets/towers/frost-tower.png");
    
    loadTexture("artillery_tower_base", "../Assets/towers/artillery-base.png");
    loadTexture("artillery_tower_shooter", "../Assets/towers/artillery-shooter.png");

    loadTexture("barrier_tower", "../Assets/towers/barrier.png");
    
    // Load projectile textures
    loadTexture("gatling_bullet", "../Assets/bullets/gatling.png");
    loadTexture("artillery_bullet", "../Assets/bullets/artillery.png");

    // Load enemy textures (if you create them later)
    // Enemy textures - directional with 2 frames each
    //normal enemiees
    loadTexture("normal_north_1", "../Assets/enemies/normal_enemy_up_1.png");
    loadTexture("normal_north_2", "../Assets/enemies/normal_enemy_up_2.png");
    loadTexture("normal_south_1", "../Assets/enemies/normal_enemy_down_1.png");
    loadTexture("normal_south_2", "../Assets/enemies/normal_enemy_down_2.png");
    loadTexture("normal_east_1", "../Assets/enemies/normal_enemy_right_1.png");
    loadTexture("normal_east_2", "../Assets/enemies/normal_enemy_right_2.png");
    loadTexture("normal_west_1", "../Assets/enemies/normal_enemy_left_1.png");
    loadTexture("normal_west_2", "../Assets/enemies/normal_enemy_left_2.png");

    //fast enemies
    loadTexture("fast_north_1", "../Assets/enemies/fast_enemy_up_1.png");
    loadTexture("fast_north_2", "../Assets/enemies/fast_enemy_up_2.png");
    loadTexture("fast_south_1", "../Assets/enemies/fast_enemy_down_1.png");
    loadTexture("fast_south_2", "../Assets/enemies/fast_enemy_down_2.png");
    loadTexture("fast_east_1", "../Assets/enemies/fast_enemy_right_1.png");
    loadTexture("fast_east_2", "../Assets/enemies/fast_enemy_right_2.png");
    loadTexture("fast_west_1", "../Assets/enemies/fast_enemy_left_1.png");
    loadTexture("fast_west_2", "../Assets/enemies/fast_enemy_left_2.png");

    //tank enemies
    loadTexture("tank_north_1", "../Assets/enemies/tank_enemy_up_1.png");
    loadTexture("tank_north_2", "../Assets/enemies/tank_enemy_up_2.png");
    loadTexture("tank_south_1", "../Assets/enemies/tank_enemy_down_1.png");
    loadTexture("tank_south_2", "../Assets/enemies/tank_enemy_down_2.png");
    loadTexture("tank_east_1", "../Assets/enemies/tank_enemy_right_1.png");
    loadTexture("tank_east_2", "../Assets/enemies/tank_enemy_right_2.png");
    loadTexture("tank_west_1", "../Assets/enemies/tank_enemy_left_1.png");
    loadTexture("tank_west_2", "../Assets/enemies/tank_enemy_left_2.png");
    
    
    // Load explosion pngs
    loadTexture("explosion1", "../Assets/grid/explosion-1.png");
    loadTexture("explosion2", "../Assets/grid/explosion-2.png");
    loadTexture("explosion3", "../Assets/grid/explosion-3.png");
    loadTexture("explosion4", "../Assets/grid/explosion-4.png");
    loadTexture("explosion5", "../Assets/grid/explosion-5.png");

    std::cout << "=== Asset Loading Complete ===" << std::endl;
    std::cout << "Textures loaded: " << textures.size() << std::endl;
    std::cout << "Fonts loaded: " << fonts.size() << std::endl << std::endl;
}