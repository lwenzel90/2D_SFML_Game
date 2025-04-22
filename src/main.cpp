#include "Game.hpp"

#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Asteroids Skeleton");
    Game game;
    game.run(window);
    return 0;
}