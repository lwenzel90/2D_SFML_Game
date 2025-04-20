
#ifndef BALL_HPP
#define BALL_HPP

#include <SFML/Graphics.hpp>

class Ball {
public:
    Ball(float radius, sf::Color color, float x, float y);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void setVelocity(float vx, float vy);

private:
    float radius;
    sf::Color color;
    float x, y;
    float velocityX;
    float velocityY;
    sf::CircleShape circle;
};

#endif