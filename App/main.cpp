#include "game_manager.hpp"
#include <iostream>

int main() {
    try {
        GameManager game;
        game.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
