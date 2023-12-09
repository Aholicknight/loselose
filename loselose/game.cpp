#include "game.h"

Game::Game() : window(sf::VideoMode(800, 600), "Lose/Lose")
{
    // Load the spaceship texture
    if (!spaceshipTexture.loadFromFile("spaceship.png"))
        throw std::runtime_error("Failed to load spaceship.png");

    // Create the spaceship sprite
    spaceship.setTexture(spaceshipTexture);
    spaceship.setPosition(400, 500);
}

void Game::run()
{
    while (window.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::update()
{
    // Move the spaceship
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && spaceship.getPosition().x > 0)
        spaceship.move(-10, 0);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && spaceship.getPosition().x < window.getSize().x - spaceshipTexture.getSize().x)
        spaceship.move(10, 0);
}

void Game::render()
{
    window.clear();
    window.draw(spaceship);
    window.display();
}