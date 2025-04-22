#include "Game.hpp"
#include "Player.hpp"
#include "Attack.hpp"
#include "InputHandler.hpp"
#include "DebugPanel.hpp"


#include <SFML/Graphics.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <SFML/System/Clock.hpp>

#include <vector>
#include <string>
#include <iostream>

namespace {
    // Timing
    constexpr float PAUSE_INPUT_COOLDOWN_S = 0.5f;
    constexpr float MENU_INPUT_COOLDOWN_S = 0.2f;

    // Rotation
    constexpr float NORMAL_ROTATION_SPEED_DEG_S = 180.0f;
    constexpr float FAST_ROTATION_SPEED_DEG_S = 360.0f;

    // Movement
    constexpr float FORWARD_FORCE_UNIT = 1.0f;

    // Attack
    constexpr float ATTACK_ANGLE_ROTATION_FACTOR = 0.5f;

    // Menu Rendering
    const sf::Color MENU_BACKGROUND_COLOR = sf::Color(30, 30, 30, 220);
    const sf::Color MENU_ITEM_DEFAULT_COLOR = sf::Color(200, 200, 200);
    const sf::Color MENU_ITEM_SELECTED_COLOR = sf::Color::Yellow;
    constexpr unsigned int MENU_TITLE_FONT_SIZE = 64;
    constexpr unsigned int MENU_ITEM_FONT_SIZE = 36;
    constexpr float MENU_TITLE_POS_X = 200.f;
    constexpr float MENU_TITLE_POS_Y = 80.f;
    constexpr float MENU_ITEM_START_X = 220.f;
    constexpr float MENU_ITEM_START_Y = 200.f;
    constexpr float MENU_ITEM_SPACING_Y = 60.f;

    // File Paths
    const std::string FONT_PATH = "../assets/arial.ttf";

    // Fullscreen
    const sf::Vector2i FULLSCREEN_WINDOW_POSITION = {0, 0};
}

Game::Game()
    : running(true), paused(false), pauseInputCooldown(0.f), player(400.f, 300.f), attack(), inputHandler(), attackToggle(false),
      pauseMenuInputCooldown(0.f), inSettingsMenu(false), settingsMenuSelectedIndex(0)
{

}

void Game::run(sf::RenderWindow& window) {
    sf::Clock clock;
    sf::Font font;
    if (!font.loadFromFile(FONT_PATH)) {
        // Handle error: Font not loaded
        std::cerr << "Error loading font: " << FONT_PATH << std::endl;
        stop();
        return;
    }

    debugPanel.setFont(font);

    // Set the window title at startup
    window.setTitle(windowTitle);

    // Set initial screen size for attack boundaries
    attack.setScreenSize(window.getSize());

    // Set the initial view to match the window size
    sf::View view = window.getDefaultView();
    view.setSize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
    view.setCenter(window.getSize().x / 2.f, window.getSize().y / 2.f);
    window.setView(view);

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
        if (debugWindowToggleCooldown > 0.f) {
            debugWindowToggleCooldown -= deltaTime;
            if (debugWindowToggleCooldown < 0.f) debugWindowToggleCooldown = 0.f;
        }

        // Handle debug window toggle with cooldown
        if (inputHandler.isDebugWindowToggled() && debugWindowToggleCooldown == 0.f) {
            toggleDebugWindow();
            debugWindowToggleCooldown = PAUSE_INPUT_COOLDOWN_S;
        }

        // Always update debug window if it's open
        updateDebugWindow();

        // Pause logic with cooldown
        if (inputHandler.isPausePressed() && pauseInputCooldown == 0.f) {
            togglePause();
            pauseInputCooldown = PAUSE_INPUT_COOLDOWN_S; // Use constant
        }

        if (paused) {
            handleInput(window); // pass window here
            if (inSettingsMenu) {
                renderSettingsMenu(window, font, settingsMenuItems, settingsMenuSelectedIndex);
            } else {
                renderPauseMenu(window, font, pauseMenuItems, pauseMenuIndex);
            }
            // Only display once per frame, so continue
            continue;
        }

        float rotationApplied = handleRotation(deltaTime);
        handleMovement(deltaTime);
        handleAttack(window, deltaTime, rotationApplied); // Pass window here

        // --- Debug panel update ---
        debugPanel.clear();
        debugPanel.addLine("Player Dir: " + std::to_string(player.getRotation()) + " deg");

        // Add player position relative to center of the screen
        sf::Vector2f playerPos = player.getPosition();
        sf::Vector2u winSize = window.getSize();
        sf::Vector2f center(winSize.x / 2.f, winSize.y / 2.f);
        sf::Vector2f rel = playerPos - center;
        debugPanel.addLine("Rel to Center: (" + std::to_string(rel.x) + ", " + std::to_string(rel.y) + ")");
        debugPanel.addLine("Press F1 for Debug Window");

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
        rotationSpeed = -FAST_ROTATION_SPEED_DEG_S; // Use constant
    } else if (inputHandler.isRotateLeft()) {
        rotationSpeed = -NORMAL_ROTATION_SPEED_DEG_S; // Use constant
    } else if (inputHandler.isFastRotateRight()) {
        rotationSpeed = FAST_ROTATION_SPEED_DEG_S; // Use constant
    } else if (inputHandler.isRotateRight()) {
        rotationSpeed = NORMAL_ROTATION_SPEED_DEG_S; // Use constant
    }
    player.handleRotation(rotationSpeed, deltaTime);
    return rotationSpeed * deltaTime;
}

