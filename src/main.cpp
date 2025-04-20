#include <SFML/Graphics.hpp>
#include "Game.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Works!");
    Game game;
    game.run(window);
    return 0;
}