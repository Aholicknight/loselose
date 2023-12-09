#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player
{
public:
    Player();
    void moveLeft();
    void moveRight();
    sf::FloatRect getPosition();
    sf::Sprite getSprite();

private:
    sf::Texture playerTexture;
    sf::Sprite playerSprite;
};

#endif