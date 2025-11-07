# Tower Defense Game

This repository contains a classic Tower Defense game built using C++ and the SFML graphics library. The core feature of this game is its dynamic pathfinding system, which utilizes the A* algorithm to allow enemies to navigate around towers placed by the player.

## Features

*   **Dynamic Pathfinding:** Enemies use the A* algorithm to find the shortest path from start to goal. The path is recalculated every time a new tower is placed.
*   **Strategic Tower Placement:** Players can place towers to create complex mazes for enemies. The game prevents players from completely blocking the path.
*   **Variety of Towers:**
    *   **Barrier Tower:** A purely defensive tower that blocks enemy paths. It’s used for maze-building since it forces enemies to recalculate their A* path.
    *   **Gatling Tower:** A standard single-target tower with a high fire rate, dealing consistent damage to enemies.
    *   **Flame Tower:** Deals area-of-effect (AoE) damage to all enemies within its short range.
    *   **Frost Tower:** Emits an area-of-effect (AoE) slow zone that increases the traversal cost of nearby grid cells. It doesn’t block the path but slows enemies and affects their pathfinding decisions.
    *   **Artillery Tower:** Long-range tower that deals AoE (splash) damage to all enemies within its blast radius.
*   **Diverse Enemy Types:**
    *   **Normal:** Standard health and speed.
    *   **Shielded:** High health, average speed.
    *   **Fast:** Low health, high speed.
    *   **Tank:** Very high health, low speed.
*   **Real-time Gameplay:** Enemies spawn in waves, towers attack automatically, and projectiles travel towards their targets.
*   **Simple UI:** An in-game UI displays player lives, the number of towers placed, and the currently selected tower for placement.

## Technical Deep Dive

The game is built around a few core components:

*   **Grid System:** The game map is a tile-based grid. Each tile (or `Node`) can be empty or blocked by a tower.
*   **A* Algorithm (`findPathAStar`):** This algorithm is the heart of the game's logic.
    1.  When a player attempts to place a tower, the corresponding grid cell is temporarily marked as `Blocked`.
    2.  The A* algorithm runs to see if a valid path still exists from the enemy spawn point to the goal.
    3.  If a path exists, the tower placement is confirmed, and the new path is stored.
    4.  If placing the tower would completely block the path, the placement is rejected.
    5.  All active enemies are then re-routed to follow the new path.
*   **SFML (Simple and Fast Multimedia Library):** SFML is used for all rendering and window management. This includes:
    *   Drawing the grid, towers, enemies, and projectiles.
    *   Displaying the UI text and health bars.
    *   Handling keyboard and mouse input for game controls.
*   **Game Loop:** The main game loop manages game state updates, including enemy movement, tower firing logic, projectile physics, and collision detection, all regulated by a delta time clock for consistent behavior across different frame rates.

## How to Build and Run

### Prerequisites

*   A C++ compiler (e.g., g++, Clang, MSVC).
*   The **SFML** development libraries installed and configured for your environment.

### Dependencies

The application requires the `arial.ttf` font file to be present in the same directory as the executable to render UI text.

### Compilation

You can compile the project using a command similar to the following (example for g++ on Linux/macOS):

```bash
g++ Backup/backup.cpp -o tower-defense -lsfml-graphics -lsfml-window -lsfml-system
```

After successful compilation, run the executable:

```bash
./tower-defense
