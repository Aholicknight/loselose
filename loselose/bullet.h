#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>

class Bullet
{
public:
    Bullet();
    void fire(float startX, float startY);
    void update();
    sf::FloatRect getPosition() const;
    sf::Sprite getSprite();

private:
    sf::Texture bulletTexture;
    sf::Sprite bulletSprite;
};

#endif