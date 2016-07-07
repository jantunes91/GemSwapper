#pragma once
#include <SDL.h>
#include "Gem.h"

class Animation
{
public:
	Animation();

	~Animation();

	//linear interpolation between gem0 position and gem1 position
	void rect_lerp(Gem *gem, float f);
	
	//handles the swap animation between two gems
	void swapGemsAnim(Gem *gPressedButtons[2], Gem gems[TOTAL_GEMS]);

	void sequenceRemoveAnim(Gem gems[TOTAL_GEMS]);

	void dropGemsAnim(Gem gems[TOTAL_GEMS]);

	//renders the board
	void render(Gem gems[TOTAL_GEMS]);

	bool animate(Uint32 animation_start_time, Uint32 animation_time_total, Gem* gem);
};

