#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class AnimatedSprite : public sf::Sprite {
public:
    AnimatedSprite(float fps, const std::string& textureFile);

    void add_animation_frame(const sf::IntRect& frame);
    void animate(sf::Time dt);
    void applyGravity(sf::Time dt);

    void jump(float speed);
    float getVerticalSpeed() const;
    void setOnGround(bool val);
    bool isOnGround() const;

    void resetToIdleFrame();

private:
    std::vector<sf::IntRect> frames;
    sf::Texture texture;

    float verticalSpeed = 0.f;
    const float gravity = 980.f; // pixels/s²
    bool onGround = false;

    float fps;
    sf::Time frameDuration;
    sf::Time elapsedTime;

    size_t currentFrame = 0;
};
