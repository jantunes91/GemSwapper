#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Variables.h"

enum ButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
};

class Button
{
public:
	Button();

	~Button();

	//Sets top left position
	void setPosition(int x, int y);

	//Sets top left position
	void setDimensions(int w, int h);

	//Returns the button position
	SDL_Point getPosition();
	
	//Returns the button ID
	char* getID();

	//Sets the button ID
	void setID(char* tempID);

	//Handles mouse event
	bool handleEvent(SDL_Event* e);

	//Shows gem sprite
	void render();

private:
	//Top left position
	SDL_Point position;

	int height;

	int width;

	//Currently used global sprite
	ButtonSprite currentSprite;

	char* id;
};

