#pragma once
#include <SDL.h>
#include "Gem.h"
#include "Variables.h"
#include "Animation.h"

class Board
{
public:
	Board();

	~Board();

	//Checks if two gems are adjacent
	bool isAdjacent(Gem *gem0, Gem *gem1);

	//Swaps two gems
	void swapGems(Gem *pressedGems[2], Gem gems[TOTAL_GEMS], Window *window);

	//Checks if there's a sequence and removes it from the board
	//with simple flag as true, it doesn't remove the sequence, plays the animation or updates the score
	bool checkSequence(Gem gems[TOTAL_GEMS], Window *window, bool simple);

	//Removes a previously found sequence, given it's end and it's lenght
	void removeSequence(int x, int y, int lenght, int orientation, Gem gems[TOTAL_GEMS]);

	//Updates the current score
	void calculateScore(int lenght);

	//drops down gems after a sequence is removed
	void dropDownGems(Gem gems[TOTAL_GEMS], Window *window);

	//generates new gems to fill empty spaces
	void generateNewGems(Gem gems[TOTAL_GEMS]);

	//checks if there are any available moves left
	bool checkAvailableMoves(Gem gems[TOTAL_GEMS], Window *window);
};

