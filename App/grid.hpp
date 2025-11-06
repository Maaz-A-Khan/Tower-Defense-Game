#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "node.hpp"

class Grid {
private:
    int width, height;
    std::vector<std::vector<Node>> nodes;
    sf::Vector2i startCell, endCell;

public:
    Grid(int w = 0, int h = 0);
    void initialize(int w, int h);
    
    Node* getNode(int x, int y);
    std::vector<Node*> getNeighbors(Node* node);
    
    bool isWalkable(int x, int y);
    void setObstacle(int x, int y, bool blocked);
    
    void setStartEnd(sf::Vector2i start, sf::Vector2i end);
    void draw(sf::RenderWindow& window);
};
