#include "Variables.h"

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

//Window background
SDL_Rect backgroundClip;
LTexture backgroundTexture;

//The music that will be played
Mix_Music *music = NULL;

//The sound effects that will be used
Mix_Chunk *selectSquare = NULL;
Mix_Chunk *sequence1 = NULL;
Mix_Chunk *sequence2 = NULL;
Mix_Chunk *sequence3 = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//
int pressedCount = 0;

//Stores all the squares
//class LButton gButtons[TOTAL_BUTTONS];