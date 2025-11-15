#include "game_manager.hpp"
#include "main_menu.hpp"
#include "asset_manager.hpp"
#include <iostream>

int main() {
    try {
        // Create window for main menu
        sf::RenderWindow window(sf::VideoMode({1248, 720}), "Tower Defense");
        
        // Load assets
        AssetManager assetManager;
        assetManager.loadAllAssets();
        
        // Show main menu
        MainMenu menu(window, &assetManager);
        
        if (assetManager.hasTexture("menu_background")) {
            menu.setBackgroundTexture(assetManager.getTexture("menu_background"));
        }
        
        if (assetManager.hasTexture("start_button")) {
            menu.setStartButtonTexture(assetManager.getTexture("start_button"));
        }
        
        if (assetManager.hasTexture("exit_button")) {
            menu.setExitButtonTexture(assetManager.getTexture("exit_button"));
        }
        
        bool startGame = false;
        
        // Main menu loop
        while (window.isOpen() && !startGame) {
            startGame = menu.handleInput();
            menu.update();
            menu.render();
        }
        
        // If start was clicked, close menu window and start game
        if (startGame) {
            window.close();
            
            GameManager game;
            game.run();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
