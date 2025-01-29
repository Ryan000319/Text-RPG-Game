#include "Weapon.h"

dynaArr<string> Weapon::names;

void Weapon::initNames()
{
	Weapon::names.push("Combat Knife");
	Weapon::names.push("Great Sword");
	Weapon::names.push("Axe");
	Weapon::names.push("PickAxe");
	Weapon::names.push("WarHammer");
	Weapon::names.push("Katana");
}

Weapon::Weapon()
	:Item("NONE", 0, 0, 0, COMMON) { // Default constructor
	this->damageMax = 0;
	this->damageMin = 0;
}

Weapon::Weapon(int level, int rarity)
	:Item(Weapon::names[rand() % Weapon::names.size()],
		level,
		calculateBuyValue(level, rarity),
		calculateSellValue(level, rarity),
		rarity) 
{
	this->damageMax = rand() % level * (rarity + 1);
	this->damageMax += (rarity + 1) * 5;
	if (rarity == 3)
		this->damageMax += level * 5;
	else if (rarity == 4)
		this->damageMax += level * 10;
	this->damageMin = this->damageMax / 2;
}

Weapon::Weapon(int damageMin, int damageMax, string name, int level, int buyValue, int sellValue, int rarity)
	: Item(name, level, buyValue, sellValue, rarity) {
	this->damageMin = damageMin;
	this->damageMax = damageMax;
}

Weapon::~Weapon()
{

}

Weapon* Weapon::clone() const
{
	return new Weapon(*this);
}

string Weapon::toString() const
{
	string str = 
		" -------------------- \n Name: " + this->getName()
		+ " \n Level: " + to_string(this->getLevel())
		+ " \n Rarity: " + this->getRarity()
		+ " \n Damage: " + to_string(this->damageMin) + " - " + to_string(this->damageMax)
		+ " \n Buy Value: " + to_string(this->getBuyValue()) + " Gold"
		+ " \n -------------------- ";

	return str;
}

void Weapon::render(sf::RenderWindow& window, sf::Font& font, float x, float y) const {
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);
	text.setPosition(x, y);

	// Create the string representation of the weapon
	std::string weaponInfo = toString();  

	// Set the string to the text object
	text.setString(weaponInfo);

	// Draw the text to the window
	window.draw(text);
}