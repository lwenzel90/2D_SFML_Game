#include "Game.hpp"
#include "Player.hpp"
#include "Attack.hpp"
#include "InputHandler.hpp"
#include <SFML/Graphics.hpp>

Game::Game()
    : running(true), player(400.f, 300.f), attack(), inputHandler(), attackToggle(false)
{
    // ...any other initialization...
}

void Game::run(sf::RenderWindow& window) {
    sf::Clock clock;
    while (window.isOpen() && isRunning()) {
        handleWindowEvents(window);

        float deltaTime = clock.restart().asSeconds();

        inputHandler.update();

        float rotationApplied = handleRotation(deltaTime);
        handleMovement(deltaTime);
        handleAttack(deltaTime, rotationApplied);

        render(window);
    }
}

// --- New helper methods ---

void Game::handleWindowEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

float Game::handleRotation(float deltaTime) {
    float rotationSpeed = 0.f;
    if (inputHandler.isFastRotateLeft()) {
        rotationSpeed = -360.f;
    } else if (inputHandler.isRotateLeft()) {
        rotationSpeed = -180.f;
    } else if (inputHandler.isFastRotateRight()) {
        rotationSpeed = 360.f;
    } else if (inputHandler.isRotateRight()) {
        rotationSpeed = 180.f;
    }
    player.handleRotation(rotationSpeed, deltaTime);
    return rotationSpeed * deltaTime;
}

void Game::handleMovement(float deltaTime) {
    float force = 0.f;
    if (inputHandler.isMoveForward()) {
        force = 1.f; // Apply forward force
    }
    player.applyMovementForce(force, deltaTime);
}

void Game::handleAttack(float deltaTime, float rotationApplied) {
    if (inputHandler.isAttackToggled()) {
        attackToggle = !attackToggle;
    }
    float attackAngle = player.getRotation() + (rotationApplied * 0.5f);
    attack.update(
        deltaTime,
        player.getPosition(),
        attackAngle,
        attackToggle
    );
}

void Game::render(sf::RenderWindow& window) {
    window.clear();
    player.draw(window);
    attack.draw(window);
    window.display();
}

bool Game::isRunning() const {
    return running;
}

void Game::stop() {
    running = false;
}
