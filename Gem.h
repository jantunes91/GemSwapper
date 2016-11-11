#pragma once

#include <SDL.h>

enum GemSprite
{
	GEM_SPRITE_MOUSE_OUT = 0,
	GEM_SPRITE_MOUSE_OVER_MOTION = 1,
	GEM_SPRITE_MOUSE_DOWN = 1,
};

//The mouse button
class Gem
{
public:
	//Initializes internal variables
	Gem();

	//Sets top left position
	void setPosition(int x, int y);

	//Returns the Gem position
	SDL_Point getPosition();

	//Changes the pressed flag
	void setPressed(bool isPressed);

	//Returns the gem type
	int getType();

	//Sets the gem type
	void setType(int tempType);

	//Changes the removed flag
	void setRemoved(bool isRemoved);

	//Returns the value of the removed flag
	bool isRemoved();

	//Marks or unmarks the Gem to update
	void setToUpdate(bool toUpdate);

	//Returns if the Gem is marked to update
	bool toUpdate();

	//Sets the sprite to unselected
	void unselectTexture();

	//replace by SDL_Point origin
	int origX;

	int origY;

	//replace by SDL_Point destiny
	int destX;

	int destY;

	//stores how many places to drop
	int updateDrop;

	//Handles mouse event
	void handleEvent(SDL_Event* e, Gem* pressedGems[2]);

	//Shows gem sprite
	void render();


private:
	//Top left position
	SDL_Point position;

	//Currently used global sprite
	GemSprite currentSprite;

	bool pressed;

	int type;

	bool removed;

	bool updatePosition;
};
