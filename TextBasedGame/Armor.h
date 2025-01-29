#pragma once
#include "STLInclude.h"
#include "Item.h"

class Armor : public Item
{
public:
    Armor();
    Armor(int level, int rarity);
    Armor(int defence, 
        string name, 
        int level, 
        int buyValue, 
        int sellValue, 
        int rarity);

    virtual ~Armor();

    // Pure virtual
    virtual Armor* clone() const override;

    // Functions
    string toString() const;

    // Accessors
    inline int getDefence() const { return this->defence; }

    // Modifiers
    static dynaArr<string> names; //using Dynamic Array to store Armor names
    static void initNames(); //Initialise unique Armor names

    

private:
    int defence;
};

