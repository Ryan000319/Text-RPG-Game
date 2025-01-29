#pragma once
#include "STLInclude.h"
#include "Inventory.h"
#include "Action.h"

class Character
{
public:
	Character();
	Character(string name,
		int distanceTravelled,
		int gold, int level,
		int exp, int strength,
		int vitality, int dexterity,
		int intelligence, int hp,
		int stamina, int statPoints);
	virtual ~Character();

	// Functions
	void initialize(const string name);
	void printStats() const;
	void levelUp();
	void updateStats();
	void addToStat(int stat, int value);
	inline void resetHP() { this->hp = this->hpMax; }
	void addItem(const Item& item) { this->inventory.addItem(item); }
	void usePotion(int index);
	void equipWeapon(int index);
	void equipArmor(int index);
	void applyPotionEffects(const Potion& potion);
	void useActionPoints(int amount);
	int getActionPoints() const;
	void resetActionPoints();
	void increaseHP(int amount);
	void increaseStat(const string& statName, int value);


	//Accessors
	inline const int& getDistanceTravelled() const { return this->distanceTravelled; }
	inline const string& getName() const { return this->name; }
	inline const int& getLevel() const { return this->level; }
	inline const int& getExp() const { return this->exp; }
	inline const int& getExpNext() const { return this->expNext; }
	inline const int& getStatPoints() const { return this->statPoints; }
	inline const int& getHp() const { return this->hp; }
	inline const int& getHpMax() const { return this->hpMax; }
	inline bool isAlive() { return this->hp > 0; }
	inline const int& getStamina() const { return this->stamina; }
	inline const int& getDamageMin() const { return this->damageMin; }
	inline const int& getDamageMax() const { return this->damageMax; }
	inline const int getDamage() const { return rand() % this->damageMax + this->damageMin; }
	inline const int& getDefence() const { return this->defence; }
	inline const int& getAccuracy() const { return this->accuracy; }
	inline const int& getGold() const { return this->gold; }
	int getStatValue(const string& statName) const;
	string getAsString() const;
	string getInventoryAsString() const;
	Item* getInventoryItem(int index) const;
	int getInventorySize() const;
	Weapon* getEquippedWeapon() const;
	Armor* getEquippedArmor() const;
	

	// Modifier
	inline void setDistanceTravelled(const int& distance) { this->distanceTravelled = distance; }
	inline void travel() { this->distanceTravelled++; }
	inline void gainExp(const int& exp) { this->exp += exp; }
	inline void gainGold(const int& gold) { this->gold += gold; }
	inline void payGold(const int gold) { this->gold -= gold; }
	void takeDamage(int damage);
	inline void retry() { this->hp = hpMax; }

	//Print to S
	void renderStats(sf::RenderWindow& window, sf::Font& font, float x, float y) const;

private:
	int distanceTravelled;

	Inventory inventory;
	Weapon weapon;
	Armor armor;

	Weapon* equippedWeapon;
	Armor* equippedArmor;

	string name;
	int level;
	int exp;
	int gold;
	int expNext;

	int strength;
	int vitality;
	int dexterity;
	int intelligence;

	int hp;
	int hpMax;
	int stamina;
	int staminaMax;
	int damageMin;
	int damageMax;
	int defence;
	int accuracy;
	int luck;

	int statPoints;

	int actionPoints;
};



