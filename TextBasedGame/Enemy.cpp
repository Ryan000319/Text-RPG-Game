#include "Enemy.h"


Enemy::Enemy(EnemyType type, int  level) : type(type), level(level)
{


	switch (type)
	{
	case EnemyType::DreadKnight:
		this->name = "Dread Knight";
		this->hpMax = level * 12;
		this->damageMin = level * 2;
		this->damageMax = level * 4;
		this->defence = level * 2;
		this->accuracy = level * 2;
		break;

	case EnemyType::ForestWraith:
		this->name = "Forest Wraith";
		this->hpMax = level * 5;
		this->damageMin = level;
		this->damageMax = level * 3;
		this->defence = level;
		this->accuracy = level * 3;
		break;

	case EnemyType::RockTroll:
		this->name = "Rock Troll";
		this->hpMax = level * 15;
		this->damageMin = level * 2;
		this->damageMax = level * 3;
		this->defence = level * 5;
		this->accuracy = level;
		break;

	case EnemyType::CrystalDragon:
		this->name = "Crystal Dragon";
		this->hpMax = level * 20;
		this->damageMin = level * 4;
		this->damageMax = level * 6;
		this->defence = level * 4;
		this->accuracy = level * 2;
		break;

	case EnemyType::MinotaurWarlord:
		this->name = "Minotaur Warlord";
		this->hpMax = level * 18;
		this->damageMin = level * 3;
		this->damageMax = level * 7;
		this->defence = level * 5;
		this->accuracy = level * 2;
		break;

	case EnemyType::FireGolem:
		this->name = "Fire Golem";
		this->hpMax = level * 10;
		this->damageMin = level * 5;
		this->damageMax = level * 7;
		this->defence = level  * 2;
		this->accuracy = level;
		break;
	}


	this->hp = this->hpMax;

	
}

Enemy::~Enemy()
{

}

string Enemy::getAsString() const
{
	stringstream ss;

	ss << " Name: " << this->name << "\n"
		<< " Level : " << this->level << "\n"
		<< " HP: " << this->hp << " / " << this->hpMax << "\n"
		<< " Damage: " << this->damageMin << " - " << this->damageMax << "\n"
		<< " Defence: " << this->defence << "\n"
		<< " Accuracy: " << this->accuracy << "\n";


	return ss.str();
		


}

void Enemy::render(sf::RenderWindow& window, sf::Font& font, float x, float y) const {
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);
	text.setPosition(x, y);

	std::stringstream ss;
	ss << "Name: " << this->name << "\n"
		<< "Level: " << this->level << "\n"
		<< "HP: " << this->hp << " / " << this->hpMax << "\n"
		<< "Damage: " << this->damageMin << " - " << this->damageMax << "\n"
		<< "Defence: " << this->defence << "\n"
		<< "Accuracy: " << this->accuracy;

	text.setString(ss.str());
	window.draw(text);

	// Adjust y-position for each line of text.
	float yOffset = 0.0f;
	std::string line;
	while (std::getline(ss, line)) {
		text.setPosition(x, y + yOffset);
		text.setString(line);
		window.draw(text);
		yOffset += text.getLocalBounds().height + 5; // Adjust line spacing.
	}
}

void Enemy::takeDamage(int damage)
{
	this->hp -= damage;

	if (this->hp <= 0)
	{
		this->hp = 0;
	}


}

void Enemy::allocateActionPoints()
{
	this->actionPoints = (rand() % 3) + this->level / 2;
}