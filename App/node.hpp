#pragma once
#include <cmath>

class Node {
public:
    int x, y;              // Grid coordinates
    bool walkable;         // Can this cell be walked on or is it blocked?

    float gCost;           // Distance from start node
    float hCost;           // Heuristic distance to end node
    Node* parent;          // Pointer to the parent node in the path

    // Constructor
    Node(int x = 0, int y = 0, bool walkable = true);

    // Returns the total cost (g + h)
    float getFCost() const;

    // Compares two nodes by their coordinates
    bool operator==(const Node& other) const;
};
