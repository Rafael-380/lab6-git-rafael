#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(float fps, const std::string& textureFile) : fps(fps) {
    if (!texture.loadFromFile(textureFile)) {
        throw std::runtime_error("Failed to load texture");
    }
    setTexture(texture);
    frameDuration = sf::seconds(1.f / fps);
}

void AnimatedSprite::add_animation_frame(const sf::IntRect& frame) {
    frames.push_back(frame);
    if (frames.size() == 1) {
        setTextureRect(frames[0]);  // start with the first frame
    }
}

void AnimatedSprite::animate(sf::Time dt) {
    applyGravity(dt);

    if (frames.size() <= 1) return; // nothing to animate

    elapsedTime += dt;
    if (elapsedTime >= frameDuration) {
        elapsedTime -= frameDuration;
        // Skip first frame (idle) and loop from frame 1 to end
        currentFrame++;
        if (currentFrame >= frames.size()) {
            currentFrame = 1;
        }
        setTextureRect(frames[currentFrame]);
    }
}

void AnimatedSprite::applyGravity(sf::Time dt) {
    if (!onGround) {
        verticalSpeed += gravity * dt.asSeconds();
        move(0, verticalSpeed * dt.asSeconds());
    } else {
        verticalSpeed = 0;
    }
}

void AnimatedSprite::jump(float speed) {
    if (onGround) {
        verticalSpeed = -speed;
        onGround = false;
    }
}

float AnimatedSprite::getVerticalSpeed() const {
    return verticalSpeed;
}

void AnimatedSprite::setOnGround(bool val) {
    onGround = val;
    if (val) verticalSpeed = 0;
}

bool AnimatedSprite::isOnGround() const {
    return onGround;
}

void AnimatedSprite::resetToIdleFrame() {
    currentFrame = 0;
    setTextureRect(frames[0]);
    elapsedTime = sf::Time::Zero;
}

