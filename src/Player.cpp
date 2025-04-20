#include "Player.hpp"
#include <cmath>

// Triangle shape, pointing up
Player::Player(float x, float y) : speed(200.0f), rotationSpeed(180.0f), velocity(0.f, 0.f), acceleration(600.f), friction(800.f), maxSpeed(600.f) {
    shape.setPointCount(3);
    shape.setPoint(0, sf::Vector2f(0.f, -20.f));
    shape.setPoint(1, sf::Vector2f(-15.f, 15.f));
    shape.setPoint(2, sf::Vector2f(15.f, 15.f));
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(x, y);
    shape.setRotation(0.f);
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

void Player::applyMovementForce(float force, float deltaTime) {
    // force: 1.0 = full forward, 0 = none
    float angleRad = getRotation() * 3.14159265f / 180.f;
    sf::Vector2f forward(std::cos(angleRad), std::sin(angleRad));
    if (force > 0.f) {
        velocity += forward * (acceleration * force * deltaTime);
    }
    updateMovement(deltaTime);
}

void Player::updateMovement(float deltaTime) {
    // Apply friction if not accelerating
    if (velocity.x != 0.f || velocity.y != 0.f) {
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
    // Update position
    setPosition(getPosition() + velocity * deltaTime);
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
