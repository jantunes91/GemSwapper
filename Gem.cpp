#include "Gem.h"
#include "Constants.h"
#include "Variables.h"

Gem::Gem()
{
	mPosition.x = OFFSET_X;
	mPosition.y = OFFSET_Y;

	mCurrentSprite = GEM_SPRITE_MOUSE_OUT;

	type = rand() % 5;

	removed = false;

	updatePosition = false;
	destY = 0;
	destX = 0;
}

void Gem::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

SDL_Point Gem::getPosition()
{
	return mPosition;
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

void Gem::handleEvent(SDL_Event* e, Gem* gPressedButtons[2])
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
		if (x < mPosition.x)
		{
			inside = false;
		}
		//Mouse is right of the gem
		else if (x > mPosition.x + GEM_WIDTH)
		{
			inside = false;
		}
		//Mouse above the gem
		else if (y < mPosition.y)
		{
			inside = false;
		}
		//Mouse below the gem
		else if (y > mPosition.y + GEM_HEIGHT)
		{
			inside = false;
		}

		//Mouse is outside gem
		if (!inside)
		{
			if (pressed) {
				mCurrentSprite = GEM_SPRITE_MOUSE_DOWN;
			}
			else
			{
				mCurrentSprite = GEM_SPRITE_MOUSE_OUT;
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
					mCurrentSprite = GEM_SPRITE_MOUSE_OVER_MOTION;
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				//Play the sound effect
				Mix_PlayChannel(-1, selectSound, 0);
				//Change the sprite state
				mCurrentSprite = GEM_SPRITE_MOUSE_DOWN;
				if (pressed)
				{
					//Removed the pressed tag
					setPressed(false);
					gPressedButtons[0] = NULL;
					pressedCount--;
					break;
				}
				else
				{
					//Tag the gem as pressed
					setPressed(true);
					gPressedButtons[pressedCount] = this;
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
		color1SpriteSheetTexture.render(mPosition.x, mPosition.y, &color1SpriteClips[mCurrentSprite]);
	}

	if (type == 1)
	{
		color2SpriteSheetTexture.render(mPosition.x, mPosition.y, &color2SpriteClips[mCurrentSprite]);
	}

	if (type == 2)
	{
		color3SpriteSheetTexture.render(mPosition.x, mPosition.y, &color3SpriteClips[mCurrentSprite]);
	}

	if (type == 3)
	{
		color4SpriteSheetTexture.render(mPosition.x, mPosition.y, &color4SpriteClips[mCurrentSprite]);
	}

	if (type == 4)
	{
		color5SpriteSheetTexture.render(mPosition.x, mPosition.y, &color5SpriteClips[mCurrentSprite]);
	}
}