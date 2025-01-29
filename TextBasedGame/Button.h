#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button {
    sf::RectangleShape shape;
    sf::Text text;
    sf::Texture normalTexture; //Texture when no mouse hover
    sf::Texture hoverTexture; //Texture when mouse hover
    bool isHovering;

public:
    //Constructor
    Button(float x, float y, float width, float height, sf::Font& font, const std::string& textStr,
        const std::string& normalTexturePath, const std::string& hoverTexturePath) {
        shape.setPosition(sf::Vector2f(x, y));
        shape.setSize(sf::Vector2f(width, height));

        
        if (!normalTexture.loadFromFile(normalTexturePath)) {
            std::cerr << "Error loading normal texture" << std::endl;
        }

        if (!hoverTexture.loadFromFile(hoverTexturePath)) {
            std::cerr << "Error loading hover texture" << std::endl;
        }

        shape.setTexture(&normalTexture);
        isHovering = false;

        
        text.setFont(font);
        text.setString(textStr);
       
    }

    void update(sf::RenderWindow& window) {
        if (isMouseOver(window)) {
            isHovering = true;
            shape.setTexture(&hoverTexture);
        }
        else {
            isHovering = false;
            shape.setTexture(&normalTexture);
        }
    }

    void drawTo(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }

    // Check if the mouse is over the button
    bool isMouseOver(sf::RenderWindow& window) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f btnPos = shape.getPosition();

        if (mousePos.x > btnPos.x && mousePos.x < btnPos.x + shape.getLocalBounds().width &&
            mousePos.y > btnPos.y && mousePos.y < btnPos.y + shape.getLocalBounds().height) {

            
            return true;
        }

        return false;
    }
};
