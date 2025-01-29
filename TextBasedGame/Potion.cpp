#include "Potion.h"
//Initialize static namesMap container
map<Potion::PotionType, string> Potion::namesMap;

void Potion::initNames()
{
	namesMap[PotionType::Health] = "Potion of Healing";
	namesMap[PotionType::Strength] = "Elixir of Strength";
	namesMap[PotionType::Vitality] = "Essence of Vitality";
	namesMap[PotionType::Dexterity] = "Brew of Dexterity";
	namesMap[PotionType::Intelligence] = "Tonic of Intelligence";
}

Potion::Potion()
	: Item(), type(PotionType::Health),potency(0){}

Potion::Potion(string name, int level, int rarity, PotionType type, int potency)
	:Item(namesMap[type], level, calculateBuyValue(level, rarity),calculateSellValue(level, rarity), rarity),
	type(type), potency(potency){}

Potion::~Potion(){}

Potion* Potion::clone() const 
{
	return new Potion(*this);
}

string Potion::toString() const
{
	string typeStr;
	switch (type)
	{
	case PotionType::Health: typeStr = "Health"; break;
	case PotionType::Strength: typeStr = "Strength"; break;
	case PotionType::Vitality: typeStr = "Vitality"; break;
	case PotionType::Dexterity: typeStr = "Dexterity"; break;
	case PotionType::Intelligence: typeStr = "Intelligence"; break;
	}
	string str =
		" -------------------- \n Name: " + this->getName()
		+ " \n Description: +1 " + typeStr
		+ " \n Potency: " + to_string(this->getPotency())
		+ " \n Buy Value: " + to_string(this->getBuyValue()) + " Gold"
		+ " \n -------------------- ";


	return str;
}

Potion::PotionType Potion::getType() const
{
	return type;
}

int Potion::getPotency() const
{
	return potency;
}

void Potion::render(sf::RenderWindow& window, sf::Font& font, float x, float y) const {
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);
	text.setPosition(x, y);

	std::string typeStr;
	switch (type) {
	case PotionType::Health: typeStr = "Health"; break;
	case PotionType::Strength: typeStr = "Strength"; break;
	case PotionType::Vitality: typeStr = "Vitality"; break;
	case PotionType::Dexterity: typeStr = "Dexterity"; break;
	case PotionType::Intelligence: typeStr = "Intelligence"; break;
	}

	std::stringstream ss;
	ss << "Name: " << getName() << "\n"
		<< "Type: " << typeStr << "\n"
		<< "Potency: " << potency << "\n"
		<< "Level: " << getLevel() << "\n"
		<< "Rarity: " << getRarity() << "\n";

	text.setString(ss.str());
	window.draw(text);
}