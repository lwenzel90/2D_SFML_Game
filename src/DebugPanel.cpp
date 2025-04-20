#include "DebugPanel.hpp"
#include <cmath>

// Default values for position and spacing
DebugPanel::DebugPanel()
    : position(10.f, 10.f), lineSpacing(22.f), fontSize(18)
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
        sf::Text text(line, font, fontSize);
        text.setFillColor(sf::Color::White);
        text.setPosition(position.x, y);
        window.draw(text);
        y += lineSpacing;
    }
}

void DebugPanel::drawCompass(sf::RenderWindow& window, float angleDegrees) {
    // Compass parameters
    float compassRadius = 32.f;
    sf::Vector2f compassCenter(position.x + compassRadius + 10.f, position.y + 80.f);

    // Draw compass circle
    sf::CircleShape circle(compassRadius);
    circle.setFillColor(sf::Color(0, 0, 0, 0));
    circle.setOutlineColor(sf::Color::White);
    circle.setOutlineThickness(2.f);
    circle.setOrigin(compassRadius, compassRadius);
    circle.setPosition(compassCenter);
    window.draw(circle);

    // Draw compass needle (player direction)
    float angleRad = (angleDegrees - 90.f) * 3.14159265f / 180.f;
    sf::Vector2f needleEnd = compassCenter + sf::Vector2f(std::cos(angleRad), std::sin(angleRad)) * (compassRadius - 6.f);

    sf::Vertex needle[] = {
        sf::Vertex(compassCenter, sf::Color::Red),
        sf::Vertex(needleEnd, sf::Color::Red)
    };
    window.draw(needle, 2, sf::Lines);

    // Draw N label
    sf::Text nText("N", font, fontSize);
    nText.setFillColor(sf::Color::White);
    nText.setPosition(compassCenter.x - 8.f, compassCenter.y - compassRadius - 18.f);
    window.draw(nText);
}

void DebugPanel::drawCenterCompass(sf::RenderWindow& window, const sf::Vector2f& playerPos, const sf::Vector2f& centerPos) {
    // Draw a second compass below the first
    float compassRadius = 32.f;
    sf::Vector2f compassCenter(position.x + compassRadius + 10.f, position.y + 80.f + compassRadius * 2.f + 20.f);

    // Draw compass circle
    sf::CircleShape circle(compassRadius);
    circle.setFillColor(sf::Color(0, 0, 0, 0));
    circle.setOutlineColor(sf::Color::White);
    circle.setOutlineThickness(2.f);
    circle.setOrigin(compassRadius, compassRadius);
    circle.setPosition(compassCenter);
    window.draw(circle);

    // Calculate angle from player to center
    sf::Vector2f toCenter = centerPos - playerPos;
    float angleRad = std::atan2(toCenter.y, toCenter.x);
    // No adjustment: 0 rad points right (east), PI/2 up, PI left, -PI/2 down

    sf::Vector2f needleEnd = compassCenter + sf::Vector2f(std::cos(angleRad), std::sin(angleRad)) * (compassRadius - 6.f);

    sf::Vertex needle[] = {
        sf::Vertex(compassCenter, sf::Color::Cyan),
        sf::Vertex(needleEnd, sf::Color::Cyan)
    };
    window.draw(needle, 2, sf::Lines);

    // Draw C label for "Center"
    sf::Text cText("C", font, fontSize);
    cText.setFillColor(sf::Color::Cyan);
    cText.setPosition(compassCenter.x - 8.f, compassCenter.y - compassRadius - 18.f);
    window.draw(cText);
}
