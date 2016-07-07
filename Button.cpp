#include "Button.h"



Button::Button()
{
}


Button::~Button()
{
}

void Button::setPosition(int x, int y)
{
	position.x = x;
	position.y = y;
}

void Button::setDimensions(int w, int h)
{
	width = w;
	height = h;
}

SDL_Point Button::getPosition()
{
	return position;
}

char * Button::getID()
{
	return id;
}

void Button::setID(char * tempID)
{
	id = tempID;
}

bool Button::handleEvent(SDL_Event* e)
{
	bool buttonPressed = false;
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in gem
		bool inside = true;

		//Mouse is left of the gem
		if (x < position.x)
		{
			inside = false;
		}
		//Mouse is right of the gem
		else if (x > position.x + width)
		{
			inside = false;
		}
		//Mouse above the gem
		else if (y < position.y)
		{
			inside = false;
		}
		//Mouse below the gem
		else if (y > position.y + height)
		{
			inside = false;
		}

		//Mouse is outside gem
		if (!inside)
		{
			currentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside gem
		else
		{
			//Set mouse over sprite
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				
				currentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;

			case SDL_MOUSEBUTTONDOWN:
				//Play the sound effect
				Mix_PlayChannel(-1, selectSound, 0);
				//Change the sprite state
				currentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				buttonPressed = true;
			}
		}
	}
	return buttonPressed;
}

void Button::render()
{
	//Show PLAY button
	if (id == "play")
	{
		playSpriteSheetTexture.render(position.x, position.y, &playSpriteClips[currentSprite]);
	}
}
