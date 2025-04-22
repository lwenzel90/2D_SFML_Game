#ifndef ATTACK_HPP
#define ATTACK_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

// Standard C++ includes
#include <vector>

struct Projectile {
    sf::CircleShape shape;
    sf::Vector2f velocity;
};

class Attack {
public:
    Attack();
    Attack(float projectileSize, float shootCooldown, float projectileSpeed, float screenWidth, float screenHeight);
    void update(float deltaTime, const sf::Vector2f& playerPos, float playerAngle, bool attackActive);
    void draw(sf::RenderWindow& window);
    void setAttackActive(bool active);
    bool isAttackActive() const;
    void setScreenSize(sf::Vector2u size);

    // Getters for debug controls
    float getProjectileSize() const;
    float getShootCooldown() const;
    float getProjectileSpeed() const;

    // Setters for debug controls
    void setProjectileSize(float size);
    void setShootCooldown(float cooldown);
    void setProjectileSpeed(float speed);

    const std::vector<Projectile>& getProjectiles() const;

private:
    std::vector<Projectile> projectiles;
    bool attackActive;
    float shootCooldown;
    float shootTimer;
    float projectileSpeed;
    float projectileSize;
    float screenWidth;
    float screenHeight;
    sf::Vector2u screenSize;
};

#endif
