#include "a_star_path_finder.hpp"
#include <cmath>
#include <algorithm>

AStarPathfinder::AStarPathfinder(Grid* grid) : grid(grid) {}

float AStarPathfinder::calculateHCost(Node* a, Node* b) {
    // Manhattan Distance heuristic
    return std::abs(a->x - b->x) + std::abs(a->y - b->y);
}

std::vector<Node*> AStarPathfinder::findPath(Node* start, Node* end) {
    std::vector<Node*> path;
    // Early sanity checks before touching grid state
    if (!start || !end) return {};
    if (start == end) return {start};

    grid->resetCosts();
    openSet.clear();
    closedSet.clear();

    start->gCost = 0;
    start->hCost = calculateHCost(start, end);
    openSet.push(start);

    while (!openSet.empty()) {
        Node* current = openSet.pop();
        if (current == end) {
            reconstructPath(start, end, path);
            return path;
        }

        closedSet.insert(current);

        // Check all neighboring cells 
        for (Node* neighbor : grid->getNeighbors(current)) {
            // Skip if neighbor is blocked or already visited
            if (!neighbor->walkable || closedSet.count(neighbor)) 
                continue;

            // Calculate cost to reach this neighbor from current node
            float moveCost = neighbor->getMovementCost();  // Includes frost effect (if any)
            float newCostToNeighbor = current->gCost + moveCost;

            // If this path to neighbor is better than any previous path
            bool isNewNode = (neighbor->gCost == 0);
            bool isBetterPath = (newCostToNeighbor < neighbor->gCost);
            
            if (isNewNode || isBetterPath) {
                // Update this neighbor's pathfinding data
                neighbor->parent = current;              // Remember we came from 'current'
                neighbor->gCost = newCostToNeighbor;     // Cost from start to neighbor
                neighbor->hCost = calculateHCost(neighbor, end);  // Estimated cost to goal
                
                // Add to open set for evaluation
                openSet.push(neighbor);
            }
        }
    }

    return path;
}

void AStarPathfinder::reconstructPath(Node* start, Node* end, std::vector<Node*>& path) {
    Node* current = end;
    while (current) {
        path.push_back(current);
        current = current->parent;
    }
    std::reverse(path.begin(), path.end());
}
