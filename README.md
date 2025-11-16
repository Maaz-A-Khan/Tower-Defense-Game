# Tower Defense Game

This repository contains a classic Tower Defense game built using C++ and the SFML graphics library. The core feature of this game is its dynamic pathfinding system, which utilizes the A* algorithm to allow enemies to navigate around towers placed by the player.

## Features

*   **Dynamic Pathfinding:** Enemies use the A* algorithm to find the optimal path from the start to the goal. The path is recalculated every time a new tower is placed, forcing enemies to adapt their route.
*   **Strategic Tower Placement:** Players can place towers to create complex mazes for enemies. The game validates each placement to ensure the enemy path is never completely blocked.
*   **Variety of Towers:**
    *   **Barrier Tower:** A low-cost, passive tower that only blocks the path, used for creating intricate mazes.
    *   **Gatling Tower:** A standard single-target tower with a high fire rate that deals consistent damage.
    *   **Frost Tower:** Creates an area-of-effect (AoE) slow zone that increases the traversal cost of grid cells for enemies, affecting their pathfinding decisions.
    *   **Artillery Tower:** A long-range tower that fires projectiles dealing area-of-effect (splash) damage upon impact.
*   **Diverse Enemy Types:**
    *   **Normal:** Standard health and speed.
    *   **Fast:** Low health but high speed, posing a quick threat.
    *   **Tank:** Very high health and low speed, difficult to take down.
    *   **Shielded:** An enemy with a layer of shields that must be depleted before its health can be damaged.
*   **Real-time Gameplay:** Enemies spawn in waves, towers attack automatically, and projectiles travel towards their targets in real-time.
*   **Game UI:** A sidebar displays player lives, money, current wave, and information about the currently selected tower for placement.

## Technical Deep Dive

The game is built around several core components:

*   **Grid System:** The map is a tile-based grid where each tile (`Node`) can be empty, blocked by a tower, or affected by special modifiers like the Frost Tower's slow effect.
*   **A\* Algorithm (`AStarPathfinder`):** This algorithm is central to the game's dynamic nature.
    1.  When a player attempts to place a blocking tower, the corresponding grid cell is temporarily marked as blocked.
    2.  The A* algorithm runs to verify that a valid path still exists from the enemy spawn point to the goal.
    3.  If a path exists, the tower placement is confirmed. If not, the placement is rejected.
    4.  All active enemies are then re-routed to follow the new shortest path.
*   **SFML (Simple and Fast Multimedia Library):** SFML is used for all rendering, windowing, and input handling. This includes drawing the grid, animated sprites for enemies, towers, projectiles, and rendering UI elements like health bars and text.
*   **Game Loop:** The main game loop uses a fixed timestep to ensure consistent game logic and physics behavior (enemy movement, projectile travel) across different frame rates. It manages game state updates, rendering, and player input.

## How to Build and Run

### Prerequisites

*   A C++ compiler (e.g., g++, Clang, MSVC).
*   The **SFML** development libraries (Graphics, Window, System) installed and configured for your environment.

### Compilation

You can compile the project by targeting all the `.cpp` source files within the `App/` directory. The following is an example command for g++ on Linux/macOS, run from the repository root:

```bash
g++ App/main.cpp -o tower-defense -lsfml-graphics -lsfml-window -lsfml-system
```

### Running the Game

After successful compilation, run the executable.

*Note: The application's source code uses relative paths (`../Assets/`) to load game assets. For the game to run correctly, the executable must be run from a subdirectory within the project root, such as the `App/` directory.*

A simple way to achieve this is to place the compiled `tower-defense` executable inside the `App/` folder and run it from there:

```bash
# Example assuming the executable is now in the App/ directory
cd App/
./tower-defense
