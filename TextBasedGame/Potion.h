#pragma once
#include "Item.h"

class Potion : public Item 
{
public: 
	enum class PotionType
	{
		Health = 0,
		Strength,
		Vitality,
		Dexterity,
		Intelligence
	};

	Potion();
	Potion(string name, int level, int rarity, PotionType type, int potency);

	virtual ~Potion();

	virtual Potion* clone() const override;
	virtual string toString() const override;

	//Accessors
	PotionType getType() const;
	int getPotency() const;
	
	static void initNames();

	static map<PotionType, string> namesMap;

	void render(sf::RenderWindow& window, sf::Font& font, float x, float y) const;
private:
	PotionType type;
	int potency;
};

