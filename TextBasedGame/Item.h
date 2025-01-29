#pragma once

#include "STLInclude.h"
enum rarity
{
	COMMON,
	UNCOMMON,
	RARE,
	EPIC,
	LEGENDARY
};

class Item
{
public:
	Item();
	Item(int level, int rarity);
	Item(string name, 
		int level, 
		int buyValue, 
		int sellValue, 
		int rarity);

	virtual ~Item();

	inline string debugPrint() const { return this->name; }
	virtual Item* clone() const = 0;
	virtual string toString() const = 0;

	//Accessors
	inline const string& getName() const { return this->name; }
	inline const int& getLevel() const { return this->level; }
	inline const int& getBuyValue() const { return this->buyValue; }
	inline const int& getSellValue() const { return this->sellValue; }

	//Rarity of Items converted to string
	inline string getRarity() const {
		switch (this->rarity) {
		case COMMON: return "Common";
		case UNCOMMON: return "Uncommon";
		case RARE: return "Rare";
		case EPIC: return "Epic";
		case LEGENDARY: return "Legendary";
		default: return "Unknown";
		}
	}

	//Modifiers
	inline void setName(string name) { this->name = name; }

	//Static
	inline static int calculateBuyValue(int level, int rarity){ return 100 * level * (rarity + 1); }
	inline static int calculateSellValue(int level, int rarity) { return calculateBuyValue(level, rarity) / 2; };

	void render(sf::RenderWindow& window, sf::Font& font, float x, float y) const;
private:
	string name;
	int level;
	int buyValue;
	int sellValue;
	int rarity;
};

