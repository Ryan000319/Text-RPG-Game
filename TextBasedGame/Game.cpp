#include "Game.h"

Game::Game(sf::RenderWindow& window, sf::Font* font)
	: window(window), font(font), enemies(nullptr), characters(5), shop(nullptr)
{
	playing = true;
	activeCharacter = 0;
	fileName = "characters.txt";
}

Game::~Game() 
{
    while (enemies != nullptr) {
        auto toDelete = enemies;
        enemies = enemies->getNext();
        delete toDelete;
    }

    for (size_t i = 0; i < characters.size(); ++i) {
        delete characters[i];
    }

	delete shop;
}

//Functions
void Game::initializeShop() {
	if (characters.size() > activeCharacter && characters[activeCharacter] != nullptr) {
		delete shop; // Clean up the old shop if it exists
		shop = new Shop(characters[activeCharacter]->getLevel());
	}
	else {
		std::cerr << "Active character not set. Shop cannot be initialized." << std::endl;
	}
}

string Game::loadStoryFromFile(const std::string& filename) {
	std::ifstream inFile(filename);
	std::string story((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
	return story;
}

void Game::showTypingEffect(const std::string& text, unsigned int maxWidth) {
	sf::Text storyText;
	storyText.setFont(*this->font);
	storyText.setCharacterSize(30);
	storyText.setFillColor(sf::Color::White);
	storyText.setPosition(50.f, 50.f);

	std::string displayedText = "";
	std::string line = "";
	std::string word = "";
	float yPos = 50.f;

	for (char c : text) {
		if (c == ' ' || c == '\n') {
			std::string tempLine = line + word + c;
			storyText.setString(tempLine);

			if (storyText.getLocalBounds().width > maxWidth || c == '\n') {
				displayedText += line + "\n";
				line = word + c;
				yPos += storyText.getLocalBounds().height;
			}
			else {
				line = tempLine;
			}

			word = "";
		}
		else {
			word += c;
		}

		storyText.setString(displayedText + line + word);

		this->window.clear();
		this->window.draw(storyText);
		this->window.display();

		sf::sleep(sf::milliseconds(10)); // Typing effect delay
	}

	displayedText += line; 
	storyText.setString(displayedText);

	displayedText += "\nPress Enter to continue...";
	storyText.setString(displayedText);

	this->window.clear();
	this->window.draw(storyText);
	this->window.display();

	// Wait for Enter key press
	bool waitingForEnter = true;
	while (this->window.isOpen() && waitingForEnter) {
		sf::Event event;
		while (this->window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				this->window.close();
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Enter) {
					waitingForEnter = false;  // Exit the loop when Enter is pressed
				}
			}
		}
	}
}

void Game::initGame() {
	if (!backgroundTexture.loadFromFile("Assets/IMG/main_menu.png")) {
		std::cerr << "Failed to load background texture" << std::endl;
	}
	backgroundSprite.setTexture(backgroundTexture);

	if (!gameMusic.openFromFile("Assets/AUDIO/main3.ogg")) {
		std::cerr << "Failed to load music" << std::endl;
	}
    ifstream in("characters.txt");

    Weapon::initNames();
    Armor::initNames();
    Potion::initNames();

	// After loading characters
	if (in.is_open()) {
		this->loadCharacters();
		this->initializeShop();
	}

	else {
		// If no characters are loaded, create a default character
		this->createNewCharacter();
		this->initializeShop();
	}
}

void Game::addEnemy(const Enemy& enemy) {
    auto newNode = new SinglyLinkedNode<Enemy>(enemy, enemies);
    enemies = newNode;
}

void Game::printEnemies(sf::RenderWindow& window) const {
	auto current = enemies;
	float yPos = 20.0f;

	while (current != nullptr) {
		sf::Text enemyText;
		enemyText.setFont(*this->font); // Dereference the font pointer here
		enemyText.setString(current->getValue().getName());
		enemyText.setCharacterSize(24);
		enemyText.setFillColor(sf::Color::White);
		enemyText.setPosition(10.f, yPos);

		window.draw(enemyText);

		yPos += 30.0f;
		current = current->getNext();
	}
}

