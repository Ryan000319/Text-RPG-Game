#include "Inventory.h"

Inventory::Inventory() : head(nullptr), tail(nullptr), itemCount(0){}

Inventory::~Inventory() 
{
    while (head != nullptr)
    {
        DoublyLinkedNode<Item*>* temp = head;
        head = head->getNext();
        delete temp->getValue();
        delete temp;
    }
}

Inventory::Inventory(const Inventory& obj) : Inventory() {
    if (obj.head == nullptr) {
        return; // Nothing to copy
    }

    DoublyLinkedNode<Item*>* current = obj.head;
    while (current != nullptr) {
        Item* currentItem = current->getValue();
        if (currentItem != nullptr) {
            addItem(*currentItem);
        }
        else {
            // Handle the error: currentItem is null
        }
        current = current->getNext();
    }
}


void Inventory::addItem(const Item& item) 
{
    DoublyLinkedNode<Item*>* newNode = new DoublyLinkedNode<Item*>(item.clone());
    if (tail != nullptr)
    {
        tail->append(newNode);

    }
    else
    {
        head = newNode;
    }

    tail = newNode;
    itemCount++;
}


void Inventory::removeItem(int index) {
    if (index < 0 || index >= itemCount)
    {
        throw out_of_range("Index out of bounds");
    }

    DoublyLinkedNode<Item*>* current = head;
    for (int i = 0; i < index; ++i)
    {
        current = current->getNext();
    }

    if (current == head)
    {
        head = current->getNext();
    }

    if (current == tail)
    {
        tail = current->getPrevious();
    }

    current->remove();
    delete current->getValue();
    delete current;
    itemCount--;
}


Item* Inventory::getItem(int index) const {
    if (index < 0 || index >= itemCount)
    {
        throw out_of_range("Index out of bounds");
    }

    DoublyLinkedNode<Item*>* current = head;
    for (int i = 0; i < index; ++i)
    {
        current = current->getNext();
    }

    return current->getValue();
}

int Inventory::size() const
{
    return itemCount;
}


void Inventory::render(sf::RenderWindow& window, sf::Font& font, float x, float y) const {
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);

    float yOffset = 0.0f;
    DoublyLinkedNode<Item*>* current = head;
    int index = 0;

    while (current != nullptr) {
        Item* currentItem = current->getValue();
        if (currentItem != nullptr) {
            text.setPosition(x, y + yOffset);
            text.setString(std::to_string(++index) + ": " + currentItem->getName());
            window.draw(text);

            yOffset += text.getLocalBounds().height + 5; // Adjust line spacing.
        }

        current = current->getNext();
    }
}