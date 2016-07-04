#pragma once
#include <SDL.h>
#include "LButton.h"

class Animation
{
public:
	Animation();

	~Animation();

	//linear interpolation between button0 position and button1 position
	void rect_lerp(LButton *button, float f);
	
	//handles the swap animation between two squares
	void swapSquaresAnim(LButton *gPressedButtons[2], LButton gButtons[TOTAL_BUTTONS]);

	void sequenceRemoveAnim(LButton gButtons[TOTAL_BUTTONS]);

	//renders the board
	void render(LButton gButtons[TOTAL_BUTTONS]);

	bool animate(Uint32 animation_start_time, Uint32 animation_time_total, LButton* button);
};