bool Game::mainMenu() {

	// Load and set the main menu background
	if (!backgroundTexture.loadFromFile("Assets/IMG/prologue2.png")) {
		std::cerr << "Failed to load main menu background texture" << std::endl;
	}

	backgroundSprite.setTexture(backgroundTexture);
	
	//Banner
	sf::Texture rectangleTexture;
	if (!rectangleTexture.loadFromFile("Assets/IMG/Buttons/banner3.png")) {
		std::cerr << "Failed to load rectangle texture" << std::endl;
	}

	// Black Rectangle
	sf::Sprite bannerSprite;
	bannerSprite.setTexture(rectangleTexture);
	bannerSprite.setPosition(50, 500);

	// Welcome text
	sf::Text welcomeText;
	welcomeText.setFont(*this->font);
	welcomeText.setString("Main Menu");
	welcomeText.setCharacterSize(30);
	welcomeText.setFillColor(sf::Color::White);
	welcomeText.setPosition(165, 525); 



	// Main menu Buttons
	//Travel Button
	buttons.push_back(Button(100, 600, 89, 27, *this->font, "",
		"Assets/IMG/Buttons/Travel.png", "Assets/IMG/Buttons/Travel2.png"));

	//Shop Button
	buttons.push_back(Button(100, 625, 67, 29, *this->font, "", 
		"Assets/IMG/Buttons/Shop.png", "Assets/IMG/Buttons/Shop2.png"));

	//Level Up Button
	buttons.push_back(Button(100, 650, 122, 29, *this->font, "",
		"Assets/IMG/Buttons/Lvlup.png", "Assets/IMG/Buttons/Lvlup2.png"));

	//Rest Button
	buttons.push_back(Button(100, 675, 65, 27, *this->font, "", 
		"Assets/IMG/Buttons/Rest.png", "Assets/IMG/Buttons/Rest2.png"));

	//Show Character Sheet Button
	buttons.push_back(Button(100, 700, 197, 27, *this->font, "", 
		"Assets/IMG/Buttons/CharInfo.png", "Assets/IMG/Buttons/CharInfo2.png"));

	//Show Inventory Button
	buttons.push_back(Button(100, 725, 202, 29, *this->font, "", 
		"Assets/IMG/Buttons/inventory.png", "Assets/IMG/Buttons/inventory2.png"));

	//Create New Character Button
	buttons.push_back(Button(100, 750, 187, 27, *this->font, "", 
		"Assets/IMG/Buttons/newChar.png", "Assets/IMG/Buttons/newChar2.png"));

	//Save Current Character Button
	buttons.push_back(Button(100, 775, 205, 27, *this->font, "", 
		"Assets/IMG/Buttons/save.png", "Assets/IMG/Buttons/save2.png"));

	//Load Saved Character BUtton
	buttons.push_back(Button(100, 800, 205, 27, *this->font, "", 
		"Assets/IMG/Buttons/select.png", "Assets/IMG/Buttons/select2.png"));

	//Quit Button
	buttons.push_back(Button(100, 825, 60, 27, *this->font, "", 
		"Assets/IMG/Buttons/quit.png", "Assets/IMG/Buttons/quit2.png"));


	while (this->window.isOpen()) {
		sf::Event event;

		// Update character info text based on the current character's status
		if (characters.size() > 0 && characters[activeCharacter]) {
			std::string characterInfo = "";
			if (characters[activeCharacter]->getExp() >= characters[activeCharacter]->getExpNext()) {
				characterInfo += " - Level Up Available!";
			}
			
		}

		while (this->window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				this->window.close();
			}

			for (auto& button : buttons) {
				button.update(this->window);
			}

			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {

					if (buttons[0].isMouseOver(this->window)) {
						// Travel button action
						travel();
						break;
					}

					else if (buttons[1].isMouseOver(this->window)) {
						//Shop

						openShop();
						break;
					}

					else if (buttons[2].isMouseOver(this->window)) {
						//Level up
						levelUpMenu();
						break;
					}

					else if (buttons[3].isMouseOver(this->window)) {
						//Rest
						rest();
						break;
					}

					else if (buttons[4].isMouseOver(this->window)) {
						//Character Information
						characterSheet();
						break;
					}

					else if (buttons[5].isMouseOver(this->window)) {
						//Character Inventory
						inventoryMenu();
						break;
					}

					else if (buttons[6].isMouseOver(this->window)) {
						//Create New Character
						createNewCharacter();
						break;
					}

					else if (buttons[7].isMouseOver(this->window)) {
						//Save Current Character
						saveCharacters();
						break;
					}

					else if (buttons[8].isMouseOver(this->window)) {
						//Select Saved Character
						selectCharacter();
						break;
					}

					else if (buttons[9].isMouseOver(this->window)) {
						this->window.close();
						return false; // Return false to indicate game should exit
					}
				}
			}
		}

		this->window.clear();
		this->window.draw(backgroundSprite);
		this->window.draw(bannerSprite);
		
		// Draw the welcome text and character info
		this->window.draw(welcomeText);
		

		for (auto& button : buttons) {
			button.drawTo(this->window);
		}

		this->window.display();
	}
	return true;
}

void Game::openShop() {

	// Check if shop is initialized
	if (shop) {
		shop->openShop(*characters[activeCharacter], this->window, *this->font);
	}

	else {
		// Display error message or initialize shop if needed
		sf::Text errorMessage("Shop is not available.", *this->font, 24);
		errorMessage.setFillColor(sf::Color::White);
		errorMessage.setPosition(50.f, 50.f);

		this->window.clear();
		this->window.draw(errorMessage);
		this->window.display();
		sf::sleep(sf::seconds(2)); // Display message for 2 seconds

		// Return to main menu
		mainMenu();
	}
}

void Game::characterSheet() {
	if (!CharSheetTexture.loadFromFile("Assets/IMG/main_menu6.png")) {
		std::cerr << "Failed to load main menu background texture" << std::endl;
	}
	CharSheetSprite.setTexture(CharSheetTexture);
	CharSheetSprite.setPosition(0.f, 0.f);

	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f(325, 600));
	rectangle.setFillColor(sf::Color::Black);
	rectangle.setOutlineColor(sf::Color::Black);
	rectangle.setOutlineThickness(5);
	rectangle.setPosition(40, 290);

	if (characters.size() > 0 && characters[activeCharacter]) {
		// Clear the window
		this->window.clear();

		this->window.draw(CharSheetSprite);
		this->window.draw(rectangle);

		// Call the renderStats method from the Character class
		characters[activeCharacter]->renderStats(this->window, *this->font, 50.f, 300.f);

		// Display the character sheet
		
		this->window.display();

		// Wait for the user to press Enter to return to the main menu
		bool waitingForEnter = true;
		while (this->window.isOpen() && waitingForEnter) {
			sf::Event event;
			while (this->window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					this->window.close();
				}
				else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
					waitingForEnter = false;
				}
			}
		}
	}
}

//Create new Character window
void Game::createNewCharacter() {
	std::string inputName;

	// Initialize text objects for display
	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f(250, 100));
	rectangle.setFillColor(sf::Color::Black);
	rectangle.setOutlineColor(sf::Color::Black);
	rectangle.setOutlineThickness(5);
	rectangle.setPosition(450, 500);

	sf::Text promptText;
	promptText.setFont(*this->font); // Set the font (dereference the pointer)
	promptText.setString("Enter Character Name:"); // Set the text
	promptText.setCharacterSize(24); // Set the character size
	promptText.setFillColor(sf::Color::White);
	promptText.setPosition(460, 500);

	sf::Text inputText;
	inputText.setFont(*this->font); // Set the font (dereference the pointer)
	inputText.setString(""); // Initially empty string
	inputText.setCharacterSize(24); // Set the character size
	inputText.setFillColor(sf::Color::White);
	inputText.setPosition(460, 520);

	bool nameEntered = false;

	while (this->window.isOpen() && !nameEntered) {
		sf::Event event;
		while (this->window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				this->window.close();
			}
			// Handle text input event
			if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode < 128) {
					inputName += static_cast<char>(event.text.unicode);
					inputText.setString(inputName);
				}
			}
			// Handle Enter key for finalizing name
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Enter) {
					nameEntered = true;
				}
				// Handle backspace for editing name
				else if (event.key.code == sf::Keyboard::BackSpace && !inputName.empty()) {
					inputName.pop_back();
					inputText.setString(inputName);
				}
			}
		}

		this->window.clear();
		this->window.draw(backgroundSprite);
		this->window.draw(rectangle);
		this->window.draw(promptText);
		this->window.draw(inputText);
		this->window.display();

		if (nameEntered) {
			// Check if name already exists
			for (size_t i = 0; i < this->characters.size(); i++) {
				if (inputName == this->characters[i]->getName()) {
					inputName = ""; // Reset name if it already exists
					nameEntered = false; // Restart name entry
					inputText.setString(inputName);
					break;
				}
			}
		}
	}

	if (!inputName.empty()) {
		Character* newCharacter = new Character(inputName, 0, 100, 1, 0, 5, 5, 5, 5, 22, 8, 0);
		this->characters.push(newCharacter);
		this->activeCharacter = this->characters.size() - 1;
		this->initializeShop(); // Initialize shop for the new character
		mainMenu();  // Start the game with the new character
	}
}

void Game::levelUpCharacter() {
	if (this->characters[this->activeCharacter]->getExp() >= this->characters[this->activeCharacter]->getExpNext()) {

		// Level up character
		this->characters[this->activeCharacter]->levelUp();

		sf::Text levelUpText("Choose a stat to upgrade:", *this->font, 24);
		levelUpText.setFillColor(sf::Color::White);
		levelUpText.setPosition(10.f, 10.f);

		//Options of stats to upgrade
		std::vector<std::string> optionsText = { "Strength", "Vitality", "Dexterity", "Intelligence" };
		std::vector<sf::Text> options;

		for (size_t i = 0; i < optionsText.size(); ++i) {
			sf::Text option(optionsText[i], *this->font, 24);
			option.setFillColor(sf::Color::White);
			option.setPosition(10.f, 50.f + i * 40.f);
			options.push_back(option);
		}

		int selectedOption = -1;

		while (this->window.isOpen() && selectedOption == -1) {
			sf::Event event;
			while (this->window.pollEvent(event)) {
				if (event.type == sf::Event::KeyPressed) {
					if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num4) {
						selectedOption = event.key.code - sf::Keyboard::Num1;
						this->characters[this->activeCharacter]->addToStat(selectedOption, 1);

						// Display a message indicating the stat has been upgraded
						sf::Text upgradeText("Stat upgraded successfully!", *this->font, 24);
						upgradeText.setFillColor(sf::Color::White);
						upgradeText.setPosition(10.f, 200.f);
						this->window.draw(upgradeText);
						this->window.display();
						sf::sleep(sf::seconds(3)); // Display the message for 3 seconds
						break;
					}
				}
			}

			this->window.clear();
			this->window.draw(levelUpText);
			for (auto& option : options) {
				this->window.draw(option);
			}
			this->window.display();
		}
	}
	else {
		// Handle the case when the character does not have enough experience to level up
		sf::Text noLevelUpText("Not enough experience to level up.", *this->font, 24);
		noLevelUpText.setFillColor(sf::Color::White);
		noLevelUpText.setPosition(10.f, 10.f);
		this->window.clear();
		this->window.draw(noLevelUpText);
		this->window.display();
		sf::sleep(sf::seconds(2)); // Display the message for 2 seconds
	}
	
}

void Game::saveCharacters() {
	ofstream outFile(fileName);

	if (outFile.is_open()) {
		for (size_t i = 0; i < characters.size(); i++) {
			Character* character = characters[i]; // Get the pointer to the character

			outFile << "BEGIN_CHARACTER\n";
			outFile << character->getAsString() << "\n"; // Use pointer to call getAsString
			outFile << "BEGIN_INVENTORY\n";

			for (int j = 0; j < character->getInventorySize(); ++j) {
				Item* item = character->getInventoryItem(j); // Use pointer to call getInventoryItem
				if (item != nullptr) {
					// Check item type and add prefix
					if (dynamic_cast<Weapon*>(item)) {
						outFile << "Weapon ";
					}
					else if (dynamic_cast<Armor*>(item)) {
						outFile << "Armor ";
					}
					else if (dynamic_cast<Potion*>(item)) {
						outFile << "Potion ";
					}

					// Write item details
					outFile << item->toString() << "\n";
				}
			}

			outFile << "END_INVENTORY\n";
			outFile << "END_CHARACTER\n";
		}
	}

	else
	{
		std::cerr << "Unable to open file for saving characters." << std::endl;
	}

	outFile.close();
}


bool Game::isWeapon(const string& itemName) {
	static const set<string> weaponNames = {
		"Combat Knife", "Sword", "Axe", "PickAxe", "WarHammer", "Katana"
	};

	return weaponNames.find(itemName) != weaponNames.end();
}

bool Game::isArmor(const string& itemName) {
	static const set<string> armorNames = {
		"Leather Armor", "Iron Armor", "Gold Armor", "ChainMail Armor", "Diamond Armor", "Nether Armor"
	};

	return armorNames.find(itemName) != armorNames.end();
}

void Game::loadExistingCharacters() {
	ifstream inFile("characters.txt");

	if (inFile.is_open()) {
		this->loadCharacters(); // Use your existing loadCharacters logic
		mainMenu();  // Return to main menu after loading
	}
	else {
		sf::Text errorText("No saved characters found.", *this->font, 24);
		errorText.setFillColor(sf::Color::White);
		errorText.setPosition(50.f, 50.f);
		this->window.draw(errorText);
		this->window.display();
		sf::sleep(sf::seconds(2));
		mainMenu();
	}
}

void Game::loadCharacters() {
	ifstream inFile(fileName);
	string line;

	if (inFile.is_open()) {
		Character* temp = nullptr;
		bool loadingCharacter = false;
		bool loadingInventory = false;

		
		while (getline(inFile, line)) {
			//Starting point to detect Character Object 
			if (line == "BEGIN_CHARACTER") {
				temp = new Character();
				loadingCharacter = true;
				loadingInventory = false;
			}

			//Ending point to detect Character Object
			else if (line == "END_CHARACTER") {
				if (temp != nullptr) {
					this->characters.push(temp);
					cout << "Character " << temp->getName() << " loaded!" << endl;
				}
				loadingCharacter = false;
				temp = nullptr; // Reset temp to nullptr after adding to characters
			}

			//Starting point to detect Inventory Object under Detected Character Object
			else if (line == "BEGIN_INVENTORY") {
				loadingInventory = true;
			}

			//Ending point to detect Inventory Object under Detected Character Object
			else if (line == "END_INVENTORY") {
				loadingInventory = false;
			}

			//Start loading Character Details
			else if (loadingCharacter && !loadingInventory && !line.empty()) {
				stringstream str(line);
				string name;
				int distanceTravelled, gold, level, exp, strength, vitality, dexterity, intelligence, hp, stamina, statPoints;
				str >> name >> distanceTravelled >> gold >> level >> exp >> strength >> vitality >> dexterity >> intelligence >> hp >> stamina >> statPoints;
				temp = new Character(name, distanceTravelled, gold, level, exp, strength, vitality, dexterity, intelligence, hp, stamina, statPoints);
			}

			//Start Loading Inventory Item Details
			else if (loadingInventory && !line.empty()) {
				stringstream itemStream(line);
				string itemType;
				itemStream >> itemType;

				if (itemType == "Weapon") {
					// Load weapon data
					string name;
					int damageMin, damageMax, level, buyValue, sellValue, rarity;
					itemStream >> name >> level >> rarity >> damageMin >> damageMax >> buyValue >> sellValue;
					Weapon* weapon = new Weapon(damageMin, damageMax, name, level, buyValue, sellValue, rarity);
					temp->addItem(*weapon);
					delete weapon;
				}
				else if (itemType == "Armor") {
					// Load armor data
					string name;
					int defence, level, buyValue, sellValue, rarity;
					itemStream >> name >> level >> rarity >> defence >> buyValue >> sellValue;
					Armor* armor = new Armor(defence, name, level, buyValue, sellValue, rarity);
					temp->addItem(*armor);
					delete armor;
				}
			
				else if (itemType == "Potion") {
					// Load potion data
					string name, description, typeStr;
					int level, rarity, potency;

					itemStream >> name >> description >> level >> rarity >> potency;

					Potion::PotionType type = Potion::PotionType::Health; // Default type
					if (!typeStr.empty()) {
						if (typeStr == "Health") {
							type = Potion::PotionType::Health;
						}
						else if (typeStr == "Strength") {
							type = Potion::PotionType::Strength;
						}
						else if (typeStr == "Vitality") {
							type = Potion::PotionType::Vitality;
						}
						else if (typeStr == "Dexterity") {
							type = Potion::PotionType::Dexterity;
						}
						else if (typeStr == "Intelligence") {
							type = Potion::PotionType::Intelligence;
						}
						else {
							cout << "Unknown potion type: " << typeStr << endl;
						}
					}
					Potion* potion = new Potion(name, level, rarity, type, potency);
					temp->addItem(*potion);
					delete potion;
				}
			}
		}
	}

	inFile.close();

	if (this->characters.size() == 0) {
		throw "No Characters Available.";
	}
}








void Game::selectCharacter() {
	

	// Check if there are characters to select
	if (characters.size() > 0) {
		sf::Text title("Select Character:", *this->font, 24);
		title.setFillColor(sf::Color::White);
		title.setPosition(10.f, 10.f);

		std::vector<sf::Text> characterOptions;

		// Create a menu option for each character
		for (size_t i = 0; i < characters.size(); i++) {
			std::string optionText = std::to_string(i + 1) + ". " + characters[i]->getName() + " (Level: " + std::to_string(characters[i]->getLevel()) + ")";
			sf::Text option(optionText, *this->font, 20);
			option.setFillColor(sf::Color::White);
			option.setPosition(10.f, 50.f + i * 30.f);
			characterOptions.push_back(option);
		}

		int selectedCharacter = -1;

		// Event loop for character selection
		while (this->window.isOpen() && selectedCharacter == -1) {
			sf::Event event;
			while (this->window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					this->window.close();
				}

				if (event.type == sf::Event::KeyPressed) {
					if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num9) {
						int choice = event.key.code - sf::Keyboard::Num1;
						if (choice >= 0 && choice < static_cast<int>(characters.size())) {
							selectedCharacter = choice;
						}
					}
				}
			}

			this->window.clear();
			this->window.draw(title);
			for (auto& option : characterOptions) {
				this->window.draw(option);
			}
			this->window.display();
		}

		if (selectedCharacter >= 0) {
			activeCharacter = selectedCharacter;
			this->initializeShop(); // Initialize shop for the selected character
		}
	}
	else {
		sf::Text noCharacterText("No characters to select.", *this->font, 24);
		noCharacterText.setFillColor(sf::Color::White);
		noCharacterText.setPosition(50.f, 50.f);
		this->window.clear();
		this->window.draw(noCharacterText);
		this->window.display();
		sf::sleep(sf::seconds(2));
	}

	// Return to the main menu after selection or if there are no characters
	mainMenu();
}




void Game::travel() {
	// Increment the travel distance of the active character
	characters[activeCharacter]->travel();



	// Generate a random event
	Event event;
	int eventType = rand() % 2;  // Randomly decide the type of event

	// Check the type of event and handle it accordingly
	if (eventType == 0) {
		// Handle enemy encounter event
		event.enemyEncounter(this->window, *this->font, *characters[activeCharacter], enemies);
	}
	else if (eventType == 1) {
		// Handle puzzle encounter event
		event.puzzleEncounter(this->window, *this->font, *characters[activeCharacter]);
	}

	// Update the this->window display and handle event aftermath
	this->window.clear();
	// ... Draw relevant game elements if needed
	this->window.display();

	// Update the game state after the event
	// For example, check if the character leveled up or if they are still alive
	if (!characters[activeCharacter]->isAlive()) {
		handleCharacterDeath();
	}
}

void Game::handleCharacterDeath() {
	// Display death message
	sf::Text deathText("Your character has died. \nWould you like to start a new character? (Y/N)", *this->font, 24);
	deathText.setFillColor(sf::Color::White);
	deathText.setPosition(100, 100); // Adjust as needed

	this->window.clear();
	this->window.draw(deathText);
	this->window.display();

	// Wait for user input
	sf::Event event;
	while (this->window.waitEvent(event)) {
		if (event.type == sf::Event::Closed) {
			this->window.close();
			exit(0); // Exit the game
		}
		else if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Y) {
				createNewCharacter(); // Create a new character
				break;
			}
			else if (event.key.code == sf::Keyboard::N) {
				this->window.close();
				exit(0); // Exit the game
			}
		}
	}
}



void Game::rest() {
	// Load and set the shop background
	if (!RestTexture.loadFromFile("Assets/IMG/rest1.png")) {
		std::cerr << "Failed to load main menu background texture" << std::endl;
	}
	RestSprite.setTexture(RestTexture);
	RestSprite.setPosition(0.f, 0.f);

	if (characters.size() > activeCharacter && characters[activeCharacter]) {
		Character* activeChar = characters[activeCharacter];
		int cost = (activeChar->getHpMax() - activeChar->getHp()) * 2; 

		// Display a welcome message
		sf::Text welcomeText("Ah, welcome, traveler! Weary from your journey, are you? \nStep right in and make yourself at home.", *this->font, 24);
		welcomeText.setFillColor(sf::Color::White);
		welcomeText.setPosition(50.f, 750.f);

		// Check if the character has enough gold
		if (activeChar->getGold() >= cost) {
			activeChar->resetHP(); // Restore HP to maximum
			activeChar->payGold(cost); // Deduct the gold cost

			// Display a success message
			sf::Text restText("You rented a room and took a rest for a day. Healing Cost: " + to_string(cost) + " Gold.", *this->font, 24);
			restText.setFillColor(sf::Color::White);
			restText.setPosition(50.f, 800.f);

			this->window.clear(); // Clear the this->window
			window.draw(RestSprite);
			this->window.draw(welcomeText); // Draw the welcomeText
			this->window.draw(restText); // Draw the restText
			this->window.display(); // Display the updated window

			// Wait for a key press before returning to the menu
			sf::Event event;
			while (this->window.waitEvent(event)) {
				if (event.type == sf::Event::KeyPressed) {
					break;
				}
			}

			// Re-draw the main menu
			mainMenu();
		}
		else {
			// Display a failure message
			sf::Text restText("Not enough gold to rest.", *this->font, 24);
			restText.setFillColor(sf::Color::White);
			restText.setPosition(50.f, 815.f);

			this->window.clear(); // Clear the this->window
			this->window.draw(restText); // Draw the restText
			this->window.display(); // Display the updated this->window

			// Wait for a key press before returning to the menu
			sf::Event event;
			while (this->window.waitEvent(event)) {
				if (event.type == sf::Event::KeyPressed) {
					break;
				}
			}

			// Re-draw the main menu
			mainMenu();
		}
	}
}



void Game::inventoryMenu() {

	float xPos = 50.f; // Starting x-position for inventory items
	float yPos = 50.f; // Starting y-position for inventory items
	float columnWidth = 300.f; // Width of each inventory column
	const float windowMargin = 50.f; // Margin at the right edge of the window


	sf::Text inventoryTitle("Inventory. What do you wish to equip? (Press ESC to return to main menu). ", *this->font, 24);
	inventoryTitle.setFillColor(sf::Color::White);
	inventoryTitle.setPosition(50.f, 10.f);

	

	

	std::vector<sf::Text> inventoryTexts; // Store inventory texts for re-drawing

	// Display inventory items
	for (int i = 0; i < characters[activeCharacter]->getInventorySize(); ++i) {
		Item* item = characters[activeCharacter]->getInventoryItem(i);
		sf::Text itemText;

		if (item) {
			itemText.setFont(*this->font);
			itemText.setString(std::to_string(i + 1) + ": " + item->toString());
			itemText.setCharacterSize(20);
			itemText.setFillColor(sf::Color::White);
			itemText.setPosition(xPos, yPos);

			inventoryTexts.push_back(itemText); // Store the text for re-drawing

			yPos += 200.f; // Increment y-position for the next item

			// Check if yPos exceeds window height and there is space for a new column
			if (yPos + 40.f > this->window.getSize().y && xPos + columnWidth + windowMargin < this->window.getSize().x) {
				xPos += columnWidth; // Move to a new column
				yPos = 50.f; // Reset y-position for the new column
			}
		}
	}
	this->window.clear();
	this->window.draw(inventoryTitle);
	// Re-display inventory items using inventoryTexts
	for (auto& text : inventoryTexts) {
		this->window.draw(text);
	}
	
	this->window.display();



	while (this->window.isOpen()) {
		sf::Event event;
		while (this->window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				this->window.close();
			}

			// Check for number keys to select inventory items
			if (event.type == sf::Event::KeyPressed && event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num9) {
				int choice = event.key.code - sf::Keyboard::Num1;
				if (choice >= 0 && choice < characters[activeCharacter]->getInventorySize()) 
				{
					// Equip or use the selected item
					Item* selectedItem = characters[activeCharacter]->getInventoryItem(choice);
					if (dynamic_cast<Weapon*>(selectedItem)) {
						characters[activeCharacter]->equipWeapon(choice);
						
					}
					else if (dynamic_cast<Armor*>(selectedItem)) {
						characters[activeCharacter]->equipArmor(choice);
						
					}
					else if (dynamic_cast<Potion*>(selectedItem)) {
						characters[activeCharacter]->usePotion(choice);
						
					}
					
					// After equipping an item, return to the main menu
					mainMenu();
					return; // Exit the function

				}
			}

			else if (event.key.code == sf::Keyboard::Escape)
			{
				mainMenu();
				return;
			}
		}
	}
}


void Game::drawText(const std::string& text, float x, float y) {
    sf::Text sfmlText;
    sfmlText.setFont(*this->font);
    sfmlText.setString(text);
    sfmlText.setCharacterSize(24);
    sfmlText.setFillColor(sf::Color::White);
    sfmlText.setPosition(x, y);
    this->window.draw(sfmlText);
}





void Game::updateGame() {
	this->window.clear(); // Clear the screen
	this->window.draw(backgroundSprite); // Draw the background
	mainMenu(); // Example call
	this->window.display(); // Update the screen
}

void Game::levelUpMenu() {
    Character* currentChar = characters[activeCharacter];
    
    if (currentChar->getExp() >= currentChar->getExpNext()) {
        // Level up the character
        currentChar->levelUp();

        // Display message for leveling up
        sf::Text levelUpMessage("You have leveled up! Choose a stat to increase: \n1. Strength \n2. Vitality \n3. Dexterity \n4. Intelligence \n\nWhich stat do you wish to upgrade?", *this->font, 24);
        levelUpMessage.setFillColor(sf::Color::White);
        levelUpMessage.setPosition(50.f, 50.f);

        this->window.clear();
        this->window.draw(levelUpMessage);
        this->window.display();

        // Allow the player to allocate stat points
        while (currentChar->getStatPoints() > 0 && this->window.isOpen()) {
            sf::Event event;
            while (this->window.pollEvent(event)) {
                if (event.type == sf::Event::KeyPressed) {
					switch (event.key.code) {
					case sf::Keyboard::Num1:
						currentChar->addToStat(0, 1);
						break;
					case sf::Keyboard::Num2:
						currentChar->addToStat(1, 1);
						break;
					case sf::Keyboard::Num3:
						currentChar->addToStat(2, 1);
						break;
					case sf::Keyboard::Num4:
						currentChar->addToStat(3, 1);
						break;
					default:
						break; // No valid key was pressed
					}
                }
            }
        }

        // Redraw the main menu after allocating stat points
        mainMenu();

    } else {
        // Character does not have enough experience to level up
        sf::Text noLevelUpText("Not enough experience to level up.", *this->font, 24);
        noLevelUpText.setFillColor(sf::Color::White);
        noLevelUpText.setPosition(50.f, 50.f);

        this->window.clear();
        this->window.draw(noLevelUpText);
        this->window.display();
        sf::sleep(sf::seconds(2)); // Display message for 2 seconds

        // Redraw the main menu
        mainMenu();
    }
}
