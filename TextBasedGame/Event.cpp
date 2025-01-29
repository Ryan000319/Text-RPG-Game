#include "Event.h"

//Constructor and Desctructor
Event::Event() {}
Event::~Event() {}

//Display text in SFML window
void Event::displayText(sf::RenderWindow& window, sf::Font& font, const std::string& message, bool waitForKeyPress) {
	sf::Text text;
	text.setFont(font);
	text.setString(message);
	text.setCharacterSize(30);
	text.setFillColor(sf::Color::White);
	text.setPosition(50, 200); 

	window.clear();
	window.draw(text);
	window.display();

	if (waitForKeyPress) {
		sf::Event event;
		while (window.waitEvent(event)) {
			if (event.type == sf::Event::KeyPressed) {
				break;
			}
		}
	}
}

void Event::enemyEncounter(sf::RenderWindow& window, sf::Font& font, Character& character, SinglyLinkedNode<Enemy>*& enemies)
{
	// Initialize control variables
	bool playerTurn = true;
	bool enemyTurn = false;
	bool escape = false;
	bool playerDefeated = false;
	bool enemiesDefeated = false;

	// Generate enemies
	int numOfEnemies = rand() % 4 + 1;
	EnemyType firstEnemyType = static_cast<EnemyType>(rand() % static_cast<int>(EnemyType::NumTypes));
	SinglyLinkedNode<Enemy>* enemyListHead = new SinglyLinkedNode<Enemy>(Enemy(firstEnemyType, character.getLevel() + rand() % 2), nullptr);
	SinglyLinkedNode<Enemy>* lastNode = enemyListHead;
	for (int i = 1; i < numOfEnemies; i++) {
		EnemyType enemyType = static_cast<EnemyType>(rand() % static_cast<int>(EnemyType::NumTypes));
		SinglyLinkedNode<Enemy>* newNode = new SinglyLinkedNode<Enemy>(Enemy(enemyType, character.getLevel() + rand() % 2), nullptr);
		lastNode->append(newNode);
		lastNode = newNode;
	}

	// Game loop
	while (!escape && !playerDefeated && !enemiesDefeated) {
		std::stringstream ss;

		// Handle turns
		if (playerTurn && !playerDefeated) {
			handlePlayerTurn(ss, window, font, character, enemyListHead, escape);
		}
		else if (enemyTurn && !playerDefeated) {
			handleEnemyTurn(ss, window, font, character, enemyListHead, playerDefeated);
		}

		// Check end conditions
		playerDefeated = !character.isAlive();
		enemiesDefeated = (countNodes(enemyListHead) <= 0);

		// Clean up if the encounter is over
		if (escape || playerDefeated || enemiesDefeated) {
			while (enemyListHead != nullptr) {
				SinglyLinkedNode<Enemy>* nextNode = enemyListHead->getNext();
				delete enemyListHead;
				enemyListHead = nextNode;
			}
			enemyListHead = nullptr;
		}
	}
}


//Player Turn Handling
void Event::handlePlayerTurn(std::stringstream& ss, sf::RenderWindow& window, sf::Font& font, Character& character, SinglyLinkedNode<Enemy>*& enemyListHead, bool& escape) {
	// Display player's turn options
	ss.str(""); // Clear the stringstream for new input for each turn

	ss << "\n ----------------------- CHARACTER TURN ----------------------- \n";
	ss << "YOU'VE ENCOUNTERED " << countNodes(enemyListHead) << " ENEMY/ENEMIES!\n";

	// Display current enemies
	displayEnemies(ss, enemyListHead);
	
	// Display character stats and queued actions
	displayCharacterStats(ss, character);
	
	// Display Queued Actions
	displayQueuedActions(ss, character);

	//Display Combat Log
	displayCombatLog(ss, character);

	// Display battle menu
	displayBattleMenu(ss);
	
	//Update SFML window
	updateWindow(window, font, ss);

	// Handle player input and actions
	handlePlayerInput(window, font, ss, character, enemyListHead, escape);

	
}

//Display Enemies
void Event::displayEnemies(std::stringstream& ss, SinglyLinkedNode<Enemy>*& enemyListHead) {

	
	SinglyLinkedNode<Enemy>* currentNode = enemyListHead;
	int index = 1;
	while (currentNode != nullptr) {
		Enemy& enemy = currentNode->getValue();
		if (enemy.isAlive()) {
			ss << " " << index << ": " << enemy.getName() << " Lvl: " << enemy.getLevel()
				<< " - (HP: [" << enemy.getHp() << "/" << enemy.getHpMax() << "] | DEF: "
				<< enemy.getDefence() << " | ACC: " << enemy.getAccuracy() << ")\n";
		}
		currentNode = currentNode->getNext();
		index++;
	}
}

