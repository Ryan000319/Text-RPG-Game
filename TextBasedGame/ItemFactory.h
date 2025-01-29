#pragma once
#include "Weapon.h"
#include "Armor.h"
#include "Potion.h"

//Factory Design Pattern
class ItemFactory {
public:
    virtual ~ItemFactory() {}
    virtual Item* createItem(int level, int rarity) const = 0;
};

class WeaponFactory : public ItemFactory {
public:
    Item* createItem(int level, int rarity) const override {
        return new Weapon(level, rarity);
    }
};

class ArmorFactory : public ItemFactory {
public:
    Item* createItem(int level, int rarity) const override {
        return new Armor(level, rarity);
    }
};

class PotionFactory : public ItemFactory {
public:
    Item* createItem(int level, int rarity) const override {
        Potion::PotionType type = static_cast<Potion::PotionType>(rand() % 6);
        int potency = level * 5;
        return new Potion(Potion::namesMap[type], level, rarity, type, potency);
    }
};


