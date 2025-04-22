#include "Player.hpp"

#include <cmath>
#include <iostream>

namespace {
    // Player Attributes
    constexpr float PLAYER_ACCELERATION = 600.f;
    constexpr float PLAYER_FRICTION = 800.f;
    constexpr float PLAYER_MAX_SPEED = 600.f;
    const sf::Color PLAYER_COLOR = sf::Color::Green;
    constexpr float INITIAL_ROTATION_DEG = 0.f;

    // Shape Definition (Triangle pointing up)
    constexpr float PLAYER_SHAPE_POINT_0_Y = -20.f;
    constexpr float PLAYER_SHAPE_POINT_1_X = -15.f;
    constexpr float PLAYER_SHAPE_POINT_1_Y = 15.f;
    constexpr float PLAYER_SHAPE_POINT_2_X = 15.f;
    constexpr float PLAYER_SHAPE_POINT_2_Y = 15.f;

    // Movement Calculation
    constexpr float ANGLE_CORRECTION_DEG = 90.f;
    constexpr float PI = 3.14159265f;
}

// Triangle shape, pointing up
Player::Player(float x, float y) : speed(200.0f), rotationSpeed(180.0f), velocity(0.f, 0.f), acceleration(PLAYER_ACCELERATION), friction(PLAYER_FRICTION), maxSpeed(PLAYER_MAX_SPEED) {
    shape.setPointCount(3);
    shape.setPoint(0, sf::Vector2f(0.f, PLAYER_SHAPE_POINT_0_Y));
    shape.setPoint(1, sf::Vector2f(PLAYER_SHAPE_POINT_1_X, PLAYER_SHAPE_POINT_1_Y));
    shape.setPoint(2, sf::Vector2f(PLAYER_SHAPE_POINT_2_X, PLAYER_SHAPE_POINT_2_Y));
    shape.setFillColor(PLAYER_COLOR);
    shape.setPosition(x, y);
    shape.setRotation(INITIAL_ROTATION_DEG);
}

void Player::update(float deltaTime) {
    // No automatic update for now
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Player::rotate(float angle) {
    shape.rotate(angle);
}

void Player::updateMovement(float deltaTime, float force) {
    // Apply friction if not accelerating
    if (force == 0.f && (velocity.x != 0.f || velocity.y != 0.f)) {
        sf::Vector2f frictionVec = -velocity;
        float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        if (speed > 0.f) {
            frictionVec /= speed;
            float frictionAmount = friction * deltaTime;
            if (frictionAmount > speed) frictionAmount = speed;
            velocity += frictionVec * frictionAmount;
        }
    }
    // Clamp speed
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (speed > maxSpeed) {
        velocity = velocity / speed * maxSpeed;
    }
    // Update position using velocity
    shape.setPosition(shape.getPosition() + velocity * deltaTime);
}

void Player::applyMovementForce(float force, float deltaTime, float angleDegrees) {
    // Correct the direction: subtract 90 degrees so 0 points "up"
    float angleRad = (angleDegrees - ANGLE_CORRECTION_DEG) * PI / 180.f;
    float fx = std::cos(angleRad) * force * acceleration * deltaTime;
    float fy = std::sin(angleRad) * force * acceleration * deltaTime;
    velocity.x += fx;
    velocity.y += fy;
}

void Player::handleRotation(float rotationSpeed, float deltaTime) {
    if (rotationSpeed != 0.f) {
        rotate(rotationSpeed * deltaTime);
    }
}

sf::Vector2f Player::getPosition() const {
    return shape.getPosition();
}

float Player::getRotation() const {
    return shape.getRotation();
}

// --- Getters for Debug Controls ---
float Player::getAcceleration() const {
    return acceleration;
}

float Player::getFriction() const {
    return friction;
}

float Player::getMaxSpeed() const {
    return maxSpeed;
}

float Player::getScale() const {
    // Assuming uniform scaling; return x-scale
    return shape.getScale().x;
}

// --- Setters for Debug Controls ---
void Player::setAcceleration(float accel) {
    acceleration = accel > 0 ? accel : 0; // Ensure non-negative
}

void Player::setFriction(float fric) {
    friction = fric > 0 ? fric : 0; // Ensure non-negative
}

void Player::setMaxSpeed(float speed) {
    maxSpeed = speed > 0 ? speed : 0; // Ensure non-negative
}

void Player::setScale(float scale) {
    float clampedScale = scale > 0.1f ? scale : 0.1f; // Prevent zero or negative scale
    shape.setScale(clampedScale, clampedScale);
    // Adjust origin if necessary after scaling, though center origin is usually fine
    // sf::FloatRect bounds = shape.getLocalBounds();
    // shape.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
}
