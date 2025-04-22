#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player {
public:
    Player(float x, float y);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void rotate(float angle);
    void moveForward(float deltaTime);
    void handleRotation(float rotationSpeed, float deltaTime);
    sf::Vector2f getPosition() const;
    float getRotation() const;

    void applyMovementForce(float force, float deltaTime, float angleDegrees);
    void updateMovement(float deltaTime, float force);
    void setPosition(const sf::Vector2f& position) { shape.setPosition(position); }

    // Getters for debug controls
    float getAcceleration() const;
    float getFriction() const;
    float getMaxSpeed() const;
    float getScale() const;

    // Setters for debug controls
    void setAcceleration(float accel);
    void setFriction(float fric);
    void setMaxSpeed(float speed);
    void setScale(float scale);

private:
    sf::ConvexShape shape;
    float speed;
    float rotationSpeed;
    float acceleration;
    float maxSpeed;
    float friction;
    sf::Vector2f velocity;
    
};

#endif
