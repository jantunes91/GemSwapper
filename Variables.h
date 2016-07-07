#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Texture.h"
#include "Constants.h"
//#include "Gem.h"

//Mouse button sprites
extern SDL_Rect gColor1SpriteClips[BUTTON_SPRITE_TOTAL];
extern SDL_Rect gColor2SpriteClips[BUTTON_SPRITE_TOTAL];
extern SDL_Rect gColor3SpriteClips[BUTTON_SPRITE_TOTAL];
extern SDL_Rect gColor4SpriteClips[BUTTON_SPRITE_TOTAL];
extern SDL_Rect gColor5SpriteClips[BUTTON_SPRITE_TOTAL];
extern Texture gColor1SpriteSheetTexture;
extern Texture gColor2SpriteSheetTexture;
extern Texture gColor3SpriteSheetTexture;
extern Texture gColor4SpriteSheetTexture;
extern Texture gColor5SpriteSheetTexture;

//Window background
extern SDL_Rect backgroundClip;
extern Texture backgroundTexture;

//Background music
extern Mix_Music *music;

//The sound effects that will be used
extern Mix_Chunk *selectGem;
extern Mix_Chunk *sequence1;
extern Mix_Chunk *sequence2;
extern Mix_Chunk *sequence3;

//The font that's going to be used
extern TTF_Font *font;

//The surface that contains the score rendered in the chosen font
extern SDL_Surface *scoreSurface;
extern SDL_Surface *scoreShadowSurface;

//The surface that contains the multiplier rendered in the chosen font
extern SDL_Surface *multiplierSurface;
extern SDL_Surface *multiShadowSurface;

//The texture converted from the surface
extern SDL_Texture* scoreTexture;
extern SDL_Texture* scoreShadowTexture;
extern SDL_Texture* multiplierTexture;
extern SDL_Texture* multiShadowTexture;
extern SDL_Rect scoreTextClip;
extern SDL_Rect scoreShadowClip;
extern SDL_Rect multiplierClip;
extern SDL_Rect multiShadowClip;

//The text color
extern SDL_Color textColor;
extern SDL_Color shadowColor;

//The current score
extern int score;

//The current score
extern int multiplier;

//The window renderer
extern SDL_Renderer *renderer;

//
extern int pressedCount;