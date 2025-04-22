#include "Attack.hpp"

#include <cmath>
#include <iostream>
#include <algorithm>

namespace {
    // Default Attack Parameters
    constexpr float DEFAULT_PROJECTILE_SIZE = 2.0f;
    constexpr float DEFAULT_SHOOT_COOLDOWN_S = 0.05f;
    constexpr float DEFAULT_PROJECTILE_SPEED = 400.0f;
    constexpr float DEFAULT_SCREEN_WIDTH = 800.0f;
    constexpr float DEFAULT_SCREEN_HEIGHT = 600.0f;

    // Projectile Visuals
    const sf::Color PROJECTILE_COLOR = sf::Color::Yellow;

    // Calculation Constants
    constexpr float ANGLE_CORRECTION_DEG = 90.0f;
    constexpr float PI = 3.14159265f;
}

Attack::Attack()
    : Attack(DEFAULT_PROJECTILE_SIZE, DEFAULT_SHOOT_COOLDOWN_S, DEFAULT_PROJECTILE_SPEED, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT) {}

Attack::Attack(float projectileSize, float shootCooldown, float projectileSpeed, float screenWidth, float screenHeight)
    : attackActive(false),
      shootCooldown(shootCooldown),
      shootTimer(0.0f),
      projectileSpeed(projectileSpeed),
      projectileSize(projectileSize),
      screenWidth(screenWidth),
      screenHeight(screenHeight) {}

void Attack::update(float deltaTime, const sf::Vector2f& playerPos, float playerAngle, bool attackActiveInput) {
    // Update attack state
    attackActive = attackActiveInput;

    // Handle shooting
    shootTimer += deltaTime;
    if (attackActive && shootTimer >= shootCooldown) {
        Projectile proj;
        proj.shape = sf::CircleShape(projectileSize);
        proj.shape.setFillColor(PROJECTILE_COLOR);
        proj.shape.setOrigin(projectileSize, projectileSize);
        proj.shape.setPosition(playerPos);

        float angleRad = (playerAngle - ANGLE_CORRECTION_DEG) * PI / 180.0f;
        proj.velocity = sf::Vector2f(std::cos(angleRad), std::sin(angleRad)) * projectileSpeed;

        projectiles.push_back(proj);
        shootTimer = 0.0f;
    }

    // Update projectiles
    for (auto& proj : projectiles) {
        proj.shape.move(proj.velocity * deltaTime);
    }

    // Remove projectiles that are off-screen (using configurable screen size)
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [this](const Projectile& proj) {
                sf::Vector2f pos = proj.shape.getPosition();
                // Use screenSize member variable for bounds checking
                return pos.x < 0 || pos.x > screenSize.x || pos.y < 0 || pos.y > screenSize.y;
            }),
        projectiles.end()
    );
}

void Attack::draw(sf::RenderWindow& window) {
    for (const auto& proj : projectiles) {
        window.draw(proj.shape);
    }
}

void Attack::setAttackActive(bool active) {
    attackActive = active;
}

bool Attack::isAttackActive() const {
    return attackActive;
}

const std::vector<Projectile>& Attack::getProjectiles() const {
    return projectiles;
}

// Define the setScreenSize method
void Attack::setScreenSize(sf::Vector2u size) {
    screenSize = size;
}

// --- Getters for Debug Controls ---
float Attack::getProjectileSize() const {
    return projectileSize;
}

float Attack::getShootCooldown() const {
    return shootCooldown;
}

float Attack::getProjectileSpeed() const {
    return projectileSpeed;
}

// --- Setters for Debug Controls ---
void Attack::setProjectileSize(float size) {
    projectileSize = size > 0.1f ? size : 0.1f; // Prevent tiny/negative size
}

void Attack::setShootCooldown(float cooldown) {
    shootCooldown = cooldown > 0.01f ? cooldown : 0.01f; // Prevent too rapid fire
}

void Attack::setProjectileSpeed(float speed) {
    projectileSpeed = speed;
}
