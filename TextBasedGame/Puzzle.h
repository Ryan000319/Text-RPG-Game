#pragma once

#include "STLInclude.h"

class Puzzle
{
public:
	Puzzle(string fileName);
	virtual ~Puzzle();
	string getAsString();

	inline const int& getCorrectAns()const { return this->correctAnswer; }

	void render(sf::RenderWindow& window, sf::Font& font, float x, float y) const;

private:
	string question;
	vector<string> answers;
	int correctAnswer;
};

