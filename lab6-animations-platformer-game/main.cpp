#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class CustomSprite : public sf::Sprite {
public:
    void setBounds(int left, int right, int top, int bottom) {
        bound_top = top;
        bound_bottom = bottom;
        bound_right = right;
        bound_left = left;
    }

    void moveInDirection(const sf::Time &elapsed, const std::vector<sf::Sprite> &obstacles) {
        float distance = m_speed * elapsed.asSeconds();

        sf::Vector2f directions[] = {
            sf::Vector2f(0, -distance), // Up
            sf::Vector2f(0, distance),  // Down
            sf::Vector2f(-distance, 0), // Left
            sf::Vector2f(distance, 0)   // Right
        };

        sf::Keyboard::Key keys[] = {
            sf::Keyboard::Up,
            sf::Keyboard::Down,
            sf::Keyboard::Left,
            sf::Keyboard::Right
        };

        for (int i = 0; i < 4; ++i) {
            if (sf::Keyboard::isKeyPressed(keys[i])) {
                sf::FloatRect futureBounds = getGlobalBounds();
                futureBounds.left += directions[i].x;
                futureBounds.top += directions[i].y;

                // Verify the bounds of the window
                if (futureBounds.left < bound_left ||
                    futureBounds.top < bound_top ||
                    futureBounds.left + futureBounds.width > bound_right ||
                    futureBounds.top + futureBounds.height > bound_bottom) {
                    continue;
                }

                // Verify the bounds of the walls
                bool collision = false;
                for (const auto& wall : obstacles) {
                    if (futureBounds.intersects(wall.getGlobalBounds())) {
                        collision = true;
                        break;
                    }
                }

                if (!collision) {
                    move(directions[i]);
                }
            }
        }
    }


private:
    float m_speed = 200.0f;
    int bound_top = 0;
    int bound_bottom = 0;
    int bound_left = 0;
    int bound_right = 0;
};

class AnimatedSprite : public sf::Sprite {
public:
    AnimatedSprite(int fps, const std::string &texturePath) : m_fps(fps), m_currentFrame(0), m_timePerFrame(sf::seconds(1.f / fps)), m_elapsedTime(sf::Time::Zero) {
        if (!m_texture.loadFromFile(texturePath)) {
            std::cerr << "Failed to load " << texturePath << std::endl;
        }
        setTexture(m_texture);
    }

    void add_animation_frame(const sf::IntRect &frameRect) {
        m_frames.push_back(frameRect);
        if (m_frames.size() == 1) {
            setTextureRect(m_frames[0]); // Set first frame initially
        }
    }

    void animate(const sf::Time &elapsed) {
        m_elapsedTime += elapsed;

        if (m_elapsedTime >= m_timePerFrame && !m_frames.empty()) {
            m_elapsedTime -= m_timePerFrame;
            m_currentFrame = (m_currentFrame + 1) % m_frames.size();
            setTextureRect(m_frames[m_currentFrame]);
        }
    }

private:
    sf::Texture m_texture;
    std::vector<sf::IntRect> m_frames;
    std::size_t m_currentFrame;
    sf::Time m_timePerFrame;
    sf::Time m_elapsedTime;
    int m_fps;
};


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Labirynth");
    sf::Clock clock;

    // Textures
    sf::Texture guy_tex;
    if (!guy_tex.loadFromFile("guy.png")) {
        std::cerr << "Failed to load guy.png" << std::endl;
        return 1;
    }

    sf::Texture grass_tex;
    if (!grass_tex.loadFromFile("grass.png")) {
        std::cerr << "Failed to load grass.png" << std::endl;
        return 1;
    }
    grass_tex.setRepeated(true);

    sf::Texture wall_tex;
    if (!wall_tex.loadFromFile("wall.png")) {
        std::cerr << "Failed to load wall.png" << std::endl;
        return 1;
    }

    // Background
    sf::Sprite grass;
    grass.setTexture(grass_tex);
    grass.setTextureRect(sf::IntRect(0, 0, 800, 600));

    // Player
    CustomSprite guy;
    guy.setTexture(guy_tex);
    guy.setPosition(1, 26);
    guy.setScale(0.9,0.8);
    guy.setBounds(0, 800, 0, 600);

    // Labirynth 24x32 (0 = empty, 1 = wall)
    int maze[24][32] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,1,0,0,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,0,0,1,0,0,1,1,0,0,1},
        {1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,1,0,0,1,0,0,1,1,0,0,1},
        {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,1},
        {1,0,0,0,0,0,0,1,1,1,1,1,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,1},
        {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,0,0,1,0,0,0,1},
        {1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1},
        {1,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1},
        {1,0,0,1,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0,1},
        {1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,0,0,0,1},
        {1,0,0,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0,0,1,1,0,0,1},
        {1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,0,0,0,0,1,1,0,0,1},
        {1,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1},
        {1,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
        {1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };


    // Create Walls
    std::vector<sf::Sprite> walls;
    auto createWall = [&](float x, float y) {
        sf::Sprite wall;
        wall.setTexture(wall_tex);
        wall.setTextureRect(sf::IntRect(0, 0, 100, 100));   // Cuts the texture do 100x100 px
        wall.setScale(0.25, 0.25);                          // Reduces the scale to 25x25 px
        wall.setPosition(x, y);
        walls.push_back(wall);
    };

    // Build labirynth from matrix
    for (int row = 0; row < 24; ++row) {
        for (int col = 0; col < 32; ++col) {
            if (maze[row][col] == 1) {
                createWall(col * 25.f, row * 25.f);
            }
        }
    }

    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        guy.setBounds(0, window.getSize().x, 0, window.getSize().y);
        guy.moveInDirection(elapsed, walls);

        window.clear();
        window.draw(grass);
        window.draw(guy);

        for (const auto& wall : walls) {
            window.draw(wall);
        }

        window.display();
    }

    return 0;
}
