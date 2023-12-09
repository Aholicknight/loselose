#include "player.h"

Player::Player()
{
    // Load the player texture
    if (!playerTexture.loadFromFile("player.png"))
        throw std::runtime_error("Failed to load player.png");

    // Create the player sprite
    playerSprite.setTexture(playerTexture);

    // Set the initial position of the player
    playerSprite.setPosition(400, 500);

    // Set the player size to 50x50
    playerSprite.setScale(0.5, 0.5);
}

void Player::moveLeft()
{
    playerSprite.move(-10, 0);
}

void Player::moveRight()
{
    playerSprite.move(10, 0);
}

sf::FloatRect Player::getPosition()
{
    // Get the position of the player
    return playerSprite.getGlobalBounds();
}

sf::Sprite Player::getSprite()
{
    // Get the sprite of the player
    return playerSprite;
}