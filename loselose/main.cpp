#include <SFML/Graphics.hpp>
#include <vector>
#include "enemy.h"
#include "bullet.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "lose/lose");
    window.setFramerateLimit(60);

    // Create a clock to measure frame time
    sf::Clock frameClock;

    // Load the Consolas font
    sf::Font consolasFont;
    if (!consolasFont.loadFromFile("C:\\Windows\\Fonts\\consola.ttf")) {
        return EXIT_FAILURE;
    }

    sf::Texture spaceshipTexture;
    if (!spaceshipTexture.loadFromFile("spaceship.png"))
        return EXIT_FAILURE;

    sf::Sprite spaceship(spaceshipTexture);
    spaceship.setPosition(400, 500);
    spaceship.setScale(0.2, 0.2);

    std::vector<Bullet> bullets;
    sf::Clock bulletClock;

    std::vector<Enemy> enemies;
    sf::Clock enemySpawnClock;

    struct TimedText {
        sf::Text text;
        float lifetime;  // Time in seconds for how long to display the text
    };

    std::vector<TimedText> timedTexts;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        // Draw the spaceship
        window.draw(spaceship);

        // Update and draw bullets
        for (auto& bullet : bullets)
        {
            bullet.update();
            window.draw(bullet.getSprite());
        }

        // Check for bullet-enemy collisions
        for (auto it = bullets.begin(); it != bullets.end(); ) {
            bool bulletHit = false;
            for (auto eit = enemies.begin(); eit != enemies.end(); ) {
                if (eit->isHit(it->getPosition())) {
                    eit->displayFileName(); // Display the file name
                    TimedText tt;
                    tt.text = eit->getFileNameText();
                    tt.lifetime = 2.0f; // Display text for 2 seconds
                    timedTexts.push_back(tt);

                    eit = enemies.erase(eit); // Remove enemy
                    bulletHit = true;
                    break; // Stop checking this bullet against other enemies
                }
                else {
                    ++eit;
                }
            }
            if (bulletHit) {
                it = bullets.erase(it); // Remove bullet
            }
            else {
                ++it;
            }
        }

        float frameTime = frameClock.restart().asSeconds(); // Get frame time and restart clock

        // Update and draw timed texts
        for (auto& tt : timedTexts) {
            tt.lifetime -= frameTime; // Decrease the lifetime
            if (tt.lifetime > 0) {
                window.draw(tt.text); // Draw the text if it's still "alive"
            }
        }

        // Remove expired texts
        timedTexts.erase(std::remove_if(timedTexts.begin(), timedTexts.end(),
            [](const TimedText& t) { return t.lifetime <= 0; }),
            timedTexts.end());


        // Remove bullets that are off the screen
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return b.getPosition().top < 0; }), bullets.end());

        // Update and draw enemies
        for (auto& enemy : enemies) {
            enemy.update();
            window.draw(enemy.getSprite());
            if (enemy.shouldDisplayFileName()) {
                window.draw(enemy.getFileNameText());
            }
        }

        // Spawn new enemies periodically
        if (enemySpawnClock.getElapsedTime().asSeconds() > 2) // Spawn every 2 seconds
        {
            Enemy newEnemy(consolasFont);
            newEnemy.Spawn(window.getSize().x);
            enemies.push_back(newEnemy);

            enemySpawnClock.restart();
        }

        window.display();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && spaceship.getPosition().x > 0)
            spaceship.move(-10, 0);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && spaceship.getPosition().x < window.getSize().x - spaceshipTexture.getSize().x)
            spaceship.move(10, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if (bulletClock.getElapsedTime().asSeconds() > 0.6) // 0.6 second delay
            {
                // Create a new bullet and add it to the vector
                Bullet bullet;
                bullet.fire(spaceship.getPosition().x + spaceship.getGlobalBounds().width / 2, spaceship.getPosition().y);
                bullets.push_back(bullet);

                bulletClock.restart(); // Reset the clock after firing
            }
        }
    }


    return EXIT_SUCCESS;
}
