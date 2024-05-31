
#include <SFML/Graphics.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>

std::vector<sf::Vector2f> points;

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML OpenCV Integration");
    window.setFramerateLimit(60);

    std::string fileName = "assets/screenshot.png";
    cv::Mat srcImage = cv::imread(fileName);
    if (srcImage.empty()) {
        std::cerr << "Failed to load image: " << fileName << std::endl;
        return -1;
    }

    cv::cvtColor(srcImage, srcImage, cv::COLOR_BGR2RGBA);
    sf::Image sfImage;
    sfImage.create(srcImage.cols, srcImage.rows, srcImage.ptr());
    sf::Texture texture;
    if (!texture.loadFromImage(sfImage)) {
        std::cerr << "Failed to load texture from SFML image" << std::endl;
        return -1;
    }
    sf::Sprite sprite;
    sprite.setTexture(texture);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                    points.push_back(worldPos);
                    std::cout << "Point selected: (" << worldPos.x << ", " << worldPos.y << ")" << std::endl;
                }
            }
        }

        window.clear();
        window.draw(sprite);

        for (const auto& point : points) {
            sf::CircleShape shape(5);
            shape.setPosition(point.x - shape.getRadius(), point.y - shape.getRadius());
            shape.setFillColor(sf::Color::Red);
            window.draw(shape);
        }

        if (points.size() == 4) {
            std::vector<cv::Point2f> srcPoints, dstPoints;
            for (const auto& point : points) {
                srcPoints.emplace_back(point.x, point.y);
            }
            dstPoints.emplace_back(0, 0);
            dstPoints.emplace_back(srcImage.cols - 1, 0);
            dstPoints.emplace_back(srcImage.cols - 1, srcImage.rows - 1);
            dstPoints.emplace_back(0, srcImage.rows - 1);

            cv::Mat perspectiveMatrix = cv::getPerspectiveTransform(srcPoints, dstPoints);
            cv::Mat warpedImage;
            cv::warpPerspective(srcImage, warpedImage, perspectiveMatrix, srcImage.size());

            cv::cvtColor(warpedImage, warpedImage, cv::COLOR_RGBA2BGR);
            cv::imwrite("assets/warped_image.png", warpedImage);
            std::cout << "Perspective transformation applied and saved as 'warped_image.png'" << std::endl;
        }

        window.display();
    }

    return 0;
}

