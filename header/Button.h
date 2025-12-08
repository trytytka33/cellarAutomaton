#pragma once
#include <functional>

#include <SFML/Graphics.hpp>

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Color normalColor;
    sf::Color hoverColor;
    sf::Color pressedColor;
    bool isHovered;
    bool isPressed;
    std::function<void()> onClick;

public:
    Button(sf::Vector2f pos, sf::Vector2f size, const std::string& label,
        sf::Font& font, std::function<void()> callback);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void update();
    void draw(sf::RenderWindow& window);
    void updateTextPosition();  
    void setPosition(sf::Vector2f pos);  
};