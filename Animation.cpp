#include "Animation.h"



Animation::Animation()
{
}


Animation::~Animation()
{
}

void Animation::rect_lerp(LButton *button, float f) {
	float t = 1.0f - f;
	button->setPosition((float)button->origX * t + (float)button->destX * f, 
		(float)button->origY * t + (float)button->destY * f);
}

void Animation::swapSquaresAnim(LButton *gPressedButtons[2], LButton gButtons[TOTAL_BUTTONS])
{
	//set the correct origin point values
	gPressedButtons[0]->origX = gPressedButtons[0]->getPosition().x;
	gPressedButtons[0]->origY = gPressedButtons[0]->getPosition().y;
	gPressedButtons[1]->origX = gPressedButtons[1]->getPosition().x;
	gPressedButtons[1]->origY = gPressedButtons[1]->getPosition().y;
	//set the correct destiny point values
	gPressedButtons[0]->destX = gPressedButtons[1]->getPosition().x;
	gPressedButtons[0]->destY = gPressedButtons[1]->getPosition().y;
	gPressedButtons[1]->destX = gPressedButtons[0]->getPosition().x;
	gPressedButtons[1]->destY = gPressedButtons[0]->getPosition().y;

	//time of current frame
	Uint32 tcurrent = SDL_GetTicks();

	bool animation0Done = false;
	bool animation1Done = false;

	while (!animation0Done && !animation1Done)
	{
		animation0Done = animate(tcurrent, gPressedButtons[0]);
		animation1Done = animate(tcurrent, gPressedButtons[1]);

		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		//Render buttons
		for (int i = 0; i < TOTAL_BUTTONS; ++i)
		{
			gButtons[i].render(); //change to render with animation
		}

		//Update screen
		SDL_RenderPresent(gRenderer);
	}
}

bool Animation::animate(Uint32 animation_start_time, LButton* button)
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

		rect_lerp(button, factor);

		return false;
	}
}