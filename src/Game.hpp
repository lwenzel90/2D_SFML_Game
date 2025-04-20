#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include "Player.hpp"
#include "Attack.hpp"
#include "InputHandler.hpp"

class Game {
public:
    Game();
    void run(sf::RenderWindow& window);
    bool isRunning() const;
    void stop();

private:
    void handleWindowEvents(sf::RenderWindow& window);
    float handleRotation(float deltaTime);
    void handleMovement(float deltaTime);
    void handleAttack(float deltaTime, float rotationApplied);
    void render(sf::RenderWindow& window);

    bool running;
    Player player;
    Attack attack;
    InputHandler inputHandler;
    bool attackToggle;
};

#endif
