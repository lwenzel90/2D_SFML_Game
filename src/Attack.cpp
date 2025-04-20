#include "Attack.hpp"
#include <cmath>

// Default constants
constexpr float DEFAULT_PROJECTILE_SIZE = 2.0f;
constexpr float DEFAULT_SHOOT_COOLDOWN = 0.05f;
constexpr float DEFAULT_PROJECTILE_SPEED = 400.0f;
constexpr float DEFAULT_SCREEN_WIDTH = 800.0f;
constexpr float DEFAULT_SCREEN_HEIGHT = 600.0f;

Attack::Attack()
    : Attack(DEFAULT_PROJECTILE_SIZE, DEFAULT_SHOOT_COOLDOWN, DEFAULT_PROJECTILE_SPEED, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT) {}

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
        proj.shape.setFillColor(sf::Color::Yellow);
        proj.shape.setOrigin(projectileSize, projectileSize);
        proj.shape.setPosition(playerPos);

        float angleRad = (playerAngle - 90.0f) * 3.14159265f / 180.0f;
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
                return pos.x < 0 || pos.x > screenWidth || pos.y < 0 || pos.y > screenHeight;
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
