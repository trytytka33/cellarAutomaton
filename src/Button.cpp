#include "Button.h"

Button::Button(sf::Vector2f pos, sf::Vector2f size, const std::string& label,
    sf::Font& font, std::function<void()> callback)
    : text(font),
    normalColor(70, 130, 180),
    hoverColor(100, 149, 237),
    pressedColor(65, 105, 225),
    isHovered(false),
    isPressed(false),
    onClick(callback)
{
    shape.setSize(size);
    shape.setPosition(pos);
    shape.setFillColor(normalColor);

    text.setString(label);
    text.setCharacterSize(15);
    text.setFillColor(sf::Color::White);

    updateTextPosition();
}

void Button::updateTextPosition()
{
    sf::FloatRect textBounds = text.getLocalBounds();
    sf::Vector2f shapePos = shape.getPosition();
    sf::Vector2f shapeSize = shape.getSize();

    text.setOrigin({ textBounds.position.x + textBounds.size.x / 2.0f,
                    textBounds.position.y + textBounds.size.y / 2.0f });
    text.setPosition({ shapePos.x + shapeSize.x / 2.0f, shapePos.y + shapeSize.y / 2.0f });
}

void Button::setPosition(sf::Vector2f pos)
{
    shape.setPosition(pos);
    updateTextPosition();
}

void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

            if (shape.getGlobalBounds().contains(mousePosF))
            {
                isPressed = true;
            }
        }
    }

    if (const auto* mouseButtonReleased = event.getIf<sf::Event::MouseButtonReleased>())
    {
        if (mouseButtonReleased->button == sf::Mouse::Button::Left)
        {
            if (isPressed)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

                if (shape.getGlobalBounds().contains(mousePosF))
                {
                    if (onClick)
                    {
                        onClick();
                    }
                }
                isPressed = false;
            }
        }
    }

    if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>())
    {
        sf::Vector2f mousePosF(static_cast<float>(mouseMoved->position.x),
            static_cast<float>(mouseMoved->position.y));
        isHovered = shape.getGlobalBounds().contains(mousePosF);
    }
}

void Button::update()
{
    if (isPressed)
    {
        shape.setFillColor(pressedColor);
    }
    else if (isHovered)
    {
        shape.setFillColor(hoverColor);
    }
    else
    {
        shape.setFillColor(normalColor);
    }
}

void Button::draw(sf::RenderWindow& window)
{
    sf::Vector2f pos = shape.getPosition();
    sf::Vector2f size = shape.getSize();
    float radius = 8.0f;

    sf::RectangleShape mainRect({ size.x - 2 * radius, size.y });
    mainRect.setPosition({ pos.x + radius, pos.y });
    mainRect.setFillColor(shape.getFillColor());
    window.draw(mainRect);

    sf::RectangleShape vertRect({ size.x, size.y - 2 * radius });
    vertRect.setPosition({ pos.x, pos.y + radius });
    vertRect.setFillColor(shape.getFillColor());
    window.draw(vertRect);

    sf::CircleShape corner(radius);
    corner.setFillColor(shape.getFillColor());

    corner.setPosition({ pos.x + size.x - 2 * radius, pos.y });
    window.draw(corner);

    corner.setPosition({ pos.x, pos.y });
    window.draw(corner);

    corner.setPosition({ pos.x + size.x - 2 * radius, pos.y + size.y - 2 * radius });
    window.draw(corner);

    corner.setPosition({ pos.x, pos.y + size.y - 2 * radius });
    window.draw(corner);

    window.draw(text);
}