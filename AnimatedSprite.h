#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class AnimatedSprite : public sf::Sprite {
public:
    AnimatedSprite(int fps, const std::string& texturePath);

    void add_animation_frame(const sf::IntRect& frameRect);
    void animate(const sf::Time& elapsed);

    // Física
    void applyGravity(const sf::Time& elapsed);
    void jump(float initialJumpSpeed);
    bool isOnGround() const;
    void setOnGround(bool onGround);

    float getVerticalSpeed() const;

private:
    std::vector<sf::IntRect> frames;
    float timePerFrame;
    float timeAccumulator = 0.f;
    size_t currentFrame = 0;

    // Física
    float verticalSpeed = 0.f;
    const float gravity = 1000.f; // pixels/second²
    bool onGround = false;

    sf::Texture texture;
};
