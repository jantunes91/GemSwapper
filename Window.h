#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <Windows.h>
#include "Constants.h"
#include "Gem.h"
#include "Texture.h"
#include "Board.h"
#include "Variables.h"
#include "Button.h"

class Window
{
public:
	Window();

	~Window();

	//Loads main menu media
	bool loadMenu();

	//Loads Game Over menu media
	bool loadGameOver();

	//Loads Game media
	bool loadGame();

	//Frees main menu media
	void closeMenu();

	//Frees Game Over menu media
	void closeGameOver();

	//Frees Game media
	void closeGame();

	//Renders the main menu
	void renderMenu();

	//Renders the Game Over menu
	void renderGameOver();

	//Renders the Game
	void renderGame();

	//Show the window and start the game
	void show();

	//The window we'll be rendering to
	SDL_Window* WindowInstance = NULL;

	//The window renderer
	//SDL_Renderer* gRenderer = NULL;

	//The PLAY button
	Button playButton;

	//The PLAY AGAIN button
	Button playagainButton;

	//The QUIT button
	Button quitButton;

	//Buttons objects
	Gem gems[TOTAL_GEMS];

	//Pressed Buttons
	Gem *pressedGems[2];

	//Board instance
	Board board;
};

