#include "Shop.h"

Shop::Shop(int level) : playerLevel(level), itemsForSaleHead(nullptr)
{
    srand(static_cast<unsigned>(time(0)));
    generateItemsForSale();
}

Shop::~Shop()
{
    clearShop();
}

void Shop::displayItemsForSale(sf::RenderWindow& window, sf::Font& font) const {
    int index = 1;
    float xPos = 50.0f; // Starting y-position for displaying items

    for (SinglyLinkedNode<Item*>* current = itemsForSaleHead; current != nullptr; current = current->getNext()) {
        int price = calculateItemPrice(current->getValue());
        sf::Text itemText;
        itemText.setFont(font);
        itemText.setString(std::to_string(index) + ": " + current->getValue()->toString());
        itemText.setCharacterSize(24);
        itemText.setFillColor(sf::Color::White);
        itemText.setPosition(xPos, 650.f); // Adjusted position for better visibility

        window.draw(itemText);
        xPos += 320.0f; // Increased space between lines for clarity
        index++;
    }
}


int Shop::getItemsCount() const {
    int count = 0;
    for (SinglyLinkedNode<Item*>* current = itemsForSaleHead; current != nullptr; current = current->getNext()) {
        count++;
    }
    return count;
}


void Shop::openShop(Character& character, sf::RenderWindow& window, sf::Font& font) {
    bool shopOpen = true;

    // Load and set the shop background
    if (!ShopTexture.loadFromFile("Assets/IMG/shop2.png")) {
        std::cerr << "Failed to load main menu background texture" << std::endl;
    }
    ShopSprite.setTexture(ShopTexture);
    ShopSprite.setPosition(0.f, 0.f);
    

    // Display a welcome message
    sf::Text welcomeTextShop("Good day, brave adventurer! You've come to the right place for all your battle needs. \nHere at my shop, we have a fine selection of armor, weapons, \nand potions to aid you on your quests", font, 24);
    welcomeTextShop.setFillColor(sf::Color::White);
    welcomeTextShop.setPosition(50.f, 525.f);
   

    // Set up input prompt and user input text
    sf::Text inputPrompt("Enter item number to purchase or '0' to exit:", font, 24);
    inputPrompt.setFillColor(sf::Color::White);
    inputPrompt.setPosition(50.f, 850.f);

    sf::Text userInput("", font, 24);
    userInput.setFillColor(sf::Color::White);
    userInput.setPosition(750.f, 850.f);

    std::string userEnteredText;

    while (shopOpen) {
        // Event handling
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                shopOpen = false;
            }

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    userEnteredText += static_cast<char>(event.text.unicode);
                    userInput.setString(userEnteredText);
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    int itemIndex = std::stoi(userEnteredText) - 1;
                    if (itemIndex >= 0 && itemIndex < getItemsCount()) {
                        purchaseItem(character, itemIndex, window, font);
                    }
                    else if (itemIndex == -1) {
                        shopOpen = false; // Exit shop if 0 is entered
                    }
                    userEnteredText = ""; // Clear the entered text
                }
                else if (event.key.code == sf::Keyboard::BackSpace && !userEnteredText.empty()) {
                    userEnteredText.pop_back();
                    userInput.setString(userEnteredText);
                }
            }
        }

        // Rendering
        window.clear();
        window.draw(ShopSprite);
        window.draw(welcomeTextShop); // Draw the welcomeTextShop
        displayItemsForSale(window, font);  // Display the items for sale
        window.draw(inputPrompt);
        window.draw(userInput);
        window.display();
    }
}

void Shop::purchaseItem(Character& character, int index, sf::RenderWindow& window, sf::Font& font) {
    Iterator<Item*> it = Iterator<Item*>(itemsForSaleHead);

    for (int i = 0; i < index; ++i) {
        ++it;
    }

    sf::Text feedbackText;
    feedbackText.setFont(font);
    feedbackText.setCharacterSize(24);
    feedbackText.setPosition(100.f, 950.f); 

    if (*it) {
        int price = calculateItemPrice(*it);
        if (character.getGold() >= price) {
            character.payGold(price);
            character.addItem(**it);
            feedbackText.setString("You purchased: " + (*it)->getName() + ". It has been added to your inventory.");
        }
        else {
            feedbackText.setString("Not enough gold!");
        }
    }
    else {
        feedbackText.setString("Invalid item selection.");
    }

    // Display the feedback for a short duration
    window.clear();
    displayItemsForSale(window, font); // Redraw the items for sale
    window.draw(ShopSprite); // Draw the welcomeTextShop
    window.draw(feedbackText);
    window.display();
    sf::sleep(sf::seconds(2)); // Hold the screen for 2 seconds to allow the user to read the message
}


void Shop::generateItemsForSale() {
    clearShop();

    // Random Potion
    Potion::PotionType randomPotionType = static_cast<Potion::PotionType>(rand() % static_cast<int>(Potion::PotionType::Intelligence) + 1);
    string potionName = Potion::namesMap[randomPotionType];
    itemsForSaleHead = new SinglyLinkedNode<Item*>(new Potion(potionName, playerLevel, rand() % 5, randomPotionType, rand() % playerLevel * 10 + 10), nullptr);

    // Random Armor
    itemsForSaleHead->setNext(new SinglyLinkedNode<Item*>(new Armor(playerLevel, rand() % 5), nullptr));

    // Random Weapon
    itemsForSaleHead->getNext()->setNext(new SinglyLinkedNode<Item*>(new Weapon(playerLevel, rand() % 5), nullptr));
}


void Shop::clearShop()
{
    while (itemsForSaleHead)
    {
        SinglyLinkedNode<Item*>* temp = itemsForSaleHead;
        itemsForSaleHead = itemsForSaleHead->getNext();
        delete temp->getValue(); //Delete item
        delete temp; //Delete node
    }
}

int Shop::calculateItemPrice(const Item* item) const {
    if (item)
    {
        int rarityValue = 0;
        string rarityStr = item->getRarity();

        if (rarityStr == "Common") {
            rarityValue = COMMON;
        }
        else if (rarityStr == "Uncommon") {
            rarityValue = UNCOMMON;
        }
        else if (rarityStr == "Rare") {
            rarityValue = RARE;
        }
        else if (rarityStr == "Epic") {
            rarityValue = EPIC;
        }
        else if (rarityStr == "Legendary") {
            rarityValue = LEGENDARY;
        }

        return item->getBuyValue();
    }

    return 0;
}