void Game::handleMovement(float deltaTime) {
    float force = 0.f;
    if (inputHandler.isMoveForward()) {
        force = FORWARD_FORCE_UNIT; // Use constant
    }
    player.applyMovementForce(force, deltaTime, player.getRotation());
    player.updateMovement(deltaTime, force);
}

// Add this getter for window size
sf::Vector2u Game::getWindowSize(const sf::RenderWindow& window) const {
    return window.getSize();
}

void Game::handleAttack(sf::RenderWindow& window, float deltaTime, float rotationApplied) {
    sf::Vector2f playerPos = player.getPosition();
    sf::Vector2u winSize = getWindowSize(window); // Pass window here

    if (playerPos.x > 0 && playerPos.x < winSize.x && playerPos.y > 0 && playerPos.y < winSize.y) {
        if (inputHandler.isAttackToggled()) {
            attackToggle = !attackToggle;
        }
        float attackAngle = player.getRotation() + (rotationApplied * ATTACK_ANGLE_ROTATION_FACTOR); // Use constant
        attack.update(
            deltaTime,
            player.getPosition(),
            attackAngle,
            attackToggle
        );
    }

}

void Game::render(sf::RenderWindow& window) {
    // Ensure the view is updated each frame (handles resize/fullscreen)
    sf::View view = window.getView(); // Get the current view (might have been changed)
    // Or reset to default scaled view:
    // sf::View view = window.getDefaultView();
    view.setSize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
    view.setCenter(window.getSize().x / 2.f, window.getSize().y / 2.f);
    window.setView(view);

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
    window.clear(MENU_BACKGROUND_COLOR); // Use constant

    sf::Text pausedText("PAUSED", font, MENU_TITLE_FONT_SIZE); // Use constant
    pausedText.setFillColor(sf::Color::White);
    pausedText.setStyle(sf::Text::Bold);
    pausedText.setPosition(MENU_TITLE_POS_X, MENU_TITLE_POS_Y); // Use constants
    window.draw(pausedText);

    for (size_t i = 0; i < items.size(); ++i) {
        sf::Text item(items[i], font, MENU_ITEM_FONT_SIZE); // Use constant
        item.setPosition(MENU_ITEM_START_X, MENU_ITEM_START_Y + i * MENU_ITEM_SPACING_Y); // Use constants
        item.setFillColor(i == selected ? MENU_ITEM_SELECTED_COLOR : MENU_ITEM_DEFAULT_COLOR); // Use constants
        window.draw(item);
    }
    window.display();
}

void Game::renderSettingsMenu(sf::RenderWindow& window, sf::Font& font, const std::vector<std::string>& items, int selected) {
    window.clear(MENU_BACKGROUND_COLOR); // Use constant

    sf::Text settingsText("SETTINGS", font, MENU_TITLE_FONT_SIZE); // Use constant
    settingsText.setFillColor(sf::Color::White);
    settingsText.setStyle(sf::Text::Bold);
    settingsText.setPosition(MENU_TITLE_POS_X, MENU_TITLE_POS_Y); // Use constants
    window.draw(settingsText);

    for (size_t i = 0; i < items.size(); ++i) {
        sf::Text item(items[i], font, MENU_ITEM_FONT_SIZE); // Use constant
        item.setPosition(MENU_ITEM_START_X, MENU_ITEM_START_Y + i * MENU_ITEM_SPACING_Y); // Use constants
        item.setFillColor(i == selected ? MENU_ITEM_SELECTED_COLOR : MENU_ITEM_DEFAULT_COLOR); // Use constants
        window.draw(item);
    }
    window.display();
}

