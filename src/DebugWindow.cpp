#include "DebugWindow.hpp"
#include "Player.hpp"
#include "Attack.hpp"
#include <sstream>
#include <iomanip>
#include <iostream> // For placeholder output
#include <string>   // For std::to_string

const std::string FONT_PATH = "../assets/arial.ttf";

DebugWindow::DebugWindow() : m_isOpen(false), controlsCreated(false) {
    // Initialization, if needed
    std::cout << "DebugWindow created." << std::endl;
}

DebugWindow::~DebugWindow() {
    // Cleanup, if needed
    std::cout << "DebugWindow destroyed." << std::endl;
    if (window && window->isOpen()) { // Use window->
        window->close();
    }
}

void DebugWindow::create() {
    if (!m_isOpen) {
        // Use make_unique to create the window
        window = std::make_unique<sf::RenderWindow>(sf::VideoMode(400, 300), "Debug Info");
        m_isOpen = true;
        // Load font or other resources if necessary
        if (!font.loadFromFile(FONT_PATH)) { // Use font.
             std::cerr << "Error loading font ARIAL.TTF for DebugWindow" << std::endl;
             m_isOpen = false; // Indicate failure
             window->close(); // Use window->
             return;
        }
         m_text.setFont(font); // Use font, m_text is correct
         m_text.setCharacterSize(12);
         m_text.setFillColor(sf::Color::White);
         m_text.setPosition(10, 10);
        std::cout << "DebugWindow opened." << std::endl;
    }
}

bool DebugWindow::isOpen() const {
    // Check SFML window state as well, in case it was closed externally
    return m_isOpen && window && window->isOpen(); // Use window->
}

void DebugWindow::close() {
    if (m_isOpen) {
        if(window) window->close(); // Use window->
        m_isOpen = false;
        std::cout << "DebugWindow closed." << std::endl;
    }
}

void DebugWindow::processEvents() {
    if (!window) return;
    
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            close();
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), 
                                    static_cast<float>(event.mouseButton.y));
                for (auto& slider : sliders) {
                    // Check if click is on the handle or the bar for easier interaction
                    sf::FloatRect interactBounds = slider.bar.getGlobalBounds();
                    // Make the clickable area slightly larger vertically
                    interactBounds.top -= slider.handle.getRadius(); 
                    interactBounds.height += 2 * slider.handle.getRadius();
                    
                    if (interactBounds.contains(mousePos)) {
                        slider.active = true;
                        // Immediately update value on click
                        float barX = slider.bar.getPosition().x;
                        float barWidth = slider.bar.getSize().x;
                        float normalizedPos = (mousePos.x - barX) / barWidth;
                        normalizedPos = std::max(0.0f, std::min(normalizedPos, 1.0f));
                        float newValue = slider.min + normalizedPos * (slider.max - slider.min);
                        
                        *slider.value = newValue; // Update the referenced value
                        
                        // Update handle position visually
                        slider.handle.setPosition(
                            barX + normalizedPos * barWidth,
                            slider.handle.getPosition().y // Y position doesn't change
                        );
                        
                        // Update value text
                        std::stringstream ss;
                        ss << std::fixed << std::setprecision(2) << newValue;
                        slider.valueText.setString(ss.str());
                        
                        // Trigger callback
                        if (slider.callback) {
                            slider.callback(newValue);
                        }
                    } else {
                        slider.active = false; // Deactivate others if click is outside
                    }
                }
            }
        } else if (event.type == sf::Event::MouseButtonReleased) {
             if (event.mouseButton.button == sf::Mouse::Left) {
                for (auto& slider : sliders) {
                    slider.active = false; // Deactivate all on release
                }
             }
        } else if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x), 
                                static_cast<float>(event.mouseMove.y));
            for (auto& slider : sliders) {
                if (slider.active) { // Only update if active (dragging)
                    float barX = slider.bar.getPosition().x;
                    float barWidth = slider.bar.getSize().x;
                    
                    // Calculate value based on mouse X relative to bar
                    float normalizedPos = (mousePos.x - barX) / barWidth;
                    normalizedPos = std::max(0.0f, std::min(normalizedPos, 1.0f)); // Clamp to [0, 1]
                    
                    float newValue = slider.min + normalizedPos * (slider.max - slider.min);
                    *slider.value = newValue; // Update the referenced value
                    
                    // Update handle position visually
                    slider.handle.setPosition(
                        barX + normalizedPos * barWidth,
                        slider.handle.getPosition().y // Y position doesn't change
                    );
                    
                    // Update value text
                    std::stringstream ss;
                    ss << std::fixed << std::setprecision(2) << newValue;
                    slider.valueText.setString(ss.str());
                    
                    // Trigger callback
                    if (slider.callback) {
                        slider.callback(newValue);
                    }
                }
            }
        }
    }
}

void DebugWindow::createPlayerControls(Player& player) {
    // Use getters/setters for maxSpeed instead of direct access to private 'speed'
    float currentMaxSpeed = player.getMaxSpeed();
    createSlider("Max Speed", 100.f, 1000.f, currentMaxSpeed,
                [&player](float val) { player.setMaxSpeed(val); });

    // Use getters/setters for scale
    float currentScale = player.getScale();
    createSlider("Scale", 0.5f, 2.0f, currentScale,
                [&player](float val) { player.setScale(val); });
}

