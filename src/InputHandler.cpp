#include "InputHandler.hpp"
#include <SFML/Window/Keyboard.hpp>

InputHandler::InputHandler()
    : rotateLeft(false), rotateRight(false), moveForward(false), attackToggle(false), prevSpacePressed(false),
      fastRotateLeft(false), fastRotateRight(false) {}

void InputHandler::update() {
    rotateLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    rotateRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    moveForward = sf::Keyboard::isKeyPressed(sf::Keyboard::W);

    bool spacePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    attackToggle = spacePressed && !prevSpacePressed;
    prevSpacePressed = spacePressed;

    bool shift = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);

    fastRotateLeft = rotateLeft && shift;
    fastRotateRight = rotateRight && shift;
}

bool InputHandler::isRotateLeft() const {
    return rotateLeft;
}

bool InputHandler::isRotateRight() const {
    return rotateRight;
}

bool InputHandler::isMoveForward() const {
    return moveForward;
}

bool InputHandler::isAttackToggled() const {
    return attackToggle;
}

bool InputHandler::isFastRotateLeft() const {
    return fastRotateLeft;
}

bool InputHandler::isFastRotateRight() const {
    return fastRotateRight;
}

bool InputHandler::isPausePressed() const {
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);
}

bool InputHandler::isMenuUp() const {
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W);
}

bool InputHandler::isMenuDown() const {
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S);
}

bool InputHandler::isMenuSelect() const {
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
}
