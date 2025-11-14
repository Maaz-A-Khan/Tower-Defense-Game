#include "grid.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

Grid::Grid(int w, int h) : gridTexture(nullptr) {
    initialize(w, h);
}

void Grid::initialize(int w, int h) {
    width = w;
    height = h;
    nodes.resize(height, std::vector<Node>(width));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            nodes[y][x] = Node(x, y, true);
            nodes[y][x].baseCost = 1.0f;
            nodes[y][x].slowMultiplier = 1.0f;
        }
    }
}

void Grid::setTexture(sf::Texture& texture) {
    gridTexture = &texture;
    
    std::cout << "Grid texture set: " << texture.getSize().x << "x" << texture.getSize().y << std::endl;
}

Node* Grid::getNode(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height)
        return &nodes[y][x];
    return nullptr;
}

std::vector<Node*> Grid::getNeighbors(Node* node) {
    std::vector<Node*> neighbors;
    int x = node->x;
    int y = node->y;

    if (y > 0) neighbors.push_back(&nodes[y - 1][x]);
    if (y < height - 1) neighbors.push_back(&nodes[y + 1][x]);
    if (x > 0) neighbors.push_back(&nodes[y][x - 1]);
    if (x < width - 1) neighbors.push_back(&nodes[y][x + 1]);

    return neighbors;
}

bool Grid::isWalkable(int x, int y) {
    Node* node = getNode(x, y);
    return node && node->walkable;
}

void Grid::setObstacle(int x, int y, bool blocked) {
    Node* node = getNode(x, y);
    if (node) node->walkable = !blocked;
}

void Grid::setStartEnd(sf::Vector2i start, sf::Vector2i end) {
    startCell = start;
    endCell = end;
}

void Grid::applyFrostEffect(int centerX, int centerY, int radius, float slowMultiplier) {
    for (int y = centerY - radius; y <= centerY + radius; y++) {
        for (int x = centerX - radius; x <= centerX + radius; x++) {
            Node* node = getNode(x, y);
            if (!node || !node->walkable) continue;

            if (std::abs(x - centerX) <= radius && std::abs(y - centerY) <= radius) {
                node->slowMultiplier = std::min(node->slowMultiplier + slowMultiplier, 1.7f);
            }
        }
    }
}

void Grid::resetFrostEffects() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            nodes[y][x].slowMultiplier = 1.0f;
        }
    }
}
void Grid::resetCosts() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            nodes[y][x].resetCosts();
        }
    }
}

void Grid::draw(sf::RenderWindow& window) {
    float cellSize = 32.f;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Node& node = nodes[y][x];

            // Draw the grid texture if available
            if (gridTexture) {
                sf::Sprite gridSprite(*gridTexture);  // Create sprite each time
                gridSprite.setPosition({x * cellSize, y * cellSize});
                
                // Scale to cell size
                sf::Vector2u texSize = gridTexture->getSize();
                float scaleX = cellSize / texSize.x;
                float scaleY = cellSize / texSize.y;
                gridSprite.setScale({scaleX, scaleY});
                
                // Tint the sprite based on tile type
                if (!node.walkable) {
                    gridSprite.setColor(sf::Color(50, 50, 50));  // Dark for obstacles
                } else if (sf::Vector2i(x, y) == startCell) {
                    gridSprite.setColor(sf::Color(0, 255, 0, 200));  // Green for start
                } else if (sf::Vector2i(x, y) == endCell) {
                    gridSprite.setColor(sf::Color(255, 0, 0, 200));  // Red for end
                } else if (node.slowMultiplier > 1.0f) {
                    gridSprite.setColor(sf::Color(100, 180, 255));  // Blue for frost
                } else {
                    gridSprite.setColor(sf::Color::White);  // Normal
                }
                
                window.draw(gridSprite);
            } else {
                // Fallback to rectangle rendering if no texture
                sf::RectangleShape rect({cellSize - 1, cellSize - 1});
                
                if (!node.walkable)
                    rect.setFillColor(sf::Color::Black);
                else if (sf::Vector2i(x, y) == startCell)
                    rect.setFillColor(sf::Color::Green);
                else if (sf::Vector2i(x, y) == endCell)
                    rect.setFillColor(sf::Color::Red);
                else if (node.slowMultiplier > 1.0f)
                    rect.setFillColor(sf::Color(100, 180, 255)); 
                else
                    rect.setFillColor(sf::Color(200, 200, 200));

                rect.setPosition({x * cellSize, y * cellSize});
                window.draw(rect);
            }
        }
    }
}