void Event::displayQueuedActions(std::stringstream& ss, Character& character) {
	Queue<Action> tempQueue = this->actionQueue;
	
	ss << "\n ------------------------- Queued Actions ------------------------- \n";
	while (!tempQueue.isEmpty()) {
		Action currentAction = tempQueue.dequeue();
		ss << actionToString(currentAction, character) << "\n";
	}
}

void Event::displayCombatLog(std::stringstream& ss, Character& character) {
	
	ss << "\n ------------------------- Combat Log ------------------------- \n";
	if (combatLog.isEmpty()) {
		ss << "\nNo actions in the log.\n";
		
	}

	// Temporary stack to hold actions while displaying them
	dynaStack<Action> tempStack;

	// Display and transfer actions to the temporary stack
	while (!combatLog.isEmpty()) {
		Action action = combatLog.pop();
		ss << action.message << endl;  // Concatenate the custom message
		tempStack.push(action);
	}

	// Transfer actions back to the original stack
	while (!tempStack.isEmpty()) {
		combatLog.push(tempStack.pop());
	}
}

//Character stats display
void Event::displayCharacterStats(std::stringstream& ss, Character& character) {
	ss << "\n --------------------------- Quick Stats ------------------------- \n";
	ss << "Current Character HP: " << character.getHp() << " / " << character.getHpMax() << "\n";

	// Equipped weapon and armor
	Weapon* equippedWeapon = character.getEquippedWeapon();
	Armor* equippedArmor = character.getEquippedArmor();
	ss << "Equipped Weapon: " << (equippedWeapon ? equippedWeapon->getName() : "None") << "\n";
	ss << "Equipped Armor: " << (equippedArmor ? equippedArmor->getName() : "None") << "\n";

	// Action Points
	ss << "Action Points: " << character.getActionPoints() << "\n";
}

//Battle Menu Display
void Event::displayBattleMenu(std::stringstream& ss) {
	ss << "\n ------------------------- Battle Menu ------------------------- \n";
	ss << "1: Escape\n";
	ss << "2: Attack\n";
	ss << "3: Use Item\n";
	ss << "4: End Turn\n";
}

//Player Input Handling
void Event::handlePlayerInput(sf::RenderWindow& window, sf::Font& font, std::stringstream& ss, Character& character, SinglyLinkedNode<Enemy>*& enemyListHead, bool& escape) {
	int choice = 0;
	sf::Event event;
	PlayerAction nextAction = PlayerAction::None;

	//displayQueuedActions(window, font, character);
	updateWindow(window, font, ss);

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}

		if (event.type == sf::Event::KeyPressed) {
			switch (event.key.code) {
			case sf::Keyboard::Num1:
				escape = true;
				return;
			case sf::Keyboard::Num2:
				nextAction = PlayerAction::Attack;
				break;
			case sf::Keyboard::Num3:
				nextAction = PlayerAction::UseItem;
				break;
			case sf::Keyboard::Num4:
				nextAction = PlayerAction::EndTurn;
				break;
			default:
				break; // No valid key was pressed
			}
		}
	}

	// Perform the action after polling
	switch (nextAction) {
	case PlayerAction::Attack:
		handleAttackAction(window, font, ss, character, enemyListHead);
		break;
	case PlayerAction::UseItem:
		handleUseItemAction(window, font, ss, character);
		break;
	case PlayerAction::EndTurn:
		handleEndTurnAction(window, font, ss, character, enemyListHead, escape);
		break;
	default:
		break; 
	}
}

//Attack Action Handling
void Event::handleAttackAction(sf::RenderWindow& window, sf::Font& font, std::stringstream& ss, Character& character, SinglyLinkedNode<Enemy>*& enemyListHead) {
	if (character.getActionPoints() < 2) {
		ss << "Not enough action points to attack.\n";
		updateWindow(window, font, ss);
		return; // Not enough action points
	}

	// Display enemies to attack
	ss << "\n Choose an enemy (Press ENTER after selecting enemy number): ";
	
	updateWindow(window, font, ss);

	// Get user choice
	int choice = getPlayerChoice(window);
	cout << choice;

	// Validate choice
	if (choice < 1 || choice > countNodes(enemyListHead)) {
		ss << "Invalid enemy selection.\n";
		updateWindow(window, font, ss);
		return;
	}

	// Get targeted enemy
	Enemy* targetedEnemy = enemyListHead->get(choice - 1);

	if (!targetedEnemy || !targetedEnemy->isAlive()) {
		ss << "Targeted enemy is invalid or already defeated.\n";
		updateWindow(window, font, ss);
		return;
	}

	// Queue attack action
	queueAttackAction(character, targetedEnemy);
	ss << "Attack action queued against " << targetedEnemy->getName() << ".\n";
	
	updateWindow(window, font, ss);
}



void Event::queueAttackAction(Character& character, Enemy* targetedEnemy) {
	if (!targetedEnemy || !targetedEnemy->isAlive()) {
		// Handle the case where the targeted enemy is invalid or already defeated
		std::cout << "Cannot attack. Target enemy is invalid or already defeated." << std::endl;
		return;
	}

	// Create an attack action targeting the selected enemy
	Action attackAction(ActionType::ATTACK, &character, targetedEnemy, -1, "Attack action queued");

	// Add the attack action to the queue
	this->actionQueue.enqueue(attackAction);

	// Deduct action points required for the attack
	character.useActionPoints(2);
}

void Event::handleUseItemAction(sf::RenderWindow& window, sf::Font& font, std::stringstream& ss, Character& character) {
	ss.str(""); // Clear the stringstream
	ss << "Select an item to use:\n";
	ss << character.getInventoryAsString(); 
	updateWindow(window, font, ss);

	// Get user choice for the item
	int choice = getPlayerChoice(window);

	// Validate the choice
	if (choice < 1 || choice > character.getInventorySize()) {
		ss << "Invalid item selection. Please try again.\n";
		updateWindow(window, font, ss);
		return;
	}

	// Get the selected item from the character's inventory
	Item* selectedItem = character.getInventoryItem(choice - 1);
	if (!selectedItem) {
		ss << "Invalid item selection.\n";
		updateWindow(window, font, ss);
		return;
	}

	// Apply the item's effect
	if (auto potion = dynamic_cast<Potion*>(selectedItem)) {
		character.usePotion(choice - 1);
		ss << "Used " << potion->getName() << ".\n";
	}
	else if (auto weapon = dynamic_cast<Weapon*>(selectedItem)) {
		character.equipWeapon(choice - 1);
		ss << "Equipped " << weapon->getName() << ".\n";
	}
	else if (auto armor = dynamic_cast<Armor*>(selectedItem)) {
		character.equipArmor(choice - 1);
		ss << "Equipped " << armor->getName() << ".\n";
	}
	else {
		ss << "Item not usable.\n";
	}

	updateWindow(window, font, ss);
}

void Event::updateWindow(sf::RenderWindow& window, sf::Font& font, std::stringstream& ss) {
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);
	text.setString(ss.str());
	text.setPosition(200, 50); 

	window.clear();
	window.draw(text);
	window.display();
}

int Event::getPlayerChoice(sf::RenderWindow& window) {
	std::string inputStr;
	sf::Event event;

	// Use a loop to process events
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			// Check for number key presses
			if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode >= '0' && event.text.unicode <= '9') {
					// Append the character to the input string
					inputStr += static_cast<char>(event.text.unicode);
				}
			}

			// Check for Enter key press to finalize the choice
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
				// Convert the string to an integer and return
				return !inputStr.empty() ? std::stoi(inputStr) : 0; // Return 0 if inputStr is empty
			}
		}
	}

	return 0; // Return 0 if window is closed
}

void Event::handleEndTurnAction(sf::RenderWindow& window, sf::Font& font, std::stringstream& ss, Character& character, SinglyLinkedNode<Enemy>*& enemyListHead, bool& escape) {
	// Process all queued actions
	processActionQueue(window, font, character);

	// Reset the character's action points for the next turn
	character.resetActionPoints();
	ss.str(""); // Clear the stringstream
	
	// Check if the encounter is over (either all enemies are defeated or the player has escaped)
	if (checkEndConditions(character, enemyListHead, escape)) {
		return; // Exit the encounter if it's over
	}

	// Handle the enemy turn
	bool playerDefeated = false;
	handleEnemyTurn(ss, window, font, character, enemyListHead, playerDefeated);
	if (playerDefeated) {
		ss.str("");
		ss << "You have been defeated in battle!";
		updateWindow(window, font, ss);
		// Handle player defeat 
	}

	// Prepare for the next player turn
	prepareNextPlayerTurn(ss, window, font, character, enemyListHead);
}

bool Event::checkEndConditions(Character& character, SinglyLinkedNode<Enemy>*& enemyListHead, bool& escape) {
	// Check if all enemies are defeated
	bool allEnemiesDefeated = true;
	SinglyLinkedNode<Enemy>* currentNode = enemyListHead;
	while (currentNode != nullptr) {
		if (currentNode->getValue().isAlive()) {
			allEnemiesDefeated = false;
			break;
		}
		currentNode = currentNode->getNext();
	}

	// Check if the player has escaped or is defeated
	if (escape || !character.isAlive() || allEnemiesDefeated) {
		return true; // Encounter is over
	}

	return false; // Encounter is not over
}

void Event::displayBattleState(std::stringstream& ss, Character& character, SinglyLinkedNode<Enemy>*& enemyListHead) {
	// Display character stats
	displayCharacterStats(ss, character);

	// Display current enemies
	ss << "\n------ Enemies ------\n";
	displayEnemies(ss, enemyListHead);
}

void Event::prepareNextPlayerTurn(std::stringstream& ss, sf::RenderWindow& window, sf::Font& font, Character& character, SinglyLinkedNode<Enemy>*& enemyListHead) {
	// Prepare the state for the next player turn
	character.resetActionPoints(); // Reset action points
	ss.str(""); // Clear the stringstream
	displayBattleState(ss, character, enemyListHead); // Update the battle state display
	//updateWindow(window, font, ss);
}



void Event::handleEnemyTurn(std::stringstream& ss, sf::RenderWindow& window, sf::Font& font, Character& character, SinglyLinkedNode<Enemy>*& enemyListHead, bool& playerDefeated) {
	ss.str(""); // Clear the stringstream
	ss << "\n----------- ENEMY TURN -------------\n";
	int enemyIndex = 1;

	// Iterate through each enemy in the list
	for (SinglyLinkedNode<Enemy>* currentEnemyNode = enemyListHead; currentEnemyNode != nullptr; currentEnemyNode = currentEnemyNode->getNext(), ++enemyIndex) {
		Enemy* enemy = &currentEnemyNode->getValue();

		if (!enemy->isAlive()) {
			continue; // Skip if the enemy is already defeated
		}

		ss << "Enemy " << enemyIndex << ": " << enemy->getName() << "\n";
		performEnemyAttack(ss, character, *enemy);

		if (!character.isAlive()) {
			ss << "CHARACTER DEFEATED!\n";
			playerDefeated = true;
			displayText(window, font, ss.str(), false); // Display the defeat message
			return; // Exit the function if the player is defeated
		}
	}

	ss << "--------- END ENEMY TURN -----------\n";

	displayText(window, font, ss.str(), true); // Display the summary of the enemy turn

}

void Event::performEnemyAttack(std::stringstream& ss, Character& character, Enemy& enemy) {
	// Combat calculations
	int combatTotal = enemy.getAccuracy() + character.getDefence();
	int enemyTotal = enemy.getAccuracy() / static_cast<double>(combatTotal) * 100;
	int playerTotal = character.getDefence() / static_cast<double>(combatTotal) * 100;

	int combatRollPlayer = rand() % playerTotal + 1;
	int combatRollEnemy = rand() % enemyTotal + 1;

	ss << "Player Roll: " << combatRollPlayer << " vs. Enemy Roll: " << combatRollEnemy << "\n";

	// Determine if the attack hits or misses
	if (combatRollPlayer < combatRollEnemy) {
		int damage = enemy.getDamage();
		character.takeDamage(damage);
		ss << enemy.getName() << " hits for " << damage << " damage!\n\n";
		logAttack(enemy, character, damage); // Log the attack in the combat log
	}
	else {
		ss << enemy.getName() << "'s attack missed!\n\n";
		logMiss(enemy, character); // Log the miss in the combat log
	}
}


void Event::waitForKeyPress(sf::RenderWindow& window) {
	sf::Event event;
	while (window.waitEvent(event)) {
		if (event.type == sf::Event::KeyPressed) {
			break;
		}
	}
}

void Event::logAttack(Enemy& enemy, Character& character, int damage) {
	std::stringstream message;
	message << enemy.getName() << " ATTACKS character - Damage: " << damage;
	combatLog.push(Action(ActionType::ENEMY_ATTACK, &character, &enemy, damage, message.str()));
}

void Event::logMiss(Enemy& enemy, Character& character) {
	std::stringstream message;
	message << enemy.getName() << " ATTACKS character - Missed";
	combatLog.push(Action(ActionType::ENEMY_ATTACK, &character, &enemy, 0, message.str()));
}




void Event::executeAttackAction(sf::RenderWindow& window, sf::Font& font, Character& character, Enemy* targetedEnemy) {
	if (!targetedEnemy || !targetedEnemy->isAlive()) {
		displayText(window, font, "Cannot attack. Target enemy is invalid or already defeated.", true);
		return;
	}


	// Calculate the base hit chance as a percentage of the character's accuracy
	int baseHitChance = character.getAccuracy() * 70 / 100; // 70% of accuracy as base hit chance

	// Subtract a portion of the enemy's defense (50% of defense)
	int defenseFactor = targetedEnemy->getDefence() * 50 / 100;

	// Calculate final hit chance
	int hitChance = max(0, baseHitChance - defenseFactor); // Ensure it doesn't go below 0
	hitChance += rand() % 100;

	// Roll for hit
	int hitRoll = rand() % 100 + 1;

	std::stringstream ss;

	ss << "Hit Roll: " << hitRoll << "\nHit Chance: " << hitChance;

	// Check if the attack hits
	if (hitRoll < hitChance) {
		int damage = character.getDamage();
		targetedEnemy->takeDamage(damage);

		ss << "\n----------- Player Turn -------------\n";
		ss << "ATTACK HIT! \n";
		ss << "\nCharacter ATTACKS " << targetedEnemy->getName() << " - Damage: " << damage;
		ss << "\n----------- End Player Turn -------------\n";
		displayText(window, font, ss.str(), true);

		ss.str(""); // clears ss
		ss << "\nCharacter ATTACKS " << targetedEnemy->getName() << " - Damage: " << damage;

		combatLog.push(Action(ActionType::ATTACK, &character, targetedEnemy, damage, ss.str()));

		if (!targetedEnemy->isAlive()) {
			handleEnemyDefeat(window, font, character, targetedEnemy);
		}
	}

	else {
		// Log message for a missed attack
		ss << "\n----------- Player Turn -------------\n";
		ss << "ATTACK  MISS! \n";
		ss << "Character ATTACKS " << targetedEnemy->getName() << " - Missed";
		ss << "\n----------- End Player Turn -------------\n";
		displayText(window, font, ss.str(), true);

		ss.str(""); // clears ss
		ss << "Character ATTACKS " << targetedEnemy->getName() << " - Missed";
		combatLog.push(Action(ActionType::ATTACK, &character, targetedEnemy, 0, ss.str()));
	}
}

void Event::processActionQueue(sf::RenderWindow& window, sf::Font& font, Character& character) {
	while (!this->actionQueue.isEmpty()) {
		Action currentAction = this->actionQueue.dequeue();
		if (currentAction.type == ActionType::ATTACK) {
			if (currentAction.targetEnemy && currentAction.targetEnemy->isAlive()) {
				executeAttackAction(window, font, character, currentAction.targetEnemy);
			}
			else {
				displayText(window, font, "Target enemy is no longer valid.", false);
			}
		}
	
		// Process window events
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				return; // Exit if the window is closed
			}
		}

		this_thread::sleep_for(chrono::seconds(1)); 
	}
	character.resetActionPoints();
}


void Event::handleEnemyDefeat(sf::RenderWindow& window, sf::Font& font, Character& character, Enemy* defeatedEnemy)
{
	if (!defeatedEnemy)
	{
		//Safety check to ensure the enemy pointer is valid
		return;
	}

	std::stringstream ss;

	//Gain Experience
	int expGain = defeatedEnemy->getExp();
	character.gainExp(expGain);

	//Gain Gold
	int goldGain = rand() % (defeatedEnemy->getLevel() * 10) + 10; //Random gold dropped based on level
	character.gainGold(goldGain);

	ss << "\n ------------- ENEMY DEFEATED! ---------------- \n";
	ss << "Enemy defeated! Gained " << expGain << " EXP and " << goldGain << " gold!\n";

	//Item drop chance
	int dropChance = rand() % 100;
	if (dropChance < 30) //30% chance to drop an item
	{
		//Randomly generate a weapon or armor based on enemy level
		unique_ptr<Item> droppedItem = generateRandomItem(defeatedEnemy->getLevel());

		if (droppedItem)
		{
			character.addItem(*droppedItem);
			ss << defeatedEnemy->getName() << " dropped " << droppedItem->getName() << " !\n";
		}
	}

	// Pause for player input
	displayText(window, font, ss.str(), true);
}

unique_ptr<Item> Event::generateRandomItem(int enemyLevel) {
	int itemType = rand() % 3;
	int rarity = determineItemRarity(); // Using the function to determine rarity

	ItemFactory* factory = nullptr;
	unique_ptr<Item> item = nullptr;

	switch (itemType) {
	case 0:
		factory = new WeaponFactory();
		break;
	case 1:
		factory = new ArmorFactory();
		break;
	case 2:
		factory = new PotionFactory();
		break;
	default:
		break;
	}

	if (factory) {
		item.reset(factory->createItem(enemyLevel, rarity));
		delete factory;
	}

	return item;
}


int Event::determineItemRarity()
{
	int chance = rand() % 100;
	if (chance < 50 && chance >= 0)
	{
		return COMMON;
	}

	else if (chance < 75 && chance > 50)
	{
		return UNCOMMON;
	}

	else if (chance < 90 && chance > 75)
	{
		return RARE;
	}

	else if (chance <= 100 && chance > 90)
	{
		return LEGENDARY;
	}
}

string Event::actionToString(const Action& action, Character& character) {
	stringstream ss;
	if (action.type == ActionType::ATTACK) {
		string attackerName = (action.attacker == &character) ? "Character" : (action.targetEnemy ? action.targetEnemy->getName() : "Defeated Enemy");
		string targetName = (action.attacker == &character) ? (action.targetEnemy ? action.targetEnemy->getName() : "Defeated Enemy") : "Character";
		string damageStr = action.damage == -1 ? "Pending" : to_string(action.damage);
		ss << attackerName << " ATTACKS " << targetName << " - Damage: " << damageStr;
	}

	return ss.str();
}

void Event::puzzleEncounter(sf::RenderWindow& window, sf::Font& font, Character& character) {
	unique_ptr<BTree<Decision>> currentAdventure;

	int adventureChoice = rand() % 2;

	switch (adventureChoice) {
	case 0:
		currentAdventure = createDungeonAdventure(character);
		break;
	case 1:
		currentAdventure = createTempleAdventure(character);
		break;
	}

	processDecision(window, font, std::move(currentAdventure), character);
}

void Event::processDecision(sf::RenderWindow& window, sf::Font& font, unique_ptr<BTree<Decision>>&& currentNode, Character& character) {
	if (!currentNode || currentNode->isEmpty()) {
		displayText(window, font, "End of the path. Exiting Puzzle Adventure.", true);
		return;
	}

	displayText(window, font, currentNode->key().text, true);

	if (currentNode->key().requiresCheck) {
		int characterStatValue = character.getStatValue(currentNode->key().stat);
		std::string checkMsg = "Checking " + currentNode->key().stat + " (" + std::to_string(characterStatValue) + ").\n Press ENTER to continue....";
		displayText(window, font, checkMsg, true);

		if (characterStatValue >= currentNode->key().checkThreshold) {
			displayText(window, font, currentNode->key().successText, true);

			if (currentNode->key().successEffect) {
				currentNode->key().successEffect->applyEffect(character, currentNode->key().droppedItem.get());
			}
			processDecision(window, font, std::move(currentNode->left()), character);

		}
		else {
			displayText(window, font, currentNode->key().failText, true);

			if (currentNode->key().failureEffect) {
				currentNode->key().failureEffect->applyEffect(character, currentNode->key().droppedItem.get());
			}
			if (!character.isAlive()) {
				displayText(window, font, "Your character has been defeated. \n Press ENTER to continue....", true);
				return;
			}
			processDecision(window, font, std::move(currentNode->right()), character);
		}
	}
	else {
		// Handling user input through SFML
		sf::Event event;
		while (window.waitEvent(event)) {
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Num1) {
					processDecision(window, font, std::move(currentNode->left()), character);
					break;
				}
				else if (event.key.code == sf::Keyboard::Num2) {
					processDecision(window, font, std::move(currentNode->right()), character);
					break;
				}
			}
		}
	}
}

unique_ptr<BTree<Decision>> Event::createDungeonAdventure(Character& character) {
	// Root of the dungeon adventure decision tree
	auto rootDecision = DecisionFactory::createDecision(
		"As you stand at the entrance of the ominous dungeon, you're faced with a choice:\n\n To your left, a dimly lit corridor stretches into shadowy unknowns,\n promising silent, creeping dangers. \n\nTo your right, the echoing sound of water \nhints at hidden depths and possibly treacherous paths.\n Which way will you choose to venture into the mysteries that lie ahead?.\n\nLeft: Head to the dimly lit corridor.\nRight: Head to the sound of water. \nChoose a path : \n1.Left\n2.Right",
		false, 0, "", "", "", nullptr, nullptr, nullptr);

	auto dungeonAdventure = make_unique<BTree<Decision>>(move(*rootDecision));

	// Left subtree: Dimly lit corridor
	auto leftSubtreeDecision = DecisionFactory::createDecision(
		"At the end of the dimly lit corridor, you discover a mysterious chest, \nits ancient wood and intricate lock whispering of long-forgotten secrets. \nBeside it stands a mystical barrier, shimmering with an ethereal light, \nguarding secrets perhaps even older than the chest itself. \nYou pause, weighing your decision. Do you dare open the chest and uncover its \nhidden treasures, or will you attempt to breach the mystical barrier, \nunveiling the arcane mysteries it conceals? The choice is yours, adventurer.\n\nLeft: Mysterious chest (Require Strength LVL 5)\nRight: Mystical barrier (Require Intelligence LVL 5)\nChoose a path : \n1.Left\n2.Right",
		false, 0, "", "", "", nullptr, nullptr, nullptr);
	auto leftSubtree = make_unique<BTree<Decision>>(move(*leftSubtreeDecision));

	// Generate an item
	unique_ptr<Item> foundItem = generateRandomItem(character.getLevel());

	// Left-Left: Mysterious chest
	auto chestDecision = DecisionFactory::createDecision(
		"A heavy chest requires strength to open. I hope you are strong enough adventurer.",
		true, 5, "Strength",
		"You open the chest to find treasure! Check your Inventory!\n Press ENTER to continue....",
		"The chest won't budge. You sprained an ankle. Took 5 Damage! \n Press ENTER to continue....",
		new FindItemEffect(),
		new DamageHealthEffect(5),
		foundItem.release());
	leftSubtree->attachLeft(make_unique<BTree<Decision>>(move(*chestDecision)));

	// Left-Right: Mystical barrier
	auto barrierDecision = DecisionFactory::createDecision(
		"An arcane barrier blocks your path.", true, 5, "Intelligence",
		"You dispel the barrier! You learned a new arcane art that may prove \nuseful in the future. +1 Intelligence \n Press ENTER to continue....", "The magic is too complex. You end up with a headahce. Took 5 Damage!\n Press ENTER to continue....",
		new IncreaseStatEffect("Intelligence", 5), new DamageHealthEffect(5), nullptr);
	leftSubtree->attachRight(make_unique<BTree<Decision>>(move(*barrierDecision)));

	// Attach left subtree to root
	dungeonAdventure->attachLeft(move(leftSubtree));

	// Right subtree: Sound of water
	auto rightSubtreeDecision = DecisionFactory::createDecision(
		"Sound of water.\nLeft: Suspicious water spring.\nRight: Narrow bridge (Require Dexterity Level 5).\nChoose a path : \n1.Left\n2.Right",
		false, 0, "", "", "", nullptr, nullptr, nullptr);
	auto rightSubtree = make_unique<BTree<Decision>>(move(*rightSubtreeDecision));

	// Right-Left: Healing spring
	auto healingSpringDecision = DecisionFactory::createDecision(
		"You found a suspicious spring. It looks quite tempting to drink from it",
		true, 1, "Intelligence",
		"You feel rejuvenated! Vitality Increased. \n Press ENTER to continue....",
		"You feel rejuvenated! Vitality Increased. \n Press ENTER to continue....",
		new IncreaseStatEffect("Vitality", 1), nullptr, nullptr);
	rightSubtree->attachLeft(make_unique<BTree<Decision>>(move(*healingSpringDecision)));

	// Right-Right: Narrow bridge
	auto narrowBridgeDecision = DecisionFactory::createDecision(
		"A precarious bridge requires dexterity to cross.",
		true, 8, "Dexterity",
		"You skillfully cross the bridge! You were surprised on how you did that. +1 Dexterity \n Press ENTER to continue....",
		"You tripped and fell on your face but manage to cross. Took 5 Damage! \n Press ENTER to continue....",
		new IncreaseStatEffect("Dexterity", 1), new DamageHealthEffect(5), nullptr);
	rightSubtree->attachRight(make_unique<BTree<Decision>>(move(*narrowBridgeDecision)));

	// Attach right subtree to root
	dungeonAdventure->attachRight(move(rightSubtree));

	return dungeonAdventure;
}
unique_ptr<BTree<Decision>> Event::createTempleAdventure(Character& character) {
	// Root of the temple adventure decision tree
	auto rootDecision = DecisionFactory::createDecision(
		"As you approach the entrance of the ancient temple, \nshrouded in the mists of time, you are presented with a pivotal choice. \nTo your left, an overgrown passage, its path concealed by the relentless \ngrasp of nature, \nsuggests secrets hidden and reclaimed by the earth. \n\nTo your right, an echoing hall, its vastness reverberating with the \nwhispers of ages past, \nbeckons you towards the unknown grandeur and potential perils within.\n\n You can only choose one path. \nWill it be the tangled mysteries of the overgrown passage \nor the daunting expanse of the echoing hall?.\n\nLeft: Head to the overgrown passage.\nRight: Head to echoing hall. \n\nChoose a path : \n1.Left\n2.Right",
		false, 0, "", "", "", nullptr, nullptr, nullptr);
	auto templeAdventure = make_unique<BTree<Decision>>(move(*rootDecision));

	// Left subtree: Overgrown passage
	auto leftPathDecision = DecisionFactory::createDecision(
		"You go through the overgrown passage and you found \na hidden alcove and a suspicious shiny door in your way.\n\nLeft: Hidden alcove.\nRight: Suspicious Shiny Door \nChoose a path : \n1.Left\n2.Right",
		false, 0, "", "", "", nullptr, nullptr, nullptr);
	auto leftPath = make_unique<BTree<Decision>>(move(*leftPathDecision));

	// Left-Left: Hidden alcove
	auto hiddenAlcoveDecision = DecisionFactory::createDecision(
		"You found a hidden alcove filled with ancient texts. \nThese would require some knowledge of the old arts to decipher (Intelligence LVL 12)", true, 12, "Intelligence",
		"You decipher the texts and gain knowledge! +1 Intelligence", "The writings are too cryptic. You get a headache. Took 5 Damage!",
		new IncreaseStatEffect("Intelligence", 1), new DamageHealthEffect(5), nullptr
	);
	leftPath->attachLeft(make_unique<BTree<Decision>>(move(*hiddenAlcoveDecision)));

	// Left-Right: Trapped floor
	auto trappedFloorDecision = DecisionFactory::createDecision(
		"Oh no, the door closed behind you and you find yourself in a room with hidden floor traps. (Dexterity LVL 10)", true, 10, "Dexterity",
		"You navigate the traps safely! +1 Dexterity", "You trigger a trap but escape narrowly. You sprained an ankle. Took 5 Damage!",
		new IncreaseStatEffect("Dexterity", 1), new DamageHealthEffect(5), nullptr
	);
	leftPath->attachRight(make_unique<BTree<Decision>>(move(*trappedFloorDecision)));

	// Attach left subtree to root
	templeAdventure->attachLeft(move(leftPath));

	// Right subtree: Echoing hall
	auto rightPathDecision = DecisionFactory::createDecision(
		"You arrive at the Echoing hall and found a Guarded Relic \non your left and a Mysterious Statue on your right.\nLeft: Guarded relic \nRight: Mysterious statue \nChoose a path : \n1.Left\n2.Right",
		false, 0, "", "", "", nullptr, nullptr, nullptr
	);
	auto rightPath = make_unique<BTree<Decision>>(move(*rightPathDecision));

	// Right-Left: Guarded relic
	auto guardedRelicDecision = DecisionFactory::createDecision(
		"A relic is suddenly guarded by spectral forces. (Require Strength Lvl 15)", true, 15, "Strength",
		"You fend off the guardians and claim the relic! +1 Strength", "The spectral forces overpower you, but you escape. Barely. Took 5 Damage!",
		new IncreaseStatEffect("Strength", 1), new DamageHealthEffect(5), nullptr
	);
	rightPath->attachLeft(make_unique<BTree<Decision>>(move(*guardedRelicDecision)));

	// Right-Right: Mysterious statue
	auto mysteriousStatueDecision = DecisionFactory::createDecision(
		"A statue with a riddle. (Require Intelligence Lvl 14)", true, 14, "Intelligence",
		"You solve the riddle and find a hidden treasure! +1 Intelligence", "The riddle baffles you, until you get a migraine. Took 5 Damage!",
		new IncreaseStatEffect("Intelligence", 1), new DamageHealthEffect(5), nullptr
	);
	rightPath->attachRight(make_unique<BTree<Decision>>(move(*mysteriousStatueDecision)));

	// Attach right subtree to root
	templeAdventure->attachRight(move(rightPath));

	return templeAdventure;
}






