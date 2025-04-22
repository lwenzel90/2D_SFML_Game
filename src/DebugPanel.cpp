#include "DebugPanel.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <cmath>

namespace {
    // Default Layout
    constexpr float DEFAULT_POS_X = 10.f;
    constexpr float DEFAULT_POS_Y = 10.f;
    constexpr float DEFAULT_LINE_SPACING = 22.f;
    constexpr unsigned int DEFAULT_FONT_SIZE = 18;
    const sf::Color DEFAULT_TEXT_COLOR = sf::Color::White;

    // Compass Parameters
    constexpr float COMPASS_RADIUS = 32.f;
    constexpr float COMPASS_OUTLINE_THICKNESS = 2.f;
    const sf::Color COMPASS_OUTLINE_COLOR = sf::Color::White;
    const sf::Color COMPASS_BACKGROUND_COLOR = sf::Color(0, 0, 0, 0); // Transparent
    constexpr float COMPASS_NEEDLE_OFFSET = 6.f;
    const sf::Color PLAYER_NEEDLE_COLOR = sf::Color::Red;
    const sf::Color CENTER_NEEDLE_COLOR = sf::Color::Cyan;
    constexpr float COMPASS_LABEL_OFFSET_X = 8.f;
    constexpr float COMPASS_LABEL_OFFSET_Y = 18.f;
    constexpr float COMPASS_1_OFFSET_X = 10.f;
    constexpr float COMPASS_1_OFFSET_Y = 80.f;
    constexpr float COMPASS_2_OFFSET_Y_FACTOR = 2.f;
    constexpr float COMPASS_2_SPACING_Y = 20.f;

    // Calculation Constants
    constexpr float ANGLE_CORRECTION_DEG = 90.f;
    constexpr float PI = 3.14159265f;
}

// Default values for position and spacing
DebugPanel::DebugPanel()
    : position(DEFAULT_POS_X, DEFAULT_POS_Y),
      lineSpacing(DEFAULT_LINE_SPACING),
      fontSize(DEFAULT_FONT_SIZE)
{}

void DebugPanel::setFont(const sf::Font& f) {
    font = f;
}

void DebugPanel::setPosition(const sf::Vector2f& pos) {
    position = pos;
}

void DebugPanel::setLineSpacing(float spacing) {
    lineSpacing = spacing;
}

void DebugPanel::clear() {
    lines.clear();
}

void DebugPanel::addLine(const std::string& line) {
    lines.push_back(line);
}

void DebugPanel::draw(sf::RenderWindow& window) {
    float y = position.y;
    for (const auto& line : lines) {
        sf::Text text(line, font, fontSize); // Use fontSize member
        text.setFillColor(DEFAULT_TEXT_COLOR);
        text.setPosition(position.x, y);
        window.draw(text);
        y += lineSpacing; // Use lineSpacing member
    }
}

void DebugPanel::drawCompass(sf::RenderWindow& window, float angleDegrees) {
    // Compass parameters
    sf::Vector2f compassCenter(position.x + COMPASS_RADIUS + COMPASS_1_OFFSET_X, position.y + COMPASS_1_OFFSET_Y);

    // Draw compass circle
    sf::CircleShape circle(COMPASS_RADIUS);
    circle.setFillColor(COMPASS_BACKGROUND_COLOR);
    circle.setOutlineColor(COMPASS_OUTLINE_COLOR);
    circle.setOutlineThickness(COMPASS_OUTLINE_THICKNESS);
    circle.setOrigin(COMPASS_RADIUS, COMPASS_RADIUS);
    circle.setPosition(compassCenter);
    window.draw(circle);

    // Draw compass needle (player direction)
    float angleRad = (angleDegrees - ANGLE_CORRECTION_DEG) * PI / 180.f;
    sf::Vector2f needleEnd = compassCenter + sf::Vector2f(std::cos(angleRad), std::sin(angleRad)) * (COMPASS_RADIUS - COMPASS_NEEDLE_OFFSET);

    sf::Vertex needle[] = {
        sf::Vertex(compassCenter, PLAYER_NEEDLE_COLOR),
        sf::Vertex(needleEnd, PLAYER_NEEDLE_COLOR)
    };
    window.draw(needle, 2, sf::Lines);

    // Draw N label
    sf::Text nText("N", font, fontSize); // Use fontSize member
    nText.setFillColor(DEFAULT_TEXT_COLOR);
    nText.setPosition(compassCenter.x - COMPASS_LABEL_OFFSET_X, compassCenter.y - COMPASS_RADIUS - COMPASS_LABEL_OFFSET_Y);
    window.draw(nText);
}

void DebugPanel::drawCenterCompass(sf::RenderWindow& window, const sf::Vector2f& playerPos, const sf::Vector2f& centerPos) {
    // Draw a second compass below the first
    sf::Vector2f compassCenter(position.x + COMPASS_RADIUS + COMPASS_1_OFFSET_X,
                               position.y + COMPASS_1_OFFSET_Y + COMPASS_RADIUS * COMPASS_2_OFFSET_Y_FACTOR + COMPASS_2_SPACING_Y);

    // Draw compass circle
    sf::CircleShape circle(COMPASS_RADIUS);
    circle.setFillColor(COMPASS_BACKGROUND_COLOR);
    circle.setOutlineColor(COMPASS_OUTLINE_COLOR);
    circle.setOutlineThickness(COMPASS_OUTLINE_THICKNESS);
    circle.setOrigin(COMPASS_RADIUS, COMPASS_RADIUS);
    circle.setPosition(compassCenter);
    window.draw(circle);

    // Calculate angle from player to center
    sf::Vector2f toCenter = centerPos - playerPos;
    float angleRad = std::atan2(toCenter.y, toCenter.x);
    // No adjustment: 0 rad points right (east), PI/2 up, PI left, -PI/2 down

    sf::Vector2f needleEnd = compassCenter + sf::Vector2f(std::cos(angleRad), std::sin(angleRad)) * (COMPASS_RADIUS - COMPASS_NEEDLE_OFFSET);

    sf::Vertex needle[] = {
        sf::Vertex(compassCenter, CENTER_NEEDLE_COLOR),
        sf::Vertex(needleEnd, CENTER_NEEDLE_COLOR)
    };
    window.draw(needle, 2, sf::Lines);

    // Draw C label for "Center"
    sf::Text cText("C", font, fontSize); // Use fontSize member
    cText.setFillColor(CENTER_NEEDLE_COLOR);
    cText.setPosition(compassCenter.x - COMPASS_LABEL_OFFSET_X, compassCenter.y - COMPASS_RADIUS - COMPASS_LABEL_OFFSET_Y);
    window.draw(cText);
}

void DebugPanel::createDebugWindow() {
    if (!debugWindow) {
        debugWindow = std::make_unique<DebugWindow>();
        debugWindow->create();
    } else if (!debugWindow->isOpen()) {
        debugWindow->create();
    }
}

void DebugPanel::updateDebugWindow(Player& player, Attack& attack) {
    if (debugWindow && debugWindow->isOpen()) {
        debugWindow->update(player, attack);
    }
}

void DebugPanel::closeDebugWindow() {
    if (debugWindow) {
        debugWindow->close();
    }
}
