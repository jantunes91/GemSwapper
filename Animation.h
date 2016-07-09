#pragma once
#include <SDL.h>
#include "Gem.h"
#include "Variables.h"

class Window;

class Animation
{
public:
	Animation();

	~Animation();

	//linear interpolation between gem's original position and final position
	void rect_lerp(Gem *gem, float f);
	
	//handles the swap animation between two gems
	void swapGemsAnim(Gem *pressedGems[2], Gem gems[TOTAL_GEMS], Window *window);

	//handles the sequence remove animation
	void sequenceRemoveAnim(Gem gems[TOTAL_GEMS], Window *window);

	//handles the gems dropping down on the board animation
	void dropGemsAnim(Gem gems[TOTAL_GEMS], Window *window);

	//calculates the next position in the animation
	bool animate(Uint32 animation_start_time, Uint32 animation_time_total, Gem* gem);
};

