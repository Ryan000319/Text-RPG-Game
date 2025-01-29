#include "Item.h"

Item::Item()
{
	this->name = "NONE";
	this->level = 0;
	this->buyValue = 0;  
	this->sellValue = 0; 
	this->rarity = 0;
}

Item::Item(int level, int rarity)
{
	this->name = "NONE";
	this->level = rand() % level+3;

	this->buyValue = level * rand()%rarity+1;
	this->sellValue = this->buyValue / 2;
}

Item::Item(string name, int level, int buyValue, int sellValue, int rarity)
{
	this->name = name;
	this->level = level;
	this->buyValue = buyValue;
	this->sellValue = sellValue;
	this->rarity = rarity;

}

Item::~Item()
{

}

void Item::render(sf::RenderWindow& window, sf::Font& font, float x, float y) const {
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);
	text.setPosition(x, y);

	std::stringstream ss;
	ss << "Name: " << this->name << "\n"
		<< "Level: " << this->level << "\n"
		<< "Rarity: " << getRarity() << "\n"
		<< "Buy Value: " << this->buyValue << "\n"
		<< "Sell Value: " << this->sellValue << "\n";

	text.setString(ss.str());
	window.draw(text);
}
