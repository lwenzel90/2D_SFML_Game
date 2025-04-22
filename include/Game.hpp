#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"
#include "Attack.hpp"
#include "InputHandler.hpp"
#include "DebugPanel.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

#include <vector>
#include <string>

class Game {
public:
    Game();
    void run(sf::RenderWindow& window);
    bool isRunning() const;
    void stop();
    void togglePause();
    void renderPauseMenu(sf::RenderWindow& window, sf::Font& font, const std::vector<std::string>& items, int selected);
    void renderSettingsMenu(sf::RenderWindow& window, sf::Font& font, const std::vector<std::string>& items, int selected);
    void handleInput(sf::RenderWindow& window);
    sf::Vector2u getWindowSize(const sf::RenderWindow& window) const;
    void applyFullscreen(sf::RenderWindow& window, bool borderless);

    // Add getters for Player and Attack references
    Player& getPlayer();
    Attack& getAttack();
    
    // Debug window control
    void toggleDebugWindow();

private:
    void handleWindowEvents(sf::RenderWindow& window);
    float handleRotation(float deltaTime);
    void handleMovement(float deltaTime);
    void handleAttack(sf::RenderWindow& window, float deltaTime, float rotationApplied);
    void render(sf::RenderWindow& window);
    void updateDebugWindow();

    bool running;
    Player player;
    Attack attack;
    InputHandler inputHandler;
    bool attackToggle;
    bool paused;
    float pauseInputCooldown;
    float pauseMenuInputCooldown; // Cooldown for pause menu input
    DebugPanel debugPanel;
    bool inSettingsMenu; // Flag to check if in settings menu
    int settingsMenuSelectedIndex; // Index for settings menu selection
    std::vector<std::string> pauseMenuItems = {"Resume", "Settings", "Exit"};
    std::vector<std::string> settingsMenuItems = {
        "Fullscreen",
        "Windowed Fullscreen",
        "Back"
    };
    int pauseMenuIndex = 0; // Index for pause menu selection
    std::string windowTitle = "2D SFML Game"; // Store window title here
    
    // Debug window controls
    float debugWindowToggleCooldown = 0.0f;
};

#endif
