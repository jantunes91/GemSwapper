#pragma once
#include <SDL.h>
#include "LTexture.h"
#include "Constants.h"
//#include "LButton.h"

//Mouse button sprites
extern SDL_Rect gColor1SpriteClips[BUTTON_SPRITE_TOTAL];
extern SDL_Rect gColor2SpriteClips[BUTTON_SPRITE_TOTAL];
extern SDL_Rect gColor3SpriteClips[BUTTON_SPRITE_TOTAL];
extern SDL_Rect gColor4SpriteClips[BUTTON_SPRITE_TOTAL];
extern SDL_Rect gColor5SpriteClips[BUTTON_SPRITE_TOTAL];
extern LTexture gColor1SpriteSheetTexture;
extern LTexture gColor2SpriteSheetTexture;
extern LTexture gColor3SpriteSheetTexture;
extern LTexture gColor4SpriteSheetTexture;
extern LTexture gColor5SpriteSheetTexture;

//The window renderer
extern SDL_Renderer* gRenderer;

//
extern int pressedCount;

//Stores all the squares
//extern class LButton gButtons[TOTAL_BUTTONS];