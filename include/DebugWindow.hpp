#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <vector>

class Player;
class Attack;

class DebugWindow {
public:
    DebugWindow();
    ~DebugWindow();

    void create();
    bool isOpen() const;
    void close();
    void update(Player& player, Attack& attack);
    void processEvents();
    void render();
    void loadFont(); // Load font from file

private:
    bool m_isOpen = false;
    std::unique_ptr<sf::RenderWindow> window;
    sf::Font font;
    sf::Text m_text;
};