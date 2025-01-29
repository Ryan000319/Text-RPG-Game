#pragma once

#include "Character.h"
#include "STlInclude.h"
#include "Weapon.h"
#include "Armor.h"
#include "Potion.h"

class Shop {
public:
	Shop(int playerLevel);
	~Shop();

	void openShop(Character& character, sf::RenderWindow& window, sf::Font& font);
	void displayItemsForSale(sf::RenderWindow& window, sf::Font& font) const;
	void purchaseItem(Character& character, int index, sf::RenderWindow& window, sf::Font& font);
	int getItemsCount() const;

private:
	SinglyLinkedNode<Item*>* itemsForSaleHead;
	int playerLevel;
	
	void generateItemsForSale();
	void clearShop();
	int calculateItemPrice(const Item* item) const;

	sf::Texture ShopTexture;
	sf::Sprite ShopSprite;
};

