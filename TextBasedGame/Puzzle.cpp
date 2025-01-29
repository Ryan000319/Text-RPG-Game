#include "Puzzle.h"

Puzzle::Puzzle(string fileName)
{
	this->correctAnswer = 0;

	ifstream inFile(fileName);
	int numOfAns = 0;
	string answer = "";
	int correctAns = 0;

	if (inFile.is_open())
	{
		getline(inFile, this->question);
		inFile >> numOfAns;
		inFile.ignore();

		for (size_t i = 0; i < numOfAns; i++)
		{
			getline(inFile, answer);
			this->answers.push_back(answer);
		}


		inFile >> correctAns;
		this->correctAnswer = correctAns;
		inFile.ignore();

	}

	else
		throw("Could no open puzzle!");

	inFile.close();
}

Puzzle::~Puzzle()
{

}

string Puzzle::getAsString()
{
	string answers = "";

	for (size_t i = 0; i < this->answers.size(); i++)
	{
		answers += to_string(i) + ": " + this->answers[i] + "\n";
	}

	/*return this->question + "\n" + "\n"
		+ answers + "\n"
		+ to_string(this->correctAnswer) + "\n";*/

	return this->question + "\n" + "\n"
		+ answers + "\n";

}

void Puzzle::render(sf::RenderWindow& window, sf::Font& font, float x, float y) const {
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);

	// Render the question
	text.setString(question);
	text.setPosition(x, y);
	window.draw(text);

	// Calculate the height of the text for spacing
	float textHeight = text.getLocalBounds().height + 5;

	// Render each answer
	for (size_t i = 0; i < answers.size(); ++i) {
		text.setString(std::to_string(i) + ": " + answers[i]);
		text.setPosition(x, y + (i + 1) * textHeight);  // Position each answer below the previous
		window.draw(text);
	}
}