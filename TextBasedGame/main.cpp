
#include "Game.h"

using namespace std;

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1024, 1024), "Adventure Quest", sf::Style::Close);

    // Load background image
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Assets/IMG/menu3.png")) {
        std::cerr << "Error loading background.png" << std::endl;
        return EXIT_FAILURE;
    }

    // Create background sprite
    sf::Sprite background;
    background.setTexture(backgroundTexture);

    // Load font
    sf::Font font;
    if (!font.loadFromFile("Assets/FONT/font8.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return EXIT_FAILURE;
    }

    // Load and play music
    sf::Music menuMusic;
    if (!menuMusic.openFromFile("Assets/AUDIO/main3.ogg")) {
        std::cerr << "Error loading music" << std::endl;
        return EXIT_FAILURE;
    }

    menuMusic.setLoop(true); // Loop the music
    menuMusic.play();
    menuMusic.setVolume(10.f); // Set volume to 10%

    bool gameStarted = false;
    bool showBackstory = false;

    // Start the game loop
    try {
        while (window.isOpen()) {
            // Process events
            sf::Event event;
            while (window.pollEvent(event)) {
                // Close window: exit
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                // Check for Enter key press to start the game
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Enter && !gameStarted) {
                        gameStarted = true;
                        showBackstory = true;
                    }
                }
            }

            window.clear();
            window.draw(background);
            Game game(window, &font);

            if (!gameStarted) {
                //do nothing
            }

            else if (showBackstory) {
                // Main game logic
               
                std::string rpgStory = game.loadStoryFromFile("Assets/TXT/Introduction.txt");
                game.showTypingEffect(rpgStory, 850);

                showBackstory = false;  // Reset after showing
            }

            else {
                game.initGame();
                while (!game.mainMenu()) {
                    break;
                }

            }
            window.display();

        }
        return EXIT_SUCCESS;
    }

    catch (const std::runtime_error& e) {
        std::cout << "Exiting game: " << e.what() << std::endl;
        return EXIT_SUCCESS;
    }
    
}

