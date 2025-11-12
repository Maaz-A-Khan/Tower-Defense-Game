#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "node.hpp"

class Enemy {
protected:
    sf::Vector2f position;
    float baseSpeed;
    int health;
    bool reachedGoal;

    std::vector<Node*> path;
    int currentNodeIndex;

    sf::CircleShape shape;

public:
    Enemy(const std::vector<Node*>& path, float speed = 100.f, int health = 100);
    virtual ~Enemy() = default;

    virtual void update(float deltaTime);
    virtual void draw(sf::RenderWindow& window);
    virtual void takeDamage(int dmg);

    void setPath(const std::vector<Node*>& newPath);

    bool isDead() const;
    bool hasReachedGoal() const;

    const sf::Vector2f& getPosition() const;
    int getHealth() const { return health; }

    virtual float getCurrentSpeed(Node* node) const;
};

// ---------------------------------------------------------------------------
// Enemy Subclasses
// ---------------------------------------------------------------------------

class NormalEnemy : public Enemy {
public:
    NormalEnemy(const std::vector<Node*>& path);
};

class FastEnemy : public Enemy {
public:
    FastEnemy(const std::vector<Node*>& path);
};

class TankEnemy : public Enemy {
public:
    TankEnemy(const std::vector<Node*>& path);
};

class ShieldEnemy : public Enemy {
    int shield;
public:
    ShieldEnemy(const std::vector<Node*>& path);
    void takeDamage(int dmg) override;
    int getShield() const { return shield; }
};

#endif
