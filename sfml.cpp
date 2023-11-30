#include <SFML/Graphics.hpp>
using namespace sf;
#include <iostream>
using namespace std;

int main() {
    RenderWindow window(VideoMode(400, 500), "SFML works!", Style::Close | Style::Resize);
    window.setPosition(sf::Vector2i(300, 200));
    RectangleShape rectangle(Vector2f(100, 100));
    Texture rectangletexture;
    Sprite Rectanglesprite;
    rectangletexture.loadFromFile("Textures/fast.png");
    rectangle.setTexture(&rectangletexture);
    Rectanglesprite.setTextureRect(sf::IntRect(0, 0, 100, 100));

    Vector2u size = rectangletexture.getSize();
    size.x /= 3;
    size.y /= 4;

    rectangle.setTextureRect(sf::IntRect(size.x * 2, size.y * 1, size.x, size.y));

    const float moveSpeed = 0.2f; // Adjusted move speed for better control

    while(window.isOpen()) {
        Event OK;
        while(window.pollEvent(OK)) {
            if(OK.type == Event::Closed)
                window.close();
            else if (OK.type == Event::Resized)
                cout << "New width: " << OK.size.width << " New height: " << OK.size.height << endl;
            else if (OK.type == Event::TextEntered) {
                if(OK.text.unicode < 128) {
                    cout << static_cast<char>(OK.text.unicode);
                }
            }
        }

        // Movement logic
        if (Keyboard::isKeyPressed(Keyboard::Key::A))
            rectangle.move(-moveSpeed, 0);
        if (Keyboard::isKeyPressed(Keyboard::Key::D))
            rectangle.move(moveSpeed, 0);
        if (Keyboard::isKeyPressed(Keyboard::Key::W))
            rectangle.move(0, -moveSpeed);
        if (Keyboard::isKeyPressed(Keyboard::Key::S))
            rectangle.move(0, moveSpeed);

        // Wrapping logic
        Vector2f position = rectangle.getPosition();
        const float windowWidth = static_cast<float>(window.getSize().x);
        const float windowHeight = static_cast<float>(window.getSize().y);

        // Check and wrap horizontally
        if (position.x + rectangle.getSize().x < 0)
            position.x = windowWidth;
        else if (position.x > windowWidth)
            position.x = -rectangle.getSize().x;

        // Check and wrap vertically
        if (position.y + rectangle.getSize().y < 0)
            position.y = windowHeight;
        else if (position.y > windowHeight)
            position.y = -rectangle.getSize().y;

        rectangle.setPosition(position);

        // Mouse position handling
        if (Mouse::isButtonPressed(Mouse::Left)) {
            Vector2i mousePos = Mouse::getPosition(window);
            rectangle.setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        }

        window.clear();
        window.draw(rectangle);
        window.display();
    }

    return 0;
}
