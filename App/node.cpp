#include "Node.hpp"

// Constructor
Node::Node(int x, int y, bool walkable)
    : x(x), y(y), walkable(walkable), gCost(0), hCost(0), parent(nullptr) {}

// Get total cost (f = g + h)
float Node::getFCost() const {
    return gCost + hCost;
}

// Equality check (same coordinates)
bool Node::operator==(const Node& other) const {
    return x == other.x && y == other.y;
}
