#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "node.hpp"

class Grid {
private:
    int width, height;
    std::vector<std::vector<Node>> nodes;

public:
    sf::Vector2i startCell, endCell;

    Grid(int w, int h);
    void initialize(int w, int h);
    Node* getNode(int x, int y);
    std::vector<Node*> getNeighbors(Node* node);
    bool isWalkable(int x, int y);
    void setObstacle(int x, int y, bool blocked);
    void setStartEnd(sf::Vector2i start, sf::Vector2i end);

    void applyFrostEffect(int centerX, int centerY, int radius, float slowMultiplier);
    void resetFrostEffects();
    void resetCosts(); // NEW

    void draw(sf::RenderWindow& window);
    
    // Getter methods for start and end positions
    sf::Vector2i getStart() const { return startCell; }
    sf::Vector2i getEnd() const { return endCell; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};
