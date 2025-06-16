#include "AnimatedSprite.h"
#include <iostream>

AnimatedSprite::AnimatedSprite(int fps, const std::string& texturePath) {
    timePerFrame = 1.0f / static_cast<float>(fps);

    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Erro ao carregar textura: " << texturePath << "\n";
    }

    setTexture(texture);
}

void AnimatedSprite::add_animation_frame(const sf::IntRect& frameRect) {
    frames.push_back(frameRect);
    if (frames.size() == 1)
        setTextureRect(frames[0]);
}

void AnimatedSprite::animate(const sf::Time& elapsed) {
    float dt = elapsed.asSeconds();
    timeAccumulator += dt;

    if (frames.empty()) return;

    while (timeAccumulator >= timePerFrame) {
        timeAccumulator -= timePerFrame;
        currentFrame = (currentFrame + 1) % frames.size();
        setTextureRect(frames[currentFrame]);
    }

    applyGravity(elapsed);
    move(0, verticalSpeed * dt);
}

void AnimatedSprite::applyGravity(const sf::Time& elapsed) {
    if (!onGround) {
        verticalSpeed += gravity * elapsed.asSeconds();
    } else {
        verticalSpeed = 0.f;
    }
}

void AnimatedSprite::jump(float initialJumpSpeed) {
    if (onGround) {
        verticalSpeed = -initialJumpSpeed;
        onGround = false;
    }
}

bool AnimatedSprite::isOnGround() const {
    return onGround;
}

void AnimatedSprite::setOnGround(bool value) {
    onGround = value;
}

float AnimatedSprite::getVerticalSpeed() const {
    return verticalSpeed;
}

