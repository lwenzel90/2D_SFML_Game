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
