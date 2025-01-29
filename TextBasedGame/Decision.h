#pragma once
#include <memory>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "Character.h"  
#include "Item.h"  

//Forward Declaration
class Event;

class DecisionEffect {
public:
    virtual void applyEffect(Character& character, Item* item) = 0;
    virtual void renderEffectResult(sf::RenderWindow& window, sf::Font& font, float x, float y) = 0;
    virtual ~DecisionEffect() {}
};

class FindItemEffect : public DecisionEffect {
    std::string itemName; 

public:
    FindItemEffect() {}

    void applyEffect(Character& character, Item* item) override {
        if (item) {
            character.addItem(*item);
            itemName = item->getName();
            std::cout << "You found an item: " << itemName << "!" << std::endl;
        }
    }

    void renderEffectResult(sf::RenderWindow& window, sf::Font& font, float x, float y) override {
        if (!itemName.empty()) {
            sf::Text text;
            text.setFont(font);
            text.setString("You found an item: " + itemName + "!");
            text.setCharacterSize(24);
            text.setFillColor(sf::Color::White);
            text.setPosition(x, y);
            window.draw(text);
        }
    }
};

class DamageHealthEffect : public DecisionEffect {
    int damageAmount;

public:
    DamageHealthEffect(int damageAmount) : damageAmount(damageAmount) {}

    void applyEffect(Character& character, Item* item) override {
        character.takeDamage(damageAmount);
    }

    void renderEffectResult(sf::RenderWindow& window, sf::Font& font, float x, float y) override {
        sf::Text text;
        text.setFont(font);
        text.setString("You take " + std::to_string(damageAmount) + " damage!");
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setPosition(x, y);
        window.draw(text);
    }
};

class IncreaseStatEffect : public DecisionEffect {
    std::string stat;
    int amount;

public:
    IncreaseStatEffect(const std::string& stat, int amount) : stat(stat), amount(amount) {}

    void applyEffect(Character& character, Item* item) override {
        character.increaseStat(stat, amount);
    }

    void renderEffectResult(sf::RenderWindow& window, sf::Font& font, float x, float y) override {
        // Render the result of the stat increase effect.
        sf::Text text;
        text.setFont(font);
        text.setString("Your " + stat + " has increased by " + std::to_string(amount) + "!");
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setPosition(x, y);
        window.draw(text);
    }
};




class Decision {
public:
    std::string text;
    bool requiresCheck;
    int checkThreshold;
    std::string stat;
    std::string successText;
    std::string failText;
    std::unique_ptr<DecisionEffect> successEffect;
    std::unique_ptr<DecisionEffect> failureEffect;
    std::shared_ptr<Item> droppedItem; // Smart pointers for better memory management

    // Constructor
    Decision(const std::string& text, bool requiresCheck, int checkThreshold,
        const std::string& stat, const std::string& successText,
        const std::string& failText, DecisionEffect* successEffect = nullptr,
        DecisionEffect* failureEffect = nullptr, Item* droppedItem = nullptr)
        : text(text), requiresCheck(requiresCheck), checkThreshold(checkThreshold),
        stat(stat), successText(successText), failText(failText),
        successEffect(successEffect), failureEffect(failureEffect),
        droppedItem(droppedItem) {}

    // Delete the copy constructor and the copy assignment operator.
    Decision(const Decision&) = delete;
    Decision& operator=(const Decision&) = delete;

    // Render function
    void render(sf::RenderWindow& window, sf::Font& font, float x, float y) const {
        sf::Text sfmlText;
        sfmlText.setFont(font);
        sfmlText.setString(text);
        sfmlText.setCharacterSize(24);
        sfmlText.setFillColor(sf::Color::White);
        sfmlText.setPosition(x, y);
        window.draw(sfmlText);
    }

    // Move constructor
    Decision(Decision&& other) noexcept
        : text(std::move(other.text)),
        requiresCheck(other.requiresCheck),
        checkThreshold(other.checkThreshold),
        stat(std::move(other.stat)),
        successText(std::move(other.successText)),
        failText(std::move(other.failText)),
        successEffect(std::move(other.successEffect)),
        failureEffect(std::move(other.failureEffect)),
        droppedItem(std::move(other.droppedItem)) {}

    // Move assignment operator
    Decision& operator=(Decision&& other) noexcept {
        if (this != &other) {
            text = std::move(other.text);
            requiresCheck = other.requiresCheck;
            checkThreshold = other.checkThreshold;
            stat = std::move(other.stat);
            successText = std::move(other.successText);
            failText = std::move(other.failText);
            successEffect = std::move(other.successEffect);
            failureEffect = std::move(other.failureEffect);
            droppedItem = std::move(other.droppedItem);
        }
        return *this;
    }

    // Destructor
    ~Decision() = default;
};

//Decision Factory Design Pattern 
class DecisionFactory {
public:
    static std::unique_ptr<Decision> createDecision(
        const std::string& text,
        bool requiresCheck,
        int checkThreshold,
        const std::string& stat,
        const std::string& successText,
        const std::string& failText,
        DecisionEffect* successEffect = nullptr,
        DecisionEffect* failureEffect = nullptr,
        Item* droppedItem = nullptr);
};

