#pragma once

#include "STLInclude.h"
#include "Weapon.h"
#include "Armor.h"
#include "Item.h"
#include "Potion.h"


class Inventory
{
public:
	Inventory();
	~Inventory();

	//copy constructor
	Inventory(const Inventory& obj);

	void addItem(const Item& item);
	void removeItem(int index);

	// Accessors
	Item* getItem(int index) const;
	int size() const;
	void render(sf::RenderWindow& window, sf::Font& font, float x, float y) const;
	
private:
	//Using Doubly Linked List 
	DoublyLinkedNode<Item*>* head;
	DoublyLinkedNode<Item*>* tail;
	int itemCount;
};

