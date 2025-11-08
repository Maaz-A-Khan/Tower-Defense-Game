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

        for (Node* neighbor : grid->getNeighbors(current)) {
            if (!neighbor->walkable || closedSet.count(neighbor)) continue;

            float moveCost = neighbor->getMovementCost(); 
            float tentativeG = current->gCost + moveCost;

            if (tentativeG < neighbor->gCost || neighbor->gCost == 0) {
                neighbor->parent = current;
                neighbor->gCost = tentativeG;
                neighbor->hCost = calculateHCost(neighbor, end);
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
