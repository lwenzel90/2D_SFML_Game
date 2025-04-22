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
    
    void createSlider(const std::string& name, float min, float max, float& value, 
                     const std::function<void(float)>& callback);
    void createPlayerControls(Player& player);
    void createAttackControls(Attack& attack);

private:
    struct Slider {
        std::string name;
        float min;
        float max;
        float* value;
        std::function<void(float)> callback;
        bool active;
        
        sf::Text label;
        sf::RectangleShape bar;
        sf::CircleShape handle;
        sf::Text valueText;
    };

    bool m_isOpen = false;
    std::unique_ptr<sf::RenderWindow> window;
    sf::Font font;
    sf::Text m_text; // Added missing text member
    std::vector<Slider> sliders;
    bool controlsCreated = false; // Add this flag
    
    const float padding = 20.f;
    const float sliderWidth = 200.f;
    const float sliderHeight = 4.f;
    const float sliderSpacing = 50.f;
};