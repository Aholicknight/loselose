#include "bullet.h"

Bullet::Bullet()
{
    // Load the bullet texture
    if (!bulletTexture.loadFromFile("bullet.png"))
        throw std::runtime_error("Failed to load bullet.png");

    // Create the bullet sprite
    bulletSprite.setTexture(bulletTexture);

    // Set the bullet size to 10x10
    bulletSprite.setScale(0.1, 0.1);

    // Rotate the bullet 90 degrees
    bulletSprite.setRotation(90);

    // Set the initial position of the bullet off-screen
    bulletSprite.setPosition(-100, -100);
}

void Bullet::fire(float startX, float startY)
{
    // Create the bullet above the player
    bulletSprite.setPosition(startX, startY);
}

void Bullet::update()
{
    // Move the bullet up the screen
    bulletSprite.move(0, -10);

    // If the bullet has gone off the top of the screen, destroy it
    if (bulletSprite.getPosition().y < 0)
        bulletSprite.setPosition(-100, -100);
}

sf::FloatRect Bullet::getPosition() const
{
    return bulletSprite.getGlobalBounds();
}

sf::Sprite Bullet::getSprite()
{
    // Get the sprite of the bullet
    return bulletSprite;
}
