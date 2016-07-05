#include "LButton.h"
#include "Constants.h"
#include "Variables.h"

LButton::LButton()
{
	mPosition.x = OFFSET_X;
	mPosition.y = OFFSET_Y;

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;

	type = rand() % 5;

	removed = false;

	updatePosition = false;
	destY = 0;
	destX = 0;
}

void LButton::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

SDL_Point LButton::getPosition()
{
	return mPosition;
}

void LButton::setPressed(bool isPressed)
{
	pressed = isPressed;
}

int LButton::getType()
{
	return type;
}

void LButton::setType(int tempType)
{
	type = tempType;
}

void LButton::setRemoved(bool isRemoved)
{
	removed = isRemoved;
}

bool LButton::isRemoved()
{
	return removed;
}

void LButton::setToUpdate(bool toUpdate)
{
	updatePosition = toUpdate;
}

bool LButton::toUpdate()
{
	return updatePosition;
}

void LButton::handleEvent(SDL_Event* e, LButton* gPressedButtons[2])
{
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if (x < mPosition.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > mPosition.x + BUTTON_WIDTH)
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < mPosition.y)
		{
			inside = false;
		}
		//Mouse below the button
		else if (y > mPosition.y + BUTTON_HEIGHT)
		{
			inside = false;
		}

		//Mouse is outside button
		if (!inside && !pressed)
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				if (!pressed)
				{
					mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				//Play the sound effect
				Mix_PlayChannel(-1, selectSquare, 0);
				//Change the sprite state
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				//Tag the button as pressed
				setPressed(true);
				gPressedButtons[pressedCount] = this;
				pressedCount++;
				break;
			}
		}
	}
}

void LButton::render()
{
	if (removed)
	{
		return;
		//type = -1;
	}
	//Show current button sprite
	if (type == 0)
	{
		gColor1SpriteSheetTexture.render(mPosition.x, mPosition.y, &gColor1SpriteClips[mCurrentSprite]);
	}

	if (type == 1)
	{
		gColor2SpriteSheetTexture.render(mPosition.x, mPosition.y, &gColor2SpriteClips[mCurrentSprite]);
	}

	if (type == 2)
	{
		gColor3SpriteSheetTexture.render(mPosition.x, mPosition.y, &gColor3SpriteClips[mCurrentSprite]);
	}

	if (type == 3)
	{
		gColor4SpriteSheetTexture.render(mPosition.x, mPosition.y, &gColor4SpriteClips[mCurrentSprite]);
	}

	if (type == 4)
	{
		gColor5SpriteSheetTexture.render(mPosition.x, mPosition.y, &gColor5SpriteClips[mCurrentSprite]);
	}
}