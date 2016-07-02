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
	void swapSquaresAnim(LButton *gPressedButtons[2]);

	bool animate(Uint32 tcurrent, LButton* button);

private:
	//stores the animation start time
	///Uint32 animation_start_time;

	//whole animation will play in 0.3 s
	const Uint32 animation_time_total = 300;
};

