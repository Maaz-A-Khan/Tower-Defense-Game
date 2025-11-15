#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <optional>
#include "enemy.hpp"

// Enumeration for tower types
enum class TowerType {
    Barrier,
    Gatling,
    Frost,
    Artillery
};

// Abstract Tower base class
class Tower {
protected:
    sf::Vector2f position;
    float range;
    float fireRate;
    float cooldown;
    int cost;
    bool isBlocking;
    TowerType type;
    
    std::optional<sf::Sprite> baseSprite;    // Static base
    std::optional<sf::Sprite> shooterSprite; // Rotating shooter
    float currentRotation = -90.f;

public:
    Tower(sf::Vector2f pos, float range, float fireRate, int cost, bool isBlocking, TowerType type);
    virtual ~Tower() = default;

    // Core virtual methods
    virtual void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    // Texture support
    void setBaseTexture(sf::Texture& texture);
    void setShooterTexture(sf::Texture& texture);
    
    // Utility
    bool canAttack(Enemy* enemy);

    void rotateToward(const sf::Vector2f& targetPos);

    // Accessors
    const sf::Vector2f& getPosition() const { return position; }
    float getRange() const { return range; }
    bool getIsBlocking() const { return isBlocking; }
    int getCost() const { return cost; }
    TowerType getType() const { return type; }
};