void DebugWindow::createAttackControls(Attack& attack) {
    // Use getters/setters for projectileSpeed (mapping "Attack Speed" to this)
    float currentProjectileSpeed = attack.getProjectileSpeed();
    createSlider("Projectile Speed", 50.f, 1000.f, currentProjectileSpeed, // Adjusted range maybe
                [&attack](float val) { attack.setProjectileSpeed(val); });

    // Removed "Damage" slider as Attack class doesn't have a damage property
    // Consider adding slider for shootCooldown if needed:
    // float currentCooldown = attack.getShootCooldown();
    // createSlider("Shoot Cooldown", 0.1f, 2.0f, currentCooldown,
    //             [&attack](float val) { attack.setShootCooldown(val); });
}

void DebugWindow::createSlider(const std::string& name, float min, float max, 
                             float& value, const std::function<void(float)>& callback) {
    Slider slider;
    slider.name = name;
    slider.min = min;
    slider.max = max;
    slider.value = &value; // Store address of the actual value
    slider.callback = callback;
    slider.active = false;
    
    // Calculate Y position based on how many sliders already exist
    float y = padding + sliders.size() * sliderSpacing;
    
    slider.label.setFont(font);
    slider.label.setString(name);
    slider.label.setCharacterSize(12); // Smaller text
    slider.label.setFillColor(sf::Color::White);
    slider.label.setPosition(padding, y);
    
    // Position bar below the label
    float barY = y + 18.f; 
    slider.bar.setSize(sf::Vector2f(sliderWidth, sliderHeight));
    slider.bar.setFillColor(sf::Color(100, 100, 100));
    slider.bar.setPosition(padding, barY);
    
    // Calculate initial normalized value and handle position
    float normalizedValue = 0.f;
    if (max > min) { // Avoid division by zero
         normalizedValue = (*slider.value - min) / (max - min);
         normalizedValue = std::max(0.0f, std::min(normalizedValue, 1.0f)); // Clamp
    }
    slider.handle.setRadius(5.f); // Smaller handle
    slider.handle.setFillColor(sf::Color::White);
    slider.handle.setOrigin(slider.handle.getRadius(), slider.handle.getRadius()); // Center origin
    slider.handle.setPosition(
        padding + normalizedValue * sliderWidth, // Position based on initial value
        barY + sliderHeight / 2.f // Center vertically on the bar
    );
    
    // Position value text to the right of the bar
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << *slider.value; // Use initial value
    slider.valueText.setFont(font);
    slider.valueText.setString(ss.str());
    slider.valueText.setCharacterSize(12); // Smaller text
    slider.valueText.setFillColor(sf::Color::White);
    slider.valueText.setPosition(padding + sliderWidth + 10.f, y + 2.f); // Align with label top
    
    sliders.push_back(slider);
}

void DebugWindow::update(Player& player, Attack& attack) {
    if (!isOpen()) return;

    // Create controls only once after window is ready
    if (!controlsCreated) {
        createPlayerControls(player);
        createAttackControls(attack);
        controlsCreated = true;
    }

    // Prepare debug text
    std::string debugInfo = "Player Pos: (" + std::to_string(player.getPosition().x) + ", " + std::to_string(player.getPosition().y) + ")\n";
    // debugInfo += "Player State: " + player.getStateString() + "\n"; // Uncomment if Player has getStateString()
    debugInfo += "Attack Active: " + std::string(attack.isAttackActive() ? "Yes" : "No") + "\n"; // Use isAttackActive()
    debugInfo += "Projectiles: " + std::to_string(attack.getProjectiles().size()) + "\n";

    m_text.setString(debugInfo); // Update the text content

    // Render everything
    render(); 
}

void DebugWindow::render() {
    if (!window) return;
    
    window->clear(sf::Color(50, 50, 50));

    // Draw the debug text first
    window->draw(m_text); 
    
    // Calculate starting Y position for sliders below the text
    float startY = m_text.getPosition().y + m_text.getGlobalBounds().height + padding;
    
    // Draw sliders 
    for (size_t i = 0; i < sliders.size(); ++i) { 
        auto& slider = sliders[i]; // Use reference
        
        // Recalculate positions based on index and spacing
        float currentY = startY + i * sliderSpacing;
        float barY = currentY + 18.f; // Consistent with createSlider

        slider.label.setPosition(padding, currentY);
        slider.bar.setPosition(padding, barY);
        slider.valueText.setPosition(padding + sliderWidth + 10.f, currentY + 2.f); // Consistent

        // Update handle position based on the *current* value
        float normalizedValue = 0.f;
         if (slider.max > slider.min) { // Avoid division by zero
            normalizedValue = (*slider.value - slider.min) / (slider.max - slider.min);
            normalizedValue = std::max(0.0f, std::min(normalizedValue, 1.0f)); // Clamp
         }
        slider.handle.setPosition(
            padding + normalizedValue * sliderWidth,
            barY + sliderHeight / 2.f // Center vertically on the bar
        );

        // Update value text string in case it changed externally or via interaction
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << *slider.value;
        slider.valueText.setString(ss.str());

        // Draw components
        window->draw(slider.label);
        window->draw(slider.bar);
        window->draw(slider.handle);
        window->draw(slider.valueText);
    }
    
    window->display();
}