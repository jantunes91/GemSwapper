#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <Windows.h>
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
	void swapGems(Gem *gPressedButtons[2], Gem gems[TOTAL_GEMS]);

	//Checks if there's a sequence and removes it from the board
	//with simple flag as true, it doesn't remove the sequence, plays the animation or updates the score
	bool checkSequence(Gem gems[TOTAL_GEMS], bool simple);

	//Removes a previously found sequence, given it's end and it's lenght
	void removeSequence(int x, int y, int lenght, int orientation, Gem gems[TOTAL_GEMS]);

	void calculateScore(int lenght);

	void dropDownGems(Gem gems[TOTAL_GEMS]);

	void generateNewGems(Gem gems[TOTAL_GEMS]);

	bool checkAvailableMoves(Gem gems[TOTAL_GEMS]);
};

