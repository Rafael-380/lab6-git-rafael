#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <AnimatedSprite.h>
#include <cmath>


int main() {
    //sf::RenderWindow window(sf::VideoMode(1024, 768), "Platformer");
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(desktop.width, desktop.height), "Platformer", sf::Style::Titlebar | sf::Style::Close);

    sf::Clock clock;

    // Criar personagem
    AnimatedSprite hero(10, "character.png");
    hero.setPosition(100, 500);
    hero.setScale(2,2);

    // Adicionar frames
    hero.add_animation_frame(sf::IntRect(015, 0, 25, 37));
    hero.add_animation_frame(sf::IntRect(165, 0, 25, 37));
    hero.add_animation_frame(sf::IntRect(215, 0, 25, 37));
    hero.add_animation_frame(sf::IntRect(265, 0, 25, 37));
    hero.add_animation_frame(sf::IntRect(315, 0, 25, 37));
    hero.add_animation_frame(sf::IntRect(365, 0, 25, 37));
    hero.add_animation_frame(sf::IntRect(415, 0, 25, 37));

    // Plataforma
    /*sf::RectangleShape platform(sf::Vector2f(300, 20));
    platform.setPosition(100, 550);
    platform.setFillColor(sf::Color::Green);
*/

    // Plataformas
    std::vector<sf::RectangleShape> platforms;

    sf::RectangleShape platform1(sf::Vector2f(300, 20));
    platform1.setPosition(100, 550);
    platform1.setFillColor(sf::Color::Green);
    platforms.push_back(platform1);

    sf::RectangleShape platform2(sf::Vector2f(250, 20));
    platform2.setPosition(450, 600);
    platform2.setFillColor(sf::Color::Green);
    platforms.push_back(platform2);

    sf::RectangleShape platform3(sf::Vector2f(200, 20));
    platform3.setPosition(750, 500);
    platform3.setFillColor(sf::Color::Green);
    platforms.push_back(platform3);

    sf::RectangleShape platform4(sf::Vector2f(350, 20));
    platform4.setPosition(300, 400);
    platform4.setFillColor(sf::Color::Green);
    platforms.push_back(platform4);


    //ceu
    sf::Texture skyTexture;
    if (!skyTexture.loadFromFile("sky.png")) {
        std::cerr << "Erro ao carregar sky.png\n";
    }
    sf::Sprite skySprite(skyTexture);

    // Ajustar escala do sky para cobrir a largura da janela
    float skyX = window.getSize().x / float(skyTexture.getSize().x);
    float skyY = (window.getSize().y / 2) / float(skyTexture.getSize().y); // 1/2 da altura do ecrã
    skySprite.setScale(skyX, skyY);
    skySprite.setPosition(0, 0);

    //Mar
    sf::Texture seaTexture;
    if (!seaTexture.loadFromFile("sea.png")) {
        std::cerr << "Erro ao carregar sea.png\n";
    }
    sf::Sprite seaSprite(seaTexture);

    float seaX = window.getSize().x / float(seaTexture.getSize().x);
    float seaY = (window.getSize().y / 2) / float(seaTexture.getSize().y); // 1/2 da altura do ecrã
    seaSprite.setScale(seaX, seaY);
    seaSprite.setPosition(0, window.getSize().y / 2);

    //Nuvens
    sf::Texture cloudsTexture;
    if (!cloudsTexture.loadFromFile("clouds.png")) {
        std::cerr << "Erro ao carregar clouds.png\n";
        // lidar com erro, se quiseres
    }

    float windowWidth = window.getSize().x;
    float cloudWidth = static_cast<float>(cloudsTexture.getSize().x);
    int numClouds = static_cast<int>(std::ceil(windowWidth / cloudWidth));

    std::vector<sf::Sprite> cloudSprites;
    for (int i = 0; i < numClouds; ++i) {
        sf::Sprite cloud(cloudsTexture);

        // A base da nuvem deve ficar na metade do ecrã,
        // ou seja, a posição Y do topo da nuvem é:
        // (altura metade do ecrã) - altura da nuvem
        float cloudBaseY = window.getSize().y / 2.f;
        float cloudPosY = cloudBaseY - static_cast<float>(cloudsTexture.getSize().y);

        cloud.setPosition(i * cloudWidth, cloudPosY);
        cloudSprites.push_back(cloud);
    }


    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        bool movingLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        bool movingRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
        bool moving = movingLeft || movingRight;

        if (movingLeft) {
            hero.move(-300.f * elapsed.asSeconds(), 0);
            hero.setScale(-2.f, 2.f);
            hero.setOrigin(hero.getGlobalBounds().width, 0);
        } else if (movingRight) {
            hero.move(300.f * elapsed.asSeconds(), 0);
            hero.setScale(2.f, 2.f);
            hero.setOrigin(0.f, 0.f);
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            hero.jump(500.f);
        }

        if (moving) {
            // Se está a andar (esquerda ou direita), anima sempre
            hero.animate(elapsed);
        } else {
            // Se não está a andar
            if (hero.isOnGround()) {
                // Parado no chão: mostra o frame parado
                hero.resetToIdleFrame();
            } else {
                // Parado no ar: mantém o frame parado, sem animar
                hero.resetToIdleFrame();
            }
            hero.applyGravity(elapsed);  // Aplica gravidade sempre
        }

        // Detecção de colisão com plataformas
        bool onAnyPlatform = false;
        sf::FloatRect heroBounds = hero.getGlobalBounds();

        for (const auto& plat : platforms) {
            sf::FloatRect platformBounds = plat.getGlobalBounds();

            if (heroBounds.intersects(platformBounds) && hero.getVerticalSpeed() >= 0) {
                hero.setPosition(hero.getPosition().x, plat.getPosition().y - heroBounds.height);
                onAnyPlatform = true;
                break;  // Assume que está em cima de uma plataforma só
            }
        }

        hero.setOnGround(onAnyPlatform);


        // Limitar o movimento do hero para dentro da janela
        //sf::FloatRect heroBounds = hero.getGlobalBounds();
        sf::Vector2f pos = hero.getPosition();

        float leftBound = 0.f;
        float rightBound = window.getSize().x;
        float topBound = 0.f;
        float bottomBound = window.getSize().y;

        if (heroBounds.left < leftBound) {
            pos.x += (leftBound - heroBounds.left);
        }
        if (heroBounds.left + heroBounds.width > rightBound) {
            pos.x -= (heroBounds.left + heroBounds.width - rightBound);
        }

        if (heroBounds.top < topBound) {
            pos.y += (topBound - heroBounds.top);
        }
        if (heroBounds.top + heroBounds.height > bottomBound) {
            pos.y -= (heroBounds.top + heroBounds.height - bottomBound);
        }

        hero.setPosition(pos);


        window.clear();
        window.draw(skySprite);
        for (const auto& cloud : cloudSprites) {
            window.draw(cloud);
        }
        window.draw(seaSprite);
        //window.draw(platform);
        for (const auto& plat : platforms) {
            window.draw(plat);
        }
        window.draw(hero);
        window.display();
    }

    return 0;
}

