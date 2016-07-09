#include "Animation.h"
#include "Window.h"

#define SWAP_ANIMATION_TIME 300
#define COLUMN_DROP_ANIMATION_TIME 1000

Animation::Animation()
{
}


Animation::~Animation()
{
}

void Animation::rect_lerp(Gem *gem, float f) {
	float t = 1.0f - f;
	gem->setPosition((float)gem->origX * t + (float)gem->destX * f, 
		(float)gem->origY * t + (float)gem->destY * f);
}

void Animation::swapGemsAnim(Gem *pressedGems[2], Gem gems[TOTAL_GEMS], Window *window)
{
	//set the correct origin point values
	pressedGems[0]->origX = pressedGems[1]->getPosition().x;
	pressedGems[0]->origY = pressedGems[1]->getPosition().y;
	pressedGems[1]->origX = pressedGems[0]->getPosition().x;
	pressedGems[1]->origY = pressedGems[0]->getPosition().y;
	//set the correct destiny point values
	pressedGems[0]->destX = pressedGems[0]->getPosition().x;
	pressedGems[0]->destY = pressedGems[0]->getPosition().y;
	pressedGems[1]->destX = pressedGems[1]->getPosition().x;
	pressedGems[1]->destY = pressedGems[1]->getPosition().y;

	//time of current frame
	Uint32 tcurrent = SDL_GetTicks();

	bool animation0Done = false;
	bool animation1Done = false;

	while (!animation0Done && !animation1Done)
	{
		animation0Done = animate(tcurrent, SWAP_ANIMATION_TIME, pressedGems[0]);
		animation1Done = animate(tcurrent, SWAP_ANIMATION_TIME, pressedGems[1]);

		window->renderGame();
	}
}

void Animation::sequenceRemoveAnim(Gem gems[TOTAL_GEMS], Window *window)
{
	bool removed[TOTAL_GEMS];

	//store the previous values of the gems' removed flag
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			removed[x + y * 8] = gems[x + y * 8].isRemoved();
		}
	}

	//alternate removed between true and false 4 times. this will cause the gem to flash 4 times
	for (int i = 0; i < 4; i++)
	{
		for (int y = 0; y < 8; y++)
		{
			for (int x = 0; x < 8; x++)
			{
				if (removed[x + y * 8])
				{
					if (gems[x + y * 8].isRemoved())
					{
						gems[x + y * 8].setRemoved(false);
					}
					else
					{
						gems[x + y * 8].setRemoved(true);
					}
				}
			}
		}
		SDL_Delay(100);
		window->renderGame();
	}
}

void Animation::dropGemsAnim(Gem gems[TOTAL_GEMS], Window *window)
{
	bool animationDone = false;
	int tcurrent = SDL_GetTicks();
	//play the animations till they're all finished
	while (!animationDone)
	{
		animationDone = true;
		for (int x = 0; x < 8; x++)
		{
			for (int y = 7; y >= 0; y--)
			{
				// If the gem is not empty and has to fall
				if (gems[x + y * 8].toUpdate() == true && gems[x + y * 8].isRemoved() == false)
				{
					//updates the animation and computes if all the animations are done
					animationDone = animationDone & animate(tcurrent, 200, &gems[x + y * 8]);
				}
			}
		}
		if (!animationDone)
		{
			window->renderGame();
		}
	}
}

bool Animation::animate(Uint32 animation_start_time, Uint32 animation_time_total, Gem* gem)
{
	Uint32 tcurrent = SDL_GetTicks();
	if (tcurrent > animation_start_time + animation_time_total) {
		// if the animation is complete
		return true;
	}
	else {
		// animation is incomplete - interpolate coordinates
		// calculate current animation percentage - in range [0; 1]
		float factor = ((float)(tcurrent - animation_start_time)) / animation_time_total;

		rect_lerp(gem, factor);

		return false;
	}
}