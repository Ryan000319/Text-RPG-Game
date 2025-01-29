#pragma once

#include "STLInclude.h"
#include "Character.h"
#include "Enemy.h"
#include "Puzzle.h"
#include "Action.h"
#include "Decision.h"
#include "ItemFactory.h"


enum class PlayerAction {
    None,
    Attack,
    UseItem,
    EndTurn
};


class Event {
public:
    Event();
    virtual ~Event();

    void enemyEncounter(sf::RenderWindow& window, sf::Font& font, Character& character, SinglyLinkedNode<Enemy>*& enemies);
    void puzzleEncounter(sf::RenderWindow& window, sf::Font& font, Character& character);
    void handlePlayerTurn(std::stringstream& ss, sf::RenderWindow& window, sf::Font& font, Character& character, SinglyLinkedNode<Enemy>*& enemyListHead, bool& escape);
    void handleEnemyTurn(std::stringstream& ss, sf::RenderWindow& window, sf::Font& font, Character& character, SinglyLinkedNode<Enemy>*& enemyListHead, bool& playerDefeated);
    void executeAttackAction(sf::RenderWindow& window, sf::Font& font, Character& character, Enemy* targetedEnemy);
    void processActionQueue(sf::RenderWindow& window, sf::Font& font, Character& character);
    void handleEnemyDefeat(sf::RenderWindow& window, sf::Font& font, Character& character, Enemy* defeatedEnemy);

    unique_ptr<BTree<Decision>> createDungeonAdventure(Character& character);
    unique_ptr<BTree<Decision>> createTempleAdventure(Character& character);
    void processDecision(sf::RenderWindow& window, sf::Font& font, unique_ptr<BTree<Decision>>&& currentNode, Character& character);
    void displayQueuedActions(std::stringstream& ss, Character& character);
    void displayBattleState(std::stringstream& ss, Character& character, SinglyLinkedNode<Enemy>*& enemyListHead);

    void displayText(sf::RenderWindow& window, sf::Font& font, const std::string& message, bool waitForKeyPress);
    void displayCombatLog(std::stringstream& ss, Character& character);
    void waitForKeyPress(sf::RenderWindow& window);
    void updateWindow(sf::RenderWindow& window, sf::Font& font, std::stringstream& ss);
    
private:
    Queue<Action> actionQueue;
    dynaStack<Action> combatLog;

    void displayEnemies(std::stringstream& ss, SinglyLinkedNode<Enemy>*& enemyListHead);
    void displayCharacterStats(std::stringstream& ss, Character& character);
    void displayBattleMenu(std::stringstream& ss);
    void handlePlayerInput(sf::RenderWindow& window, sf::Font& font, std::stringstream& ss, Character& character, SinglyLinkedNode<Enemy>*& enemyListHead, bool& escape);
    void handleAttackAction(sf::RenderWindow& window, sf::Font& font, std::stringstream& ss, Character& character, SinglyLinkedNode<Enemy>*& enemyListHead);
    void queueAttackAction(Character& character, Enemy* targetedEnemy);
    void handleUseItemAction(sf::RenderWindow& window, sf::Font& font, std::stringstream& ss, Character& character);
    void handleEndTurnAction(sf::RenderWindow& window, sf::Font& font, std::stringstream& ss, Character& character, SinglyLinkedNode<Enemy>*& enemyListHead, bool& escape);
    bool checkEndConditions(Character& character, SinglyLinkedNode<Enemy>*& enemyListHead, bool& escape);
    void prepareNextPlayerTurn(std::stringstream& ss, sf::RenderWindow& window, sf::Font& font, Character& character, SinglyLinkedNode<Enemy>*& enemyListHead);
    void performEnemyAttack(std::stringstream& ss, Character& character, Enemy& enemy);
    void logAttack(Enemy& enemy, Character& character, int damage);
    void logMiss(Enemy& enemy, Character& character);
    int determineItemRarity();
    int getPlayerChoice(sf::RenderWindow& window);

    unique_ptr<Item> generateRandomItem(int enemyLevel);
    string actionToString(const Action& action, Character& character);
};

