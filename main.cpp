#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <AnimatedSprite.h>


int main() {
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Platformer");

    sf::Clock clock;

    // Criar personagem
    AnimatedSprite hero(10, "character.png");
    hero.setPosition(100, 500);
    hero.setScale(2,2);

    // Adicionar frames
    hero.add_animation_frame(sf::IntRect(15, 0, 20, 37));
    hero.add_animation_frame(sf::IntRect(165, 0, 20, 37));
    hero.add_animation_frame(sf::IntRect(215, 0, 20, 37));
    hero.add_animation_frame(sf::IntRect(265, 0, 20, 37));
    hero.add_animation_frame(sf::IntRect(315, 0, 20, 37));
    hero.add_animation_frame(sf::IntRect(365, 0, 20, 37));
    hero.add_animation_frame(sf::IntRect(415, 0, 20, 37));

    // Plataforma
    sf::RectangleShape platform(sf::Vector2f(300, 20));
    platform.setPosition(100, 550);
    platform.setFillColor(sf::Color::Green);

    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        bool moving = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            hero.move(-200.f * elapsed.asSeconds(), 0);
            hero.setScale(-2.f, 2.f);  // espelha horizontalmente
            hero.setOrigin(hero.getGlobalBounds().width, 0);
            moving = true;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            hero.move(200.f * elapsed.asSeconds(), 0);
            hero.setScale(2.f, 2.f);
            hero.setOrigin(0.f, 0.f);
            moving = true;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            hero.jump(400.f);
        }

        if (moving || !hero.isOnGround()) {
            hero.animate(elapsed);
        } else {
            hero.resetToIdleFrame();
            hero.applyGravity(elapsed);
        }


        // Detecção de colisão com a plataforma
        sf::FloatRect heroBounds = hero.getGlobalBounds();
        sf::FloatRect platformBounds = platform.getGlobalBounds();

        if (heroBounds.intersects(platformBounds) && hero.getVerticalSpeed() >= 0) {
            hero.setPosition(hero.getPosition().x, platform.getPosition().y - heroBounds.height);
            hero.setOnGround(true);
        } else {
            hero.setOnGround(false);
        }

        window.clear(sf::Color::Black);
        window.draw(platform);
        window.draw(hero);
        window.display();
    }

    return 0;
}

