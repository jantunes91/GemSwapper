#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <Windows.h>
#include "LButton.h"
#include "RenderVariables.h"
#include "Animation.h"

class Game
{
public:
	Game();

	~Game();

	//Checks if two squares are adjacent
	bool isAdjacent(LButton *button0, LButton *button1);

	//Swaps two squares
	void swapSquares(LButton *gPressedButtons[2], LButton gButtons[TOTAL_BUTTONS]);

	//Checks if there's a sequence and removes it from the board
	bool checkSequence(LButton gButtons[TOTAL_BUTTONS]);

	//Removes a previously found sequence, given it's end and it's lenght
	void removeSequence(int x, int y, int lenght, int orientation, LButton gButtons[TOTAL_BUTTONS]);

	void dropDownSquares(LButton gButtons[TOTAL_BUTTONS]);

	void generateNewSquares(LButton gButtons[TOTAL_BUTTONS]);
};

