#pragma once
#include <vector>
#include <unordered_set>
#include "grid.hpp"
#include "min_heap.hpp"

class AStarPathfinder {
private:
    Grid* grid;
    MinHeap openSet;
    std::unordered_set<Node*> closedSet;

    float calculateHCost(Node* a, Node* b);
    void reconstructPath(Node* start, Node* end, std::vector<Node*>& path);

public:
    AStarPathfinder(Grid* grid);
    std::vector<Node*> findPath(Node* start, Node* end);
};
