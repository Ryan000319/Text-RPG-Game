#pragma once

#include "STLInclude.h"

//Different Enemy types
enum class EnemyType
{
	DreadKnight = 0,
	ForestWraith,
	RockTroll,
	CrystalDragon, 
	MinotaurWarlord,
	FireGolem,
	NumTypes
};


class Enemy
{
public:
	Enemy(EnemyType type, int level);
	virtual ~Enemy();

	inline bool isAlive() const { return this->hp > 0; }
	string getAsString() const;
	void takeDamage(int damage);
	void allocateActionPoints();

	inline const string& getName() const { return this->name; }
	inline int getLevel() const { return this->level; }
	inline int getDamage() const { return rand() % this->damageMax + this->damageMin; } 
	inline int getExp() const { return this->level * 100; }
	inline int getHp() const { return this->hp; }
	inline int getHpMax() const { return this->hpMax; }
	inline int getDefence() const { return this->defence; }
	inline int getAccuracy() const { return this->accuracy; }

	void render(sf::RenderWindow& window, sf::Font& font, float x, float y) const;

private:
	string name;
	int level;
	int hp;
	int hpMax;
	int damageMin;
	int damageMax;
	int dropChance;
	int defence;
	int accuracy;
	int actionPoints;

	EnemyType type;

};

