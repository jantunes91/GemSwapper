#include "RenderVariables.h"

//Mouse button sprites
SDL_Rect gColor1SpriteClips[BUTTON_SPRITE_TOTAL];
SDL_Rect gColor2SpriteClips[BUTTON_SPRITE_TOTAL];
SDL_Rect gColor3SpriteClips[BUTTON_SPRITE_TOTAL];
SDL_Rect gColor4SpriteClips[BUTTON_SPRITE_TOTAL];
SDL_Rect gColor5SpriteClips[BUTTON_SPRITE_TOTAL];
LTexture gColor1SpriteSheetTexture;
LTexture gColor2SpriteSheetTexture;
LTexture gColor3SpriteSheetTexture;
LTexture gColor4SpriteSheetTexture;
LTexture gColor5SpriteSheetTexture;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//
int pressedCount = 0;

//Stores all the squares
//class LButton gButtons[TOTAL_BUTTONS];