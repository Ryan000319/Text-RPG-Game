#include "DynamicArray.h"
#include "Event.h"
#include "Shop.h"
#include "STLInclude.h"
#include "Button.h"

class Game {
public:
    Game(sf::RenderWindow& window, sf::Font* font);
    virtual ~Game();

    void initializeShop();
    void showTypingEffect(const std::string& text, unsigned int maxWidth);
    void initGame();
    bool mainMenu();
    void createNewCharacter();
    void levelUpCharacter();
    void saveCharacters();
    void loadCharacters();
    void selectCharacter();
    void travel();
    void rest();
    void inventoryMenu();
    void addEnemy(const Enemy& enemy);
    void printEnemies(sf::RenderWindow& window) const;

    inline bool getPlaying() const { return this->playing; }


    string loadStoryFromFile(const std::string& filename);

    void openShop();
    void handleCharacterDeath();
    void characterSheet();
    void levelUpMenu();
    void loadExistingCharacters();
private:
    int choice;
    bool playing;
    int activeCharacter;
    dynaArr<Character*> characters;
    std::string fileName;
    SinglyLinkedNode<Enemy>* enemies;

    void drawText( const std::string& text, float x, float y);
    void updateGame();
    bool isWeapon(const std::string& itemName);
    bool isArmor(const std::string& itemName);
    
    sf::Music gameMusic;
    
    Shop* shop;

    sf::RenderWindow& window; // Reference
    sf::Font* font;
    std::vector<Button> buttons;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Texture RestTexture;
    sf::Sprite RestSprite;

    sf::Texture CharSheetTexture;
    sf::Sprite CharSheetSprite;


};