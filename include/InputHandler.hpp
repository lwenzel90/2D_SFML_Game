#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include <SFML/Window/Keyboard.hpp>

class InputHandler {
public:
    InputHandler();

    void update();
    bool isRotateLeft() const;
    bool isRotateRight() const;
    bool isMoveForward() const;
    bool isAttackToggled() const;
    bool wasAttackJustPressed() const;
    bool isFastRotateLeft() const;
    bool isFastRotateRight() const;
    bool isPausePressed() const;
    bool isMenuUp() const;
    bool isMenuDown() const;
    bool isMenuSelect() const;


private:
    bool rotateLeft;
    bool rotateRight;
    bool moveForward;
    bool attackToggle;
    bool prevSpacePressed;
    bool fastRotateLeft;
    bool fastRotateRight;
};

#endif