// Change signature to accept window reference
void Game::handleInput(sf::RenderWindow& window) {
    if (paused) {
        if (inSettingsMenu) {
            if (pauseMenuInputCooldown == 0.f) {
                if (inputHandler.isMenuUp()) {
                    settingsMenuSelectedIndex = (settingsMenuSelectedIndex + settingsMenuItems.size() - 1) % settingsMenuItems.size();
                    pauseMenuInputCooldown = MENU_INPUT_COOLDOWN_S; // Use constant
                } else if (inputHandler.isMenuDown()) {
                    settingsMenuSelectedIndex = (settingsMenuSelectedIndex + 1) % settingsMenuItems.size();
                    pauseMenuInputCooldown = MENU_INPUT_COOLDOWN_S; // Use constant
                } else if (inputHandler.isMenuSelect()) {
                    if (settingsMenuItems[settingsMenuSelectedIndex] == "Fullscreen") {
                        applyFullscreen(window, false);
                    } else if (settingsMenuItems[settingsMenuSelectedIndex] == "Windowed Fullscreen") {
                        applyFullscreen(window, true);
                    } else if (settingsMenuItems[settingsMenuSelectedIndex] == "Back") {
                        inSettingsMenu = false;
                        settingsMenuSelectedIndex = 0;
                    }
                    pauseMenuInputCooldown = MENU_INPUT_COOLDOWN_S; // Use constant
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    inSettingsMenu = false;
                    settingsMenuSelectedIndex = 0;
                    pauseMenuInputCooldown = MENU_INPUT_COOLDOWN_S; // Use constant
                }
            }
        } else {
            // Pause menu navigation
            if (pauseMenuInputCooldown == 0.f) {
                if (inputHandler.isMenuUp()) {
                    pauseMenuIndex = (pauseMenuIndex + pauseMenuItems.size() - 1) % pauseMenuItems.size();
                    pauseMenuInputCooldown = MENU_INPUT_COOLDOWN_S; // 200ms delay for menu navigation
                } else if (inputHandler.isMenuDown()) {
                    pauseMenuIndex = (pauseMenuIndex + 1) % pauseMenuItems.size();
                    pauseMenuInputCooldown = MENU_INPUT_COOLDOWN_S; // Use constant
                } else if (inputHandler.isMenuSelect()) {
                    if (pauseMenuIndex == 0) { // Resume
                        togglePause();
                        pauseInputCooldown = PAUSE_INPUT_COOLDOWN_S; // Prevent instant re-pause
                    } else if (pauseMenuIndex == 1) { // Settings
                        inSettingsMenu = true;
                        settingsMenuSelectedIndex = 0;
                    } else if (pauseMenuIndex == 2) { // Exit
                        stop();
                        window.close();
                    }
                    pauseMenuInputCooldown = MENU_INPUT_COOLDOWN_S; // Use constant
                }
            }
        }
    }
}

// Helper to get the monitor the window is mostly on and resize accordingly
void Game::applyFullscreen(sf::RenderWindow& window, bool borderless) {
    // Get all available video modes (for all monitors)
    auto modes = sf::VideoMode::getFullscreenModes();
    if (modes.empty()) return;

    // Get current window position and size
    auto winPos = window.getPosition();
    auto winSize = window.getSize();

    // Find the monitor the window is mostly on (approximate)
    sf::VideoMode bestMode = modes[0];
    for (const auto& mode : modes) {
        // For simplicity, pick the mode with the largest area that fits the current window position
        if (winPos.x >= 0 && winPos.y >= 0 &&
            winPos.x < (int)mode.width && winPos.y < (int)mode.height) {
            bestMode = mode;
            break;
        }
    }

    // Use stored windowTitle instead of window.getTitle()
    window.create(
        sf::VideoMode(bestMode.width, bestMode.height, bestMode.bitsPerPixel),
        windowTitle,
        borderless ? sf::Style::None : sf::Style::Fullscreen
    );
    window.setPosition(FULLSCREEN_WINDOW_POSITION); // Use constant

    // Update attack boundaries after changing screen mode
    attack.setScreenSize(window.getSize());

    // Update the view to match the new fullscreen size
    sf::View view = window.getDefaultView();
    view.setSize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
    view.setCenter(window.getSize().x / 2.f, window.getSize().y / 2.f);
    window.setView(view);
}

// Add getters for Player and Attack references
Player& Game::getPlayer() {
    return player;
}

Attack& Game::getAttack() {
    return attack;
}

// Add these functions somewhere after the togglePause function in Game.cpp

void Game::toggleDebugWindow() {
    if (debugPanel.hasDebugWindow()) {
        debugPanel.closeDebugWindow();
    } else {
        debugPanel.createDebugWindow();
    }
}

void Game::updateDebugWindow() {
    if (debugPanel.hasDebugWindow()) {
        debugPanel.updateDebugWindow(player, attack);
    }
}
