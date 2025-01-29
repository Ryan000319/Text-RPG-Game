// Armor.cpp
#include "Armor.h"


dynaArr<string> Armor::names;

//Use Dynamic Array to store Armor names
void Armor::initNames()
{
	Armor::names.push("Leather Armor");
	Armor::names.push("iron Armor");
	Armor::names.push("Gold Armor");
	Armor::names.push("ChainMail Armor");
	Armor::names.push("Diamond Armor");
	Armor::names.push("Nether Armor");
}

//Defaule Constructor
Armor::Armor()
	:Item("NONE", 0, 0, 0, COMMON) { 
	this->defence = 0;
}

//Constructor
Armor::Armor(int level, int rarity)
	:Item(Armor::names[rand() % Armor::names.size()],
		level,
		calculateBuyValue(level, rarity),
		calculateSellValue(level, rarity),
		rarity) 
{
	this->defence = rand() % (level + (rarity + 1));
	this->defence += (rarity + 1) * 5;
	if (rarity == 3)
		this->defence += level * 5;
	else if (rarity == 4)
		this->defence += level * 10;
}


Armor::Armor(int defence, string name, int level, int buyValue, int sellValue, int rarity)
	: Item(name, level, buyValue, sellValue, rarity) {
	this->defence = defence;
}

//Destructor
Armor::~Armor()
{
}

//Pure virtual
Armor* Armor::clone() const
{
    return new Armor(*this);
}

//Convert and save private attributes in string
string Armor::toString() const
{
	string str =
		" -------------------- \n Name: " + this->getName()
		+ " \n Level: " + to_string(this->getLevel())
		+ " \n Rarity: " + this->getRarity()
		+ " \n Buy Value: " + to_string(this->getBuyValue()) + " Gold"
		+ " \n -------------------- ";

		
	return str;
}

