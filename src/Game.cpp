#include "Game.hpp"
#include "Player.hpp"
#include "Attack.hpp"
#include "InputHandler.hpp"
#include "DebugPanel.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

Game::Game()
    : running(true), paused(false), pauseInputCooldown(0.f), player(400.f, 300.f), attack(), inputHandler(), attackToggle(false),
      pauseMenuInputCooldown(0.f), inSettingsMenu(false), settingsMenuSelectedIndex(0) // <-- add this member variable initialization
{
    // ...any other initialization...
}

void Game::run(sf::RenderWindow& window) {
    sf::Clock clock;
    sf::Font font;
    font.loadFromFile("../assets/arial.ttf"); // Make sure this font file exists in your project

    debugPanel.setFont(font);

    while (window.isOpen() && isRunning()) {
        handleWindowEvents(window);

        float deltaTime = clock.restart().asSeconds();

        inputHandler.update();

        // Update pause input cooldown timer
        if (pauseInputCooldown > 0.f) {
            pauseInputCooldown -= deltaTime;
            if (pauseInputCooldown < 0.f) pauseInputCooldown = 0.f;
        }
        if (pauseMenuInputCooldown > 0.f) {
            pauseMenuInputCooldown -= deltaTime;
            if (pauseMenuInputCooldown < 0.f) pauseMenuInputCooldown = 0.f;
        }

        // Pause logic with cooldown
        if (inputHandler.isPausePressed() && pauseInputCooldown == 0.f) {
            togglePause();
            pauseInputCooldown = 0.5f; // 500ms cooldown
        }

        if (paused) {
            handleInput(window); // pass window here
            if (inSettingsMenu) {
                renderSettingsMenu(window, font, settingsMenuItems, settingsMenuSelectedIndex);
            } else {
                renderPauseMenu(window, font, pauseMenuItems, pauseMenuIndex);
            }
            // Only display once per frame, so return here
            continue;
        }

        float rotationApplied = handleRotation(deltaTime);
        handleMovement(deltaTime);
        handleAttack(deltaTime, rotationApplied);

        // --- Debug panel update ---
        debugPanel.clear();
        debugPanel.addLine("Player Dir: " + std::to_string(player.getRotation()) + " deg");

        // Add player position relative to center of the screen
        sf::Vector2f playerPos = player.getPosition();
        sf::Vector2u winSize = window.getSize();
        sf::Vector2f center(winSize.x / 2.f, winSize.y / 2.f);
        sf::Vector2f rel = playerPos - center;
        debugPanel.addLine("Rel to Center: (" + std::to_string(rel.x) + ", " + std::to_string(rel.y) + ")");

        render(window);
    }
}

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
    player.applyMovementForce(force, deltaTime, player.getRotation());
    player.updateMovement(deltaTime, force);
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
    debugPanel.draw(window); // Draw debug panel text
    debugPanel.drawCompass(window, player.getRotation()); // Draw player direction compass

    // Draw center-pointing compass
    sf::Vector2f playerPos = player.getPosition();
    sf::Vector2u winSize = window.getSize();
    sf::Vector2f center(winSize.x / 2.f, winSize.y / 2.f);
    debugPanel.drawCenterCompass(window, playerPos, center);

    window.display();
}

bool Game::isRunning() const {
    return running;
}

void Game::stop() {
    running = false;
}

void Game::togglePause() {
    paused = !paused;
}

void Game::renderPauseMenu(sf::RenderWindow& window, sf::Font& font, const std::vector<std::string>& items, int selected) {
    window.clear(sf::Color(30, 30, 30, 220));

    sf::Text pausedText("PAUSED", font, 64);
    pausedText.setFillColor(sf::Color::White);
    pausedText.setStyle(sf::Text::Bold);
    pausedText.setPosition(200, 80);
    window.draw(pausedText);

    for (size_t i = 0; i < items.size(); ++i) {
        sf::Text item(items[i], font, 36);
        item.setPosition(220, 200 + i * 60);
        item.setFillColor(i == selected ? sf::Color::Yellow : sf::Color(200, 200, 200));
        window.draw(item);
    }
    window.display();
}

void Game::renderSettingsMenu(sf::RenderWindow& window, sf::Font& font, const std::vector<std::string>& items, int selected) {
    window.clear(sf::Color(30, 30, 30, 220));

    sf::Text settingsText("SETTINGS", font, 64);
    settingsText.setFillColor(sf::Color::White);
    settingsText.setStyle(sf::Text::Bold);
    settingsText.setPosition(200, 80);
    window.draw(settingsText);

    for (size_t i = 0; i < items.size(); ++i) {
        sf::Text item(items[i], font, 36);
        item.setPosition(220, 200 + i * 60);
        item.setFillColor(i == selected ? sf::Color::Yellow : sf::Color(200, 200, 200));
        window.draw(item);
    }
    window.display();
}

// Change signature to accept window reference
void Game::handleInput(sf::RenderWindow& window) {
    if (paused) {
        if (inSettingsMenu) {
            // Add input cooldown for settings menu navigation
            if (pauseMenuInputCooldown == 0.f) {
                if (inputHandler.isMenuUp()) {
                    settingsMenuSelectedIndex = (settingsMenuSelectedIndex + settingsMenuItems.size() - 1) % settingsMenuItems.size();
                    pauseMenuInputCooldown = 0.2f;
                } else if (inputHandler.isMenuDown()) {
                    settingsMenuSelectedIndex = (settingsMenuSelectedIndex + 1) % settingsMenuItems.size();
                    pauseMenuInputCooldown = 0.2f;
                } else if (inputHandler.isMenuSelect()) {
                    if (settingsMenuItems[settingsMenuSelectedIndex] == "Back") {
                        inSettingsMenu = false;
                        settingsMenuSelectedIndex = 0;
                    }
                    pauseMenuInputCooldown = 0.2f;
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    inSettingsMenu = false;
                    settingsMenuSelectedIndex = 0;
                    pauseMenuInputCooldown = 0.2f;
                }
            }
        } else {
            // Pause menu navigation
            if (pauseMenuInputCooldown == 0.f) {
                if (inputHandler.isMenuUp()) {
                    pauseMenuIndex = (pauseMenuIndex + pauseMenuItems.size() - 1) % pauseMenuItems.size();
                    pauseMenuInputCooldown = 0.2f; // 200ms delay for menu navigation
                } else if (inputHandler.isMenuDown()) {
                    pauseMenuIndex = (pauseMenuIndex + 1) % pauseMenuItems.size();
                    pauseMenuInputCooldown = 0.2f;
                } else if (inputHandler.isMenuSelect()) {
                    if (pauseMenuIndex == 0) { // Resume
                        togglePause();
                        pauseInputCooldown = 0.5f; // Prevent instant re-pause
                    } else if (pauseMenuIndex == 1) { // Settings
                        inSettingsMenu = true;
                        settingsMenuSelectedIndex = 0;
                    } else if (pauseMenuIndex == 2) { // Exit
                        stop();
                        window.close();
                    }
                    pauseMenuInputCooldown = 0.2f; // Delay after selection
                }
            }
        }
    }
}
