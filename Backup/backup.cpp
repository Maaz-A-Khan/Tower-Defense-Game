#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <queue>
#include <limits>
#include <string>
#include <cstdint>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace sf;

// --------------------- Helpers ---------------------
float length(Vector2f v) { return std::sqrt(v.x * v.x + v.y * v.y); }
Vector2f normalize(Vector2f v) {
    float l = length(v);
    return l == 0 ? Vector2f(0.f, 0.f) : v / l;
}
float distanceVec(Vector2f a, Vector2f b) { return length(a - b); }

// --------------------- Grid & Path ---------------------
const int GRID_W = 16;
const int GRID_H = 10;
const int TILE_SIZE = 48;
const int UI_HEIGHT = 80;

struct GridPos {
    int x, y;
};

bool operator==(const GridPos& a, const GridPos& b) {
    return a.x == b.x && a.y == b.y;
}

inline bool inBounds(int x, int y) {
    return x >= 0 && x < GRID_W && y >= 0 && y < GRID_H;
}

Vector2f gridToWorldCenter(const GridPos& p) {
    return Vector2f(
        p.x * TILE_SIZE + TILE_SIZE * 0.5f,
        p.y * TILE_SIZE + TILE_SIZE * 0.5f
    );
}

GridPos worldToGrid(const Vector2f& v) {
    return GridPos{
        static_cast<int>(v.x) / TILE_SIZE,
        static_cast<int>(v.y) / TILE_SIZE
    };
}

// --------------------- Cell / Grid ---------------------
enum class CellType { Empty, Blocked };

struct Cell {
    CellType type = CellType::Empty;
};

using Grid = std::vector<std::vector<Cell>>;

// --------------------- Towers & Enemies ---------------------
enum class TowerType { Normal, Range, Flame, Freeze, Wall };
enum class EnemyType { Normal, Shielded, Fast, Tank };

struct Enemy {
    EnemyType type;
    CircleShape shape;
    float baseSpeed;
    float speed;
    float health;
    float maxHealth;
    float slowTimer = 0.f;
    float slowFactor = 1.f;
    std::size_t pathIndex = 0;
    bool alive = true;
};

struct Projectile {
    CircleShape shape;
    Vector2f velocity;
    float damage;
    bool isFreeze = false;
    float slowFactor = 0.5f;
    float slowDuration = 1.f;
};

struct Tower {
    TowerType type;
    RectangleShape shape;
    float range;
    float damage;
    float cooldown;
    Clock fireClock;
    bool blocks = true;
};

// --------------------- A* Pathfinding ---------------------
struct Node {
    int index;
    float f;
};

bool operator>(const Node& a, const Node& b) {
    return a.f > b.f;
}

int toIndex(int x, int y) {
    return y * GRID_W + x;
}

GridPos fromIndex(int idx) {
    return GridPos{ idx % GRID_W, idx / GRID_W };
}

bool isWalkable(const Grid& grid, int x, int y) {
    if (!inBounds(x, y)) return false;
    return grid[y][x].type == CellType::Empty;
}

float manhattan(const GridPos& a, const GridPos& b) {
    return static_cast<float>(std::abs(a.x - b.x) + std::abs(a.y - b.y));
}

// Returns path of grid cells from start to goal (inclusive). Empty if no path.
std::vector<GridPos> findPathAStar(const Grid& grid, const GridPos& start, const GridPos& goal) {
    const int N = GRID_W * GRID_H;

    std::vector<float> gScore(N, std::numeric_limits<float>::infinity());
    std::vector<float> fScore(N, std::numeric_limits<float>::infinity());
    std::vector<int> cameFrom(N, -1);
    std::vector<bool> closed(N, false);

    auto startIdx = toIndex(start.x, start.y);
    auto goalIdx = toIndex(goal.x, goal.y);

    gScore[startIdx] = 0.f;
    fScore[startIdx] = manhattan(start, goal);

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open;
    open.push(Node{ startIdx, fScore[startIdx] });

    const GridPos dirs[4] = { {1,0}, {-1,0}, {0,1}, {0,-1} };

    while (!open.empty()) {
        int current = open.top().index;
        open.pop();

        if (closed[current]) continue;
        closed[current] = true;

        if (current == goalIdx) {
            // Reconstruct path
            std::vector<GridPos> path;
            int cur = current;
            while (cur != -1) {
                path.push_back(fromIndex(cur));
                cur = cameFrom[cur];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        GridPos cpos = fromIndex(current);

        for (const auto& d : dirs) {
            int nx = cpos.x + d.x;
            int ny = cpos.y + d.y;
            if (!inBounds(nx, ny)) continue;
            if (!isWalkable(grid, nx, ny)) continue;

            int ni = toIndex(nx, ny);
            if (closed[ni]) continue;

            float tentativeG = gScore[current] + 1.f; // uniform cost

            if (tentativeG < gScore[ni]) {
                cameFrom[ni] = current;
                gScore[ni] = tentativeG;
                fScore[ni] = tentativeG + manhattan(GridPos{ nx, ny }, goal);
                open.push(Node{ ni, fScore[ni] });
            }
        }
    }

    // no path
    return {};
}

// --------------------- Tower Factory ---------------------
Tower makeTower(TowerType t, const GridPos& gp) {
    Tower tower;
    tower.type = t;

    tower.shape = RectangleShape(Vector2f(TILE_SIZE - 4.f, TILE_SIZE - 4.f));
    Vector2f worldPos = Vector2f(
        gp.x * TILE_SIZE + 2.f,
        gp.y * TILE_SIZE + 2.f
    );
    tower.shape.setPosition(worldPos);
    tower.shape.setOutlineThickness(2.f);
    tower.shape.setOutlineColor(Color::Black);

    switch (t) {
    case TowerType::Normal:
        tower.shape.setFillColor(Color(220, 80, 60));
        tower.range = 150.f;
        tower.damage = 18.f;
        tower.cooldown = 0.7f;
        break;
    case TowerType::Range:
        tower.shape.setFillColor(Color(70, 200, 90));
        tower.range = 220.f;
        tower.damage = 10.f;
        tower.cooldown = 1.0f;
        break;
    case TowerType::Flame:
        tower.shape.setFillColor(Color(230, 150, 40));
        tower.range = 110.f;
        tower.damage = 8.f; // per tick to all enemies in range
        tower.cooldown = 0.25f;
        break;
    case TowerType::Freeze:
        tower.shape.setFillColor(Color(80, 200, 220));
        tower.range = 140.f;
        tower.damage = 4.f;
        tower.cooldown = 0.8f;
        break;
    case TowerType::Wall:
        tower.shape.setFillColor(Color(100, 100, 100));
        tower.range = 0.f;
        tower.damage = 0.f;
        tower.cooldown = 1000.f; // effectively never fires
        break;
    }

    tower.blocks = true;
    return tower;
}

// --------------------- Enemy Factory ---------------------
Enemy makeEnemy(EnemyType type, const std::vector<Vector2f>& worldPath) {
    Enemy e;
    e.type = type;
    e.shape = CircleShape(14.f);
    e.shape.setOrigin({14.f, 14.f});

    if (!worldPath.empty()) {
        e.shape.setPosition(worldPath.front());
    }

    switch (type) {
    case EnemyType::Normal:
        e.maxHealth = 60.f;
        e.baseSpeed = 70.f;
        e.shape.setFillColor(Color(180, 40, 40));
        break;
    case EnemyType::Shielded:
        e.maxHealth = 120.f;
        e.baseSpeed = 55.f;
        e.shape.setFillColor(Color(60, 80, 200));
        break;
    case EnemyType::Fast:
        e.maxHealth = 40.f;
        e.baseSpeed = 110.f;
        e.shape.setFillColor(Color(40, 200, 100));
        break;
    case EnemyType::Tank:
        e.maxHealth = 200.f;
        e.baseSpeed = 45.f;
        e.shape.setFillColor(Color(120, 60, 30));
        break;
    }

    e.health = e.maxHealth;
    e.speed = e.baseSpeed;
    e.pathIndex = 0;
    return e;
}

// --------------------- Projectile Factory ---------------------
Projectile makeProjectile(const Vector2f& from, const Vector2f& to, float dmg, bool freeze = false) {
    Projectile p;
    p.shape = CircleShape(5.f);
    p.shape.setOrigin({5.f, 5.f});
    p.shape.setPosition(from);
    p.damage = dmg;
    p.isFreeze = freeze;
    if (freeze) {
        p.slowFactor = 0.4f;
        p.slowDuration = 1.5f;
        p.shape.setFillColor(Color(120, 220, 255));
    } else {
        p.shape.setFillColor(Color::Yellow);
    }
    Vector2f dir = normalize(to - from);
    float speed = 280.f;
    p.velocity = dir * speed;
    return p;
}

// --------------------- Main ---------------------
int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    const int WINDOW_W = GRID_W * TILE_SIZE;
    const int WINDOW_H = GRID_H * TILE_SIZE + UI_HEIGHT;

    RenderWindow window(VideoMode({static_cast<unsigned>(WINDOW_W), static_cast<unsigned>(WINDOW_H)}),
                    "Grid TD with A* pathfinding");
    window.setFramerateLimit(60);

    // Grid
    Grid grid(GRID_H, std::vector<Cell>(GRID_W));

    // Start & goal (in grid coordinates)
    GridPos start{ 0, GRID_H / 2 };
    GridPos goal{ GRID_W - 1, GRID_H / 2 };

    // Towers & enemies
    std::vector<Tower> towers;
    std::vector<Enemy> enemies;
    std::vector<Projectile> projectiles;

    const int towerLimit = 40;
    TowerType selectedType = TowerType::Normal;

    // Path data
    std::vector<GridPos> pathCells;
    std::vector<Vector2f> pathWorld;

    // Initial path (no towers)
    pathCells = findPathAStar(grid, start, goal);
    if (pathCells.empty()) {
        std::cout << "No path from start to goal on empty grid?!\n";
        return 1;
    }
    pathWorld.clear();
    for (auto& gp : pathCells) {
        pathWorld.push_back(gridToWorldCenter(gp));
    }

    // Font / UI
    Font font;
    bool fontLoaded = font.openFromFile("arial.ttf");

    Text info(font, "", 16);
    info.setFillColor(Color::White);
    // Tower placement preview (semi-transparent overlay)
RectangleShape previewShape(Vector2f(TILE_SIZE - 4.f, TILE_SIZE - 4.f));
previewShape.setOutlineThickness(1.f);
previewShape.setOutlineColor(Color::White);
previewShape.setFillColor(Color(255, 255, 255, 80));

    // Enemy spawning
    Clock spawnClock;
    float spawnInterval = 2.0f;
    int spawnCount = 0;

    // Delta time
    Clock dtClock;

    int lives = 20;
    int totalPlacedTowers = 0;

    while (window.isOpen()) {
        float dt = dtClock.restart().asSeconds();

        // --------------------- Events ---------------------
        while (const auto event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }

            if (const auto* keyEvent = event->getIf<Event::KeyPressed>()) {
                if (keyEvent->code == Keyboard::Key::Num1) selectedType = TowerType::Normal;
                if (keyEvent->code == Keyboard::Key::Num2) selectedType = TowerType::Range;
                if (keyEvent->code == Keyboard::Key::Num3) selectedType = TowerType::Flame;
                if (keyEvent->code == Keyboard::Key::Num4) selectedType = TowerType::Freeze;
                if (keyEvent->code == Keyboard::Key::Num5) selectedType = TowerType::Wall;
            }

            if (event->is<Event::MouseButtonPressed>()) {
                auto* mb = event->getIf<Event::MouseButtonPressed>();
                if (mb && mb->button == Mouse::Button::Left) {
                    Vector2i mousePosI = Mouse::getPosition(window);
                    Vector2f mousePos(static_cast<float>(mousePosI.x), static_cast<float>(mousePosI.y));

                    // Only on grid area, not UI
                    if (mousePos.y < GRID_H * TILE_SIZE) {
                        GridPos gp = worldToGrid(mousePos);
                        if (inBounds(gp.x, gp.y)) {
                            if (!(gp == start || gp == goal) &&
                                totalPlacedTowers < towerLimit &&
                                grid[gp.y][gp.x].type == CellType::Empty) {

                                // Tentatively block and run A*
                                grid[gp.y][gp.x].type = CellType::Blocked;
                                auto newPath = findPathAStar(grid, start, goal);
                                if (newPath.empty()) {
                                    grid[gp.y][gp.x].type = CellType::Empty; // revert if path blocked
                                } else {
                                    // Accept tower
                                    Tower tower = makeTower(selectedType, gp);
                                    towers.push_back(tower);
                                    totalPlacedTowers++;

                                    // Update path
                                    pathCells = std::move(newPath);
                                    pathWorld.clear();
                                    for (auto& c : pathCells)
                                        pathWorld.push_back(gridToWorldCenter(c));

                                    // Re-route enemies
                                    for (auto& e : enemies) {
                                        if (!e.alive) continue;
                                        size_t bestIdx = 0;
                                        float bestDist = 1e9f;
                                        Vector2f pos = e.shape.getPosition();
                                        for (size_t i = 0; i < pathWorld.size(); ++i) {
                                            float d = distanceVec(pos, pathWorld[i]);
                                            if (d < bestDist) {
                                                bestDist = d;
                                                bestIdx = i;
                                            }
                                        }
                                        e.pathIndex = bestIdx;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // --------------------- Spawn enemies ---------------------
        if (spawnClock.getElapsedTime().asSeconds() > spawnInterval) {
            spawnClock.restart();
            // Cycle through enemy types for variety
            EnemyType type;
            switch (spawnCount % 4) {
            case 0: type = EnemyType::Normal; break;
            case 1: type = EnemyType::Shielded; break;
            case 2: type = EnemyType::Fast; break;
            default: type = EnemyType::Tank; break;
            }
            enemies.push_back(makeEnemy(type, pathWorld));
            spawnCount++;
        }

        // --------------------- Update enemies ---------------------
        for (auto& e : enemies) {
            if (!e.alive) continue;

            // Slow effect handling
            if (e.slowTimer > 0.f) {
                e.slowTimer -= dt;
                if (e.slowTimer <= 0.f) {
                    e.slowTimer = 0.f;
                    e.slowFactor = 1.f;
                }
            }
            e.speed = e.baseSpeed * e.slowFactor;

            if (pathWorld.empty()) continue;
            if (e.pathIndex >= pathWorld.size()) {
                // enemy reached end
                e.alive = false;
                lives--;
                continue;
            }

            Vector2f targetPos = pathWorld[e.pathIndex];
            Vector2f pos = e.shape.getPosition();
            Vector2f dir = targetPos - pos;
            float dist = length(dir);

            if (dist < 2.f) {
                e.pathIndex++;
            } else {
                Vector2f step = normalize(dir) * e.speed * dt;
                if (length(step) > dist) step = dir;
                e.shape.move(step);
            }
        }

        // --------------------- Tower attacks ---------------------
        for (auto& t : towers) {
            if (t.type == TowerType::Wall) continue; // purely blocking

            if (t.fireClock.getElapsedTime().asSeconds() < t.cooldown)
                continue;

            Vector2f towerCenter = t.shape.getPosition() + t.shape.getSize() * 0.5f;

            if (t.type == TowerType::Flame) {
                // AoE damage around tower
                bool hit = false;
                for (auto& e : enemies) {
                    if (!e.alive) continue;
                    float d = distanceVec(towerCenter, e.shape.getPosition());
                    if (d <= t.range) {
                        e.health -= t.damage * dt * 5.f; // scaled
                        hit = true;
                        if (e.health <= 0.f) {
                            e.alive = false;
                        }
                    }
                }
                if (hit) {
                    t.fireClock.restart();
                }
            } else {
                // Find single target
                Enemy* target = nullptr;
                float bestDist = t.range;

                for (auto& e : enemies) {
                    if (!e.alive) continue;
                    float d = distanceVec(towerCenter, e.shape.getPosition());
                    if (d <= t.range && d < bestDist) {
                        bestDist = d;
                        target = &e;
                    }
                }

                if (target) {
                    bool freezeProj = (t.type == TowerType::Freeze);
                    projectiles.push_back(
                        makeProjectile(towerCenter, target->shape.getPosition(), t.damage, freezeProj)
                    );
                    t.fireClock.restart();
                }
            }
        }

        // --------------------- Projectiles ---------------------
        for (std::size_t i = 0; i < projectiles.size();) {
            auto& p = projectiles[i];
            p.shape.move(p.velocity * dt);

            bool remove = false;

            // hit check
            for (auto& e : enemies) {
                if (!e.alive) continue;
                float d = distanceVec(p.shape.getPosition(), e.shape.getPosition());
                if (d < p.shape.getRadius() + e.shape.getRadius()) {
                    e.health -= p.damage;
                    if (p.isFreeze) {
                        e.slowFactor = p.slowFactor;
                        e.slowTimer = p.slowDuration;
                    }
                    if (e.health <= 0.f) {
                        e.alive = false;
                    }
                    remove = true;
                    break;
                }
            }

            // remove if off screen
            Vector2f pos = p.shape.getPosition();
            if (pos.x < -20 || pos.x > WINDOW_W + 20 ||
                pos.y < -20 || pos.y > WINDOW_H + 20) {
                remove = true;
            }

            if (remove) {
                projectiles.erase(projectiles.begin() + i);
            } else {
                ++i;
            }
        }

        // --------------------- Drawing ---------------------
        window.clear(Color(40, 40, 50));

        // Draw grid
        for (int y = 0; y < GRID_H; ++y) {
            for (int x = 0; x < GRID_W; ++x) {
                RectangleShape tile(Vector2f(TILE_SIZE - 1.f, TILE_SIZE - 1.f));
                tile.setPosition({static_cast<float>(x * TILE_SIZE), static_cast<float>(y * TILE_SIZE)});

                if (GridPos{x, y} == start) {
                    tile.setFillColor(Color(60, 160, 60));
                } else if (GridPos{x, y} == goal) {
                    tile.setFillColor(Color(160, 60, 60));
                } else if (grid[y][x].type == CellType::Blocked) {
                    tile.setFillColor(Color(90, 90, 90));
                } else {
                    tile.setFillColor(Color(80, 80, 90));
                }

                tile.setOutlineThickness(1.f);
                tile.setOutlineColor(Color(30, 30, 40));

                window.draw(tile);
            }
        }

        // Draw path
        if (pathWorld.size() >= 2) {
            VertexArray lineStrip(sf::PrimitiveType::LineStrip, pathWorld.size());
            for (std::size_t i = 0; i < pathWorld.size(); ++i) {
                lineStrip[i].position = pathWorld[i];
                lineStrip[i].color = Color(250, 220, 100);
            }
            window.draw(lineStrip);
        }

        // Draw towers
        for (auto& t : towers) {
            window.draw(t.shape);
        }

        // Draw projectiles
        for (auto& p : projectiles) {
            window.draw(p.shape);
        }

        // Draw enemies & health bars
        for (auto& e : enemies) {
            if (!e.alive) continue;
            window.draw(e.shape);

            float ratio = e.health / e.maxHealth;
            RectangleShape back(Vector2f(28.f, 4.f));
            back.setFillColor(Color(40, 40, 40));
            back.setPosition(e.shape.getPosition() + Vector2f(-14.f, -22.f));

            RectangleShape bar(Vector2f(28.f * ratio, 4.f));
            bar.setFillColor(Color(
                (int)(255 * (1.f - ratio)),
                (int)(255 * ratio),
                40
            ));
            bar.setPosition(back.getPosition());

            window.draw(back);
            window.draw(bar);
        }

        // UI background
        RectangleShape uiBg(Vector2f(static_cast<float>(WINDOW_W), static_cast<float>(UI_HEIGHT)));
        uiBg.setPosition({0.f, static_cast<float>(GRID_H * TILE_SIZE)});
        uiBg.setFillColor(Color(25, 25, 35));
        window.draw(uiBg);

        // UI text
        info.setCharacterSize(16);
        std::string towerName;
        switch (selectedType) {
        case TowerType::Normal: towerName = "Normal"; break;
        case TowerType::Range:  towerName = "Range"; break;
        case TowerType::Flame:  towerName = "Flame"; break;
        case TowerType::Freeze: towerName = "Freezing"; break;
        case TowerType::Wall:   towerName = "Wall"; break;
        }

        info.setString(
            "1:Normal  2:Range  3:Flame  4:Freezing  5:Wall   "
            "Selected: " + towerName +
            "   Towers: " + std::to_string(totalPlacedTowers) + "/" + std::to_string(towerLimit) +
            "   Lives: " + std::to_string(lives)
        );
        info.setPosition({10.f, static_cast<float>(GRID_H * TILE_SIZE + 20.f)});
        window.draw(info);
        
        Vector2i mposI = Mouse::getPosition(window);
        Vector2f mpos(static_cast<float>(mposI.x), static_cast<float>(mposI.y));
        if (mpos.y < GRID_H * TILE_SIZE) {
            GridPos gp = worldToGrid(mpos);
            if (inBounds(gp.x, gp.y) && grid[gp.y][gp.x].type == CellType::Empty && !(gp == start || gp == goal)) {
                previewShape.setPosition(Vector2f(gp.x * TILE_SIZE + 2.f, gp.y * TILE_SIZE + 2.f));
                switch (selectedType) {
                    case TowerType::Normal: previewShape.setFillColor(Color(220,80,60,100)); break;
                    case TowerType::Range:  previewShape.setFillColor(Color(70,200,90,100)); break;
                    case TowerType::Flame:  previewShape.setFillColor(Color(230,150,40,100)); break;
                    case TowerType::Freeze: previewShape.setFillColor(Color(80,200,220,100)); break;
                    case TowerType::Wall:   previewShape.setFillColor(Color(100,100,100,100)); break;
                }
                window.draw(previewShape);
            }
        }
        window.display();

        if (lives <= 0) {
            std::cout << "Game over!\n";
            window.close();
        }
    }

    return 0;
}
