#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <AnimatedSprite.h>


int main() {
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Platformer");

    sf::Clock clock;

    // Criar personagem
    AnimatedSprite hero(10, "Character/character.png");
    hero.setPosition(100, 500);

    // Adicionar frames
    hero.add_animation_frame(sf::IntRect(150, 0, 50, 37));
    hero.add_animation_frame(sf::IntRect(200, 0, 50, 37));
    hero.add_animation_frame(sf::IntRect(250, 0, 50, 37));
    hero.add_animation_frame(sf::IntRect(300, 0, 50, 37));
    hero.add_animation_frame(sf::IntRect(350, 0, 50, 37));
    hero.add_animation_frame(sf::IntRect(400, 0, 50, 37));

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

        // Movimento lateral
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            hero.move(-200.f * elapsed.asSeconds(), 0);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            hero.move(200.f * elapsed.asSeconds(), 0);
        }

        // Salto
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            hero.jump(400.f);
        }

        // Chamar animação (inclui gravidade)
        hero.animate(elapsed);

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

