#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

int main()
{
    auto window = sf::RenderWindow{ { 800u, 600u }, "SFML Tester" };
    window.setFramerateLimit(144);

    sf::Font font;
    if (!font.loadFromFile("assets/Teachers-Regular.ttf"))
    {
        std::cerr << "Could not load font\n";
        return -1;
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(10.f, 10.f);

    std::string fileName;
    sf::Texture texture;
    sf::Sprite sprite;
    bool imageLoaded = false;

    while (window.isOpen())
    {

        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.scancode == sf::Keyboard::Scan::Escape)
                {
                    window.close();
                }

                if (event.key.scancode == sf::Keyboard::Scan::Enter)
                {
                    if (texture.loadFromFile("assets/" + fileName))
                    {
                        sprite.setTexture(texture);
                        imageLoaded = true;
                    }
                    else
                    {
                        std::cerr << "Failed to load image\n";
                        imageLoaded = false;
                    }
                }
            }

            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode == '\b') // Handle backspace
                {
                    if (!fileName.empty())
                    {
                        fileName.pop_back();
                    }
                }
                else if (event.text.unicode < 128)
                {
                    fileName += static_cast<char>(event.text.unicode);
                }

                text.setString("Enter file name: " + fileName);
            }
        }

        window.clear();
        window.draw(text);
        if (imageLoaded)
        {
            window.draw(sprite);
        }
        window.display();
    }

    return 0;
}
