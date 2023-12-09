#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <string>

class Enemy
{
public:
    Enemy(const sf::Font& font);
    Enemy();
    void update();
    void Spawn(int screenWidth);
    sf::Sprite getSprite() const;
    bool isHit(const sf::FloatRect& bulletBounds) const;
    void displayFileName(); // Method to handle file name display
    std::string getFileName() const; // Get the file name for rendering
    bool shouldDisplayFileName() const; // Check if the file name should be displayed
    sf::Text getFileNameText() const; // Get the text for rendering

private:
    sf::Font fileNameFont;
    sf::Texture enemyTexture;
    sf::Sprite enemySprite;
    sf::Text fileNameText;
    std::string fileName;
    bool displayFileNameFlag = false;
};

#endif
