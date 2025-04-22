#include "DebugWindow.hpp"
#include "Player.hpp" // Include necessary headers for update method parameters
#include "Attack.hpp"

#include <SFML/Window/Event.hpp>
#include <iostream> // For error messages
#include <string>   // For std::to_string
#include <sstream>  // For formatting text
#include <iomanip>  // For std::fixed, std::setprecision
// #include <cmath> // Keep if needed for other things, remove if only for sliders

namespace {
    const std::string FONT_PATH = "../assets/arial.ttf";
    const unsigned int WINDOW_WIDTH = 300;
    const unsigned int WINDOW_HEIGHT = 400; // Adjust as needed without sliders
    const unsigned int FONT_SIZE = 14;
    const float TEXT_PADDING = 10.f; // Padding for text
}

DebugWindow::DebugWindow() : m_isOpen(false) {}

DebugWindow::~DebugWindow() {
    close(); // Ensure window is closed and resources released
}

void DebugWindow::loadFont() {
    if (!font.loadFromFile(FONT_PATH)) {
        std::cerr << "Error loading font: " << FONT_PATH << std::endl;
        // Handle font loading failure (e.g., use a default font or exit)
    }
    m_text.setFont(font);
    m_text.setCharacterSize(FONT_SIZE);
    m_text.setFillColor(sf::Color::White);
    m_text.setPosition(TEXT_PADDING, TEXT_PADDING);
}

void DebugWindow::create() {
    if (m_isOpen) return;

    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Debug Controls", sf::Style::Titlebar | sf::Style::Close);
    window->setVerticalSyncEnabled(true); // Optional: Sync framerate
    m_isOpen = true;
    loadFont(); // Load font when window is created
}

bool DebugWindow::isOpen() const {
    return m_isOpen && window && window->isOpen();
}

void DebugWindow::close() {
    if (window) {
        window->close();
    }
    m_isOpen = false;
    // Release resources if needed, unique_ptr handles window memory
}

void DebugWindow::update(Player& player, Attack& attack) {
    if (!isOpen()) return;

    std::string debugInfo = "Player Pos: (" + std::to_string(static_cast<int>(player.getPosition().x)) + ", " + std::to_string(static_cast<int>(player.getPosition().y)) + ")\n";
    debugInfo += "Attack Active: " + std::string(attack.isAttackActive() ? "Yes" : "No") + "\n";
    debugInfo += "Projectiles: " + std::to_string(attack.getProjectiles().size()) + "\n";

    m_text.setString(debugInfo);

    // Process events before rendering
    processEvents(); 

    // Render the window content (now just text)
    render(); 
}

void DebugWindow::processEvents() {
    if (!window) return;
    
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            close();
        }
    }
}



void DebugWindow::render() {
    if (!window || !isOpen()) return;
    
    window->clear(sf::Color(50, 50, 50));

    // Draw the debug text 
    window->draw(m_text); 
    
    
    window->display();
}