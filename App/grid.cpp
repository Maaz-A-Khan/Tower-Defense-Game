
#include "grid.hpp"

Grid::Grid(int w, int h) {
    initialize(w, h);
}

void Grid::initialize(int w, int h) {
    width = w;
    height = h;
    nodes.resize(height, std::vector<Node>(width));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            nodes[y][x] = Node(x, y, true);
        }
    }
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

void Grid::draw(sf::RenderWindow& window) {
    float cellSize = 32.f;
    sf::RectangleShape rect(sf::Vector2f(cellSize - 1, cellSize - 1));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Node& node = nodes[y][x];

            if (!node.walkable)
                rect.setFillColor(sf::Color::Black);
            else if (sf::Vector2i(x, y) == startCell)
                rect.setFillColor(sf::Color::Green);
            else if (sf::Vector2i(x, y) == endCell)
                rect.setFillColor(sf::Color::Red);
            else
                rect.setFillColor(sf::Color(200, 200, 200));

            rect.setPosition({x * cellSize, y * cellSize});
            window.draw(rect);
        }
    }
}
