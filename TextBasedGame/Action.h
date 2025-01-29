#pragma once

#include "STLInclude.h"
#include "Enemy.h"

//forward declaration Character
class Character;


enum class ActionType
{
	ATTACK, //Attack by Character
	ENEMY_ATTACK //Attack by Enemy
};

struct Action {
    ActionType type;
    Character* attacker;
    Enemy* targetEnemy;
    int damage;
    string message;

    // Updated constructor
    Action(ActionType type, Character* attacker, Enemy* targetEnemy, int damage, string message = "")
        : type(type), attacker(attacker), targetEnemy(targetEnemy), damage(damage), message(move(message)) {}
};
