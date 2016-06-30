#pragma once

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <Windows.h>
#include "LTexture.h"
#include "Constants.h"
#include "RenderVariables.h"


enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	//BUTTON_SPRITE_MOUSE_UP = 3,
};

//The mouse button
class LButton
{
public:
	//Initializes internal variables
	LButton();

	//Sets top left position
	void setPosition(int x, int y);

	SDL_Point getPosition();

	void setPressed(bool isPressed);

	int getType();

	void setType(int tempType);

	void setRemoved(bool isRemoved);

	bool isRemoved();

	void setToUpdate(bool toUpdate);

	bool toUpdate();

	int updateY;

	//Handles mouse event
	void handleEvent(SDL_Event* e, LButton* gPressedButtons[2], int pressedCount);

	//Shows button sprite
	void render();// LTexture gColorSpriteSheetTexture, SDL_Rect gColorSpriteClips[BUTTON_SPRITE_TOTAL], SDL_Renderer* gRenderer);


private:
	//Top left position
	SDL_Point mPosition;

	//Currently used global sprite
	LButtonSprite mCurrentSprite;

	bool pressed;

	int type;

	bool removed;

	bool updatePosition;
};
