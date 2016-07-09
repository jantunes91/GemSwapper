#pragma once
#include <SDL.h>
class Window
{
public:
	Window();

	~Window();

	//Starts up SDL and creates window
	bool init();

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
	void renderGame()
};

