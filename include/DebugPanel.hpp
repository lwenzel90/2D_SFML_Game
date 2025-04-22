#ifndef DEBUG_PANEL_HPP
#define DEBUG_PANEL_HPP

#include <SFML/Graphics.hpp>
#include "DebugWindow.hpp"

#include <vector>
#include <string>
#include <functional>
#include <memory>

class Player;
class Attack;

class DebugPanel {
public:
    DebugPanel();
    void setFont(const sf::Font& font);
    void setPosition(const sf::Vector2f& pos);
    void setLineSpacing(float spacing);
    void clear();
    void addLine(const std::string& line);
    void draw(sf::RenderWindow& window);
    void drawCompass(sf::RenderWindow& window, float angleDegrees);
    void drawCenterCompass(sf::RenderWindow& window, const sf::Vector2f& playerPos, const sf::Vector2f& centerPos);

    // Debug window functions
    void createDebugWindow();
    void updateDebugWindow(Player& player, Attack& attack);
    bool hasDebugWindow() const { return debugWindow != nullptr && debugWindow->isOpen(); }
    void closeDebugWindow();

private:
    std::vector<std::string> lines;
    sf::Font font;
    sf::Vector2f position;
    float lineSpacing;
    unsigned int fontSize;
    
    // Debug window
    std::unique_ptr<DebugWindow> debugWindow;
};

#endif
