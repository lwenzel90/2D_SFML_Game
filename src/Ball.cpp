#include "Ball.hpp"


Ball::Ball(float radius, sf::Color color, float x, float y)
    : radius(radius), color(color), x(x), y(y), velocityX(50.0f), velocityY(50.0f) {
    circle.setRadius(radius);
    circle.setFillColor(color);
    circle.setPosition(x, y);
}

void Ball::update(float deltaTime) {
    x += velocityX * deltaTime;
    y += velocityY * deltaTime;
    circle.setPosition(x, y);
}

void Ball::draw(sf::RenderWindow& window) {
    window.draw(circle);
}

void Ball::setVelocity(float vx, float vy) {
    velocityX = vx;
    velocityY = vy;
}

