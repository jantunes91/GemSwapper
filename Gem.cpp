#include "Gem.h"
#include "Constants.h"
#include "Variables.h"

Gem::Gem()
{
	position.x = OFFSET_X;
	position.y = OFFSET_Y;

	currentSprite = GEM_SPRITE_MOUSE_OUT;

	type = rand() % 5;

	removed = false;

	updatePosition = false;
	destY = 0;
	destX = 0;
}

void Gem::setPosition(int x, int y)
{
	position.x = x;
	position.y = y;
}

SDL_Point Gem::getPosition()
{
	return position;
}

void Gem::setPressed(bool isPressed)
{
	pressed = isPressed;
}

int Gem::getType()
{
	return type;
}

void Gem::setType(int tempType)
{
	type = tempType;
}

void Gem::setRemoved(bool isRemoved)
{
	removed = isRemoved;
}

bool Gem::isRemoved()
{
	return removed;
}

void Gem::setToUpdate(bool toUpdate)
{
	updatePosition = toUpdate;
}

bool Gem::toUpdate()
{
	return updatePosition;
}

void Gem::unselectTexture()
{
	currentSprite = GEM_SPRITE_MOUSE_OUT;
}

void Gem::handleEvent(SDL_Event* e, Gem* pressedGems[2])
{
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
		else if (x > position.x + GEM_WIDTH)
		{
			inside = false;
		}
		//Mouse above the gem
		else if (y < position.y)
		{
			inside = false;
		}
		//Mouse below the gem
		else if (y > position.y + GEM_HEIGHT)
		{
			inside = false;
		}

		//Mouse is outside gem
		if (!inside)
		{
			if (pressed) {
				currentSprite = GEM_SPRITE_MOUSE_DOWN;
			}
			else
			{
				currentSprite = GEM_SPRITE_MOUSE_OUT;
			}
		}
		//Mouse is inside gem
		else
		{
			//Set mouse over sprite
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				if (!pressed)
				{
					currentSprite = GEM_SPRITE_MOUSE_OVER_MOTION;
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				//Play the sound effect
				Mix_PlayChannel(-1, selectSound, 0);
				//Change the sprite state
				currentSprite = GEM_SPRITE_MOUSE_DOWN;
				if (pressed)
				{
					//Removed the pressed tag
					setPressed(false);
					pressedGems[0] = NULL;
					pressedCount--;
					break;
				}
				else
				{
					//Tag the gem as pressed
					setPressed(true);
					pressedGems[pressedCount] = this;
					pressedCount++;
					break;
				}
			case SDL_MOUSEBUTTONUP:
				//Change the sprite state
				currentSprite = GEM_SPRITE_MOUSE_DOWN;
				if (pressedCount == 1 && pressedGems[0] != this)
				{
					//Tag the gem as pressed
					setPressed(true);
					pressedGems[pressedCount] = this;
					pressedCount++;
					break;
				}
			}

		}
	}
}

void Gem::render()
{
	if (removed)
	{
		return;
		//type = -1;
	}
	//Show current gem sprite
	if (type == 0)
	{
		color1SpriteSheetTexture.render(position.x, position.y, &color1SpriteClips[currentSprite]);
	}

	if (type == 1)
	{
		color2SpriteSheetTexture.render(position.x, position.y, &color2SpriteClips[currentSprite]);
	}

	if (type == 2)
	{
		color3SpriteSheetTexture.render(position.x, position.y, &color3SpriteClips[currentSprite]);
	}

	if (type == 3)
	{
		color4SpriteSheetTexture.render(position.x, position.y, &color4SpriteClips[currentSprite]);
	}

	if (type == 4)
	{
		color5SpriteSheetTexture.render(position.x, position.y, &color5SpriteClips[currentSprite]);
	}
}