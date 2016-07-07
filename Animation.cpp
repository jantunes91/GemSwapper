#include "Animation.h"

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

void Animation::swapGemsAnim(Gem *gPressedButtons[2], Gem gems[TOTAL_GEMS])
{
	//set the correct origin point values
	gPressedButtons[0]->origX = gPressedButtons[1]->getPosition().x;
	gPressedButtons[0]->origY = gPressedButtons[1]->getPosition().y;
	gPressedButtons[1]->origX = gPressedButtons[0]->getPosition().x;
	gPressedButtons[1]->origY = gPressedButtons[0]->getPosition().y;
	//set the correct destiny point values
	gPressedButtons[0]->destX = gPressedButtons[0]->getPosition().x;
	gPressedButtons[0]->destY = gPressedButtons[0]->getPosition().y;
	gPressedButtons[1]->destX = gPressedButtons[1]->getPosition().x;
	gPressedButtons[1]->destY = gPressedButtons[1]->getPosition().y;

	//time of current frame
	Uint32 tcurrent = SDL_GetTicks();

	bool animation0Done = false;
	bool animation1Done = false;

	while (!animation0Done && !animation1Done)
	{
		animation0Done = animate(tcurrent, SWAP_ANIMATION_TIME, gPressedButtons[0]);
		animation1Done = animate(tcurrent, SWAP_ANIMATION_TIME, gPressedButtons[1]);

		render(gems);
	}
}

void Animation::sequenceRemoveAnim(Gem gems[TOTAL_GEMS])
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
		render(gems);
	}
}

void Animation::dropGemsAnim(Gem gems[TOTAL_GEMS])
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
			render(gems);
		}
	}
}

void Animation::render(Gem gems[TOTAL_GEMS])
{
	//Clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	//Render background
	backgroundTexture.render(0, 0, &backgroundClip);

	std::string scoreString = std::to_string(score);
	char const *scoreArray = scoreString.c_str();
	//Render the score and the shadow
	scoreSurface = TTF_RenderText_Solid(font, scoreArray, textColor);
	scoreShadowSurface = TTF_RenderText_Solid(font, scoreArray, shadowColor);
	//Convert it to texture
	scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
	scoreShadowTexture = SDL_CreateTextureFromSurface(renderer, scoreShadowSurface);
	//Apply the score to the screen
	SDL_RenderCopy(renderer, scoreShadowTexture, NULL, &scoreShadowClip);
	SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreTextClip);

	//Convert the multiplier from int to char*
	std::string multiString = std::to_string(multiplier);
	multiString += "x";
	char const *multiArray = multiString.c_str();
	//Render the multiplier and the shadow
	multiplierSurface = TTF_RenderText_Solid(font, multiArray, textColor);
	multiShadowSurface = TTF_RenderText_Solid(font, multiArray, shadowColor);
	//Convert it to texture
	multiplierTexture = SDL_CreateTextureFromSurface(renderer, multiplierSurface);
	multiShadowTexture = SDL_CreateTextureFromSurface(renderer, multiShadowSurface);
	//Apply the multiplier to the screen	
	SDL_RenderCopy(renderer, multiShadowTexture, NULL, &multiShadowClip);
	SDL_RenderCopy(renderer, multiplierTexture, NULL, &multiplierClip);

	//Render gems
	for (int i = 0; i < TOTAL_GEMS; ++i)
	{
		gems[i].render(); //change to render with animation
	}

	//Update screen
	SDL_RenderPresent(renderer);
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