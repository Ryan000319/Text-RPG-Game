#include "Character.h"

//Default Constructor Implementation
Character::Character():equippedWeapon(nullptr), equippedArmor(nullptr)
{
	this->distanceTravelled = 0;
	this->gold = 0;

	this->name = "None";
	this->level = 1;
	this->exp = 0;
	this->expNext = 0;

	this->strength = 0;
	this->vitality = 0;
	this->dexterity = 0;
	this->intelligence = 0;

	this->hp = 0;
	this->hpMax = 0;
	this->stamina = 0;
	this->staminaMax = 0;
	this->damageMin = 0;
	this->damageMax = 0;
	this->defence = 0;
	this->accuracy = 0;
	this->luck = 0;

	this->statPoints = 0;
	
	this->actionPoints = this->level*2;
}

//Constructor - for loading saved characters
Character::Character(string name, 
	int distanceTravelled, 
	int gold, int level, 
	int exp, int strength, 
	int vitality, int dexterity, 
	int intelligence, int hp, 
	int stamina, int statPoints) 
{
	this->distanceTravelled = distanceTravelled;
	this->gold = gold;

	this->name = name;
	this->level = level;
	this->exp = exp;
	this->expNext = 0;

	this->strength = strength;
	this->vitality = vitality;
	this->dexterity = dexterity;
	this->intelligence = intelligence;

	this->hp = hp;
	this->hpMax = 0;
	this->stamina = stamina;
	this->staminaMax = 0;
	this->damageMin = 0;
	this->damageMax = 0;
	this->defence = 0;
	this->accuracy = 0;
	this->luck = 0;

	this->statPoints = statPoints;
	
	this->actionPoints = this->level * 2;
	

	this->updateStats();
}


Character::~Character()
{

}

//Constructor - for creating new character
void Character::initialize(const string name)
{
	this->distanceTravelled = 0;
	this->gold = 100;

	this->name = name;
	this->level = 1;
	this->exp = 0;
	this->expNext = 
		static_cast<int>((static_cast<double>(50) / 3) * ((pow(level, 3) - 6 * pow(level, 2)) + 17 * level - 12)) + 100;

	this->strength = 5;
	this->vitality = 5;
	this->dexterity = 5;
	this->intelligence = 5;

	this->hpMax = (this->vitality * 2) + (this->strength / 2) + (this->level*10);
	this->hp = this->hpMax;

	this->staminaMax = this->vitality + (this->strength / 2) + (this->dexterity / 3);
	this->stamina = this->staminaMax;

	this->damageMin = this->strength;
	this->damageMax = this->strength + 3;
	this->defence = this->dexterity + (this->intelligence / 2);
	this->accuracy = ((this->dexterity) / 2) + intelligence;
	this->luck = this->intelligence;

	this->statPoints = 0;
	this->actionPoints = level * 2;
	this->updateStats();
}

//Print all Character Attributes
void Character::printStats() const {
	cout << "---- Character Sheet ----" << endl;
	cout << " Name: " << this->name << endl;
	cout << " Level: " << this->level << endl;
	cout << " Experience: " << this->exp << endl;
	cout << " Experience to Next Level: " << this->expNext << endl;
	cout << " Available Stat Points: " << this->statPoints << endl;
	cout << "-------------------------" << endl;
	cout << " Strength: " << this->strength << endl;
	cout << " Vitality: " << this->vitality << endl;
	cout << " Dexterity: " << this->dexterity << endl;
	cout << " Intelligence: " << this->intelligence << endl;
	cout << "-------------------------" << endl;
	cout << " HP: " << this->hp << " / " << this->hpMax << endl;
	cout << " Stamina: " << this->stamina << " / " << this->staminaMax << endl;
	cout << " Damage: " << this->damageMin << " - " << this->damageMax << endl;
	cout << " Defence: " << this->defence << endl;
	cout << " Accuracy: " << this->accuracy << endl;
	cout << " Luck: " << this->luck << endl;
	cout << "-------------------------" << endl;
	cout << " Gold: " << this->gold << endl;
	cout << " Distance Travelled: " << this->distanceTravelled << endl;
	cout << "-------------------------" << endl;

	// Show equipped weapon
	Weapon* equippedWeapon = getEquippedWeapon();
	cout << " Equipped Weapon:" << endl;

	if (equippedWeapon) {
		
		cout << " Name: " << equippedWeapon->getName() << endl;
		cout << " Damage: " << equippedWeapon->getDamageMin() << " - " << equippedWeapon->getDamageMax() << " \n" << endl;
		
	}

	else {
		cout << " None" << endl;
	}

	// Show equipped armor
	cout << " Equipped Armor: ";
	if (equippedArmor) {
		cout << equippedArmor->getName() << endl;
		cout << " (Defence: " << equippedArmor->getDefence() << ")\n" << endl;
	}

	else {
		cout << "None\n" << endl;
	}
}

//Update Stats when level up
void Character::updateStats()
{
	this->expNext =
		static_cast<int>((static_cast<double>(50) / 3) * ((pow(level, 3) - 6 * pow(level, 2)) + 17 * level - 12)) + 100;

	this->hpMax = (this->vitality * 2) + (this->strength / 2) + (this->level * 10);
	this->hp = this->hpMax;

	this->staminaMax = this->vitality + (this->strength / 2) + (this->dexterity / 3);

	this->damageMin = this->strength;
	this->damageMax = this->strength + 3;
	this->defence = this->dexterity + (this->intelligence / 2);
	this->accuracy = (this->dexterity) * 2;
	this->luck = this->intelligence;
}

//Upgrade Stats 
void Character::addToStat(int stat, int value)
{
	if (this->statPoints < value)
	{
		cout << "Error! Not Enough Statpoints! " << "\n";
	}

	else
	{
		switch (stat)
		{
		case 0:
			this->strength += value;
			cout << " Strength Increased! " << "\n";
			break;

		case 1:
			this->vitality += value;
			cout << " Vitality Increased! " << "\n";
			break;

		case 2:
			this->dexterity += value;
			cout << " Dexterity Increased! " << "\n";
			break;

		case 3:
			this->intelligence += value;
			cout << " Intelligence Increased! " << "\n";
			break;

		default:
			cout << " Invalid input. " << "\n";
			break;
		}
		this->statPoints -= value;

		this->updateStats();
	}
}

void Character::levelUp()
{
	if (this->exp >= this->expNext)
	{
		this->exp -= this->expNext;
		this->level++;
		this->expNext =
			static_cast<int>((static_cast<double>(50) / 3) * ((pow(level, 3) - 6 * pow(level, 2)) + 17 * level - 12)) + 100;

		this->statPoints++;

		
		this->updateStats();
		

		cout << " You are now Level: " << this->level << " ! " << "\n\n";
	}

	else 
	{
		cout << " NOT ENOUGH EXP! \n\n"; 
	}
}

string Character::getAsString() const
{
	return name + " "
		+ to_string(distanceTravelled) + " "
		+ to_string(gold) + " "
		+ to_string(level) + " "
		+ to_string(exp) + " "
		+ to_string(strength) + " "
		+ to_string(vitality) + " "
		+ to_string(dexterity) + " "
		+ to_string(intelligence) + " "
		+ to_string(hp) + " "
		+ to_string(stamina) + " "
		+ to_string(statPoints);

}


string Character::getInventoryAsString() const {
	std::stringstream ss;
	for (int i = 0; i < inventory.size(); ++i) {
		Item* item = inventory.getItem(i);
		if (item != nullptr) {
			ss << "\n Item Number: " << i + 1 << "\n" // Adding the item number
				<< item->toString() << "\n";
		}
	}
	return ss.str();
}

Item* Character::getInventoryItem(int index) const
{
	if (index >= 0 && index < this->inventory.size())
	{
		return this->inventory.getItem(index);
	}

	return nullptr;
}

int Character::getInventorySize() const
{
	return this->inventory.size();
}

void Character::takeDamage(int damage)
{
	this->hp -= damage;

	if (this->hp <= 0)
	{
		this->hp = 0;
	}


}

void Character::usePotion(int index)
{
	if (index < 0 || index >= this->inventory.size())
	{
		cout << "Invalid index. No potion at this index." << endl;
		return;
	}

	Potion* potion = dynamic_cast<Potion*>(this->inventory.getItem(index));
	if (potion)
	{
		applyPotionEffects(*potion);
		inventory.removeItem(index);
		cout << "Potion used successfully." << endl;
	}
	else
	{
		cout << "Invalid selection. No potion at this index." << endl;
	}
}


void Character::applyPotionEffects(const Potion& potion)
{

	switch (potion.getType())
	{
	case Potion::PotionType::Health:
		this->hp += potion.getPotency();
		if (this->hp > this->hpMax)
			this->hp = this->hpMax;
		break;

	case Potion::PotionType::Strength:
		this->strength += potion.getPotency();
		break;

	case Potion::PotionType::Vitality:
		this->vitality += potion.getPotency();
		break;

	case Potion::PotionType::Dexterity:
		this->dexterity += potion.getPotency();
		break;

	case Potion::PotionType::Intelligence:
		this->intelligence += potion.getPotency();
		break;
	}
}

void Character::equipWeapon(int index) {
	if (index < 0 || index >= this->inventory.size()) {
		cout << "Invalid index." << endl;
		return;
	}

	Weapon* weaponToEquip = dynamic_cast<Weapon*>(this->inventory.getItem(index));
	if (!weaponToEquip) {
		cout << "No weapon at this index." << endl;
		return;
	}

	Weapon* clonedWeapon = weaponToEquip->clone();
	if (!clonedWeapon) {
		cout << "Failed to clone weapon." << endl;
		return;
	}

	if (this->equippedWeapon) {
		this->inventory.addItem(*this->equippedWeapon);
		delete this->equippedWeapon;
	}

	this->equippedWeapon = clonedWeapon;
	this->damageMin = clonedWeapon->getDamageMin();
	this->damageMax = clonedWeapon->getDamageMax();

	this->inventory.removeItem(index);

	cout << "Weapon equipped: " << clonedWeapon->getName() << endl;
}

void Character::equipArmor(int index) {
	if (index < 0 || index >= this->inventory.size()) {
		cout << "Invalid index." << endl;
		return;
	}

	Armor* armorToEquip = dynamic_cast<Armor*>(this->inventory.getItem(index));
	if (armorToEquip) {
		// Clone the armor to equip
		Armor* clonedArmor = dynamic_cast<Armor*>(armorToEquip->clone());

		// If there's already an equipped armor, add it back to the inventory
		if (this->equippedArmor) {
			this->inventory.addItem(*this->equippedArmor);
			delete this->equippedArmor; // Delete the old equipped armor
		}

		// Equip the cloned armor and update the character's defense
		this->equippedArmor = clonedArmor;
		this->defence = clonedArmor->getDefence();

		// Remove the original armor from the inventory
		this->inventory.removeItem(index);

		cout << "Armor equipped." << endl;
	}
	else {
		cout << "No armor at this index." << endl;
	}
}


Weapon* Character::getEquippedWeapon() const
{
	return this->equippedWeapon;
}

Armor* Character::getEquippedArmor() const
{
	return this->equippedArmor;
}


void Character::useActionPoints(int amount)
{
	this->actionPoints -= amount;
	if (this->actionPoints < 0)
	{
		this->actionPoints = 0;
	}
}

int Character::getActionPoints() const
{
	return this->actionPoints;
}

void Character::resetActionPoints()
{
	this->actionPoints = level*2;
}

int Character::getStatValue(const string& statName) const {
	if (statName == "Strength") {
		return this->strength;
	}
	else if (statName == "Vitality") {
		return this->vitality;
	}
	else if (statName == "Dexterity") {
		return this->dexterity;
	}
	else if (statName == "Intelligence") {
		return this->intelligence;
	}
	else {
		cout << "Stat not found: " << statName << endl;
		return 0;
	}
}


void Character::increaseHP(int amount) {
	this->hp += amount;
	if (this->hp > this->hpMax) {
		this->hp = this->hpMax; // Ensure HP does not exceed maximum
	}
}


void Character::increaseStat(const string& statName, int value) {
	if (statName == "Strength") {
		this->strength += value;
	}
	else if (statName == "Vitality") {
		this->vitality += value;
	}
	else if (statName == "Dexterity") {
		this->dexterity += value;
	}
	else if (statName == "Intelligence") {
		this->intelligence += value;
	}
	else {
		std::cout << "Unknown stat: " << statName << std::endl;
	}
	// Update other dependent attributes if necessary
	this->updateStats();
}


void Character::renderStats(sf::RenderWindow& window, sf::Font& font, float x, float y) const {
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);
	text.setPosition(x, y);

	std::stringstream ss;
	ss << "---- Character Sheet ----\n"
		<< " Name: " << this->name << "\n"
		<< " Level: " << this->level << "\n"
		<< " Experience: " << this->exp << "\n"
		<< " Experience to Next Level: " << this->expNext << "\n"
		<< " Available Stat Points: " << this->statPoints << "\n"
		<< "-------------------------" << "\n"
		<< " Strength: " << this->strength << "\n"
		<< " Vitality: " << this->vitality << "\n"
		<< " Dexterity: " << this->dexterity << "\n"
		<< " Intelligence: " << this->intelligence << "\n"
		<< "-------------------------" << "\n"
		<< " HP: " << this->hp << " / " << this->hpMax << "\n"
		<< " Stamina: " << this->stamina << " / " << this->staminaMax << "\n"
		<< " Damage: " << this->damageMin << " - " << this->damageMax << "\n"
		<< " Defence: " << this->defence << "\n"
		<< " Accuracy: " << this->accuracy << "\n"
		<< " Luck: " << this->luck << "\n"
		<< " Distance Travelled: " << this->distanceTravelled << "\n"
		<< "-------------------------" << "\n"
		<< " Gold: " << this->gold << "\n"
		<< " Equipped Weapon: " << (this->equippedWeapon ? this->equippedWeapon->getName() : "None") << "\n"
		<< " Equipped Armor: " << (this->equippedArmor ? this->equippedArmor->getName() : "None") << "\n";

	text.setString(ss.str());
	window.draw(text);
}
