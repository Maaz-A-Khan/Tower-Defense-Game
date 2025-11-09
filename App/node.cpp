#include "node.hpp"

// Constructor
Node::Node(int x, int y, bool walkable)
    : x(x),
      y(y),
      walkable(walkable),
      gCost(0),
      hCost(0),
      baseCost(1.0f),
      slowMultiplier(1.0f),
      parent(nullptr) {}

// fCost = gCost + hCost
float Node::getFCost() const {
    return gCost + hCost;
}

// Effective movement cost (affected by frost/slowing)
float Node::getMovementCost() const {
    return baseCost * slowMultiplier;
}

// Equality check
bool Node::operator==(const Node& other) const {
    return x == other.x && y == other.y;
}

// Reset temporary A* data
void Node::resetCosts() {
    gCost = 0;
    hCost = 0;
    parent = nullptr;
}
