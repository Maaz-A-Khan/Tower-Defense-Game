#pragma once
#include <cmath>

class Node {
public:
    int x, y;                 // Grid coordinates
    bool walkable;            // Whether this tile can be walked on or not

    float gCost;              // Distance from start node
    float hCost;              // Heuristic distance to end node
    float baseCost;           // Default traversal cost (usually 1.0f)
    float slowMultiplier;     // Additional movement penalty (used for Frost effect)
    Node* parent;             // Pointer to parent node in path reconstruction

    Node(int x = 0, int y = 0, bool walkable = true);

    // Get total pathfinding cost (g + h)
    float getFCost() const;

    // Get movement cost including terrain modifiers
    float getMovementCost() const;

    // Compare two nodes
    bool operator==(const Node& other) const;

    // Reset runtime costs (useful when recalculating paths)
    void resetCosts();
};
