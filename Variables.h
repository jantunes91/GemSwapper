#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Texture.h"
#include "Constants.h"
//#include "Gem.h"

//Gem sprites
extern SDL_Rect color1SpriteClips[GEM_SPRITE_TOTAL];
extern SDL_Rect color2SpriteClips[GEM_SPRITE_TOTAL];
extern SDL_Rect color3SpriteClips[GEM_SPRITE_TOTAL];
extern SDL_Rect color4SpriteClips[GEM_SPRITE_TOTAL];
extern SDL_Rect color5SpriteClips[GEM_SPRITE_TOTAL];
extern Texture color1SpriteSheetTexture;
extern Texture color2SpriteSheetTexture;
extern Texture color3SpriteSheetTexture;
extern Texture color4SpriteSheetTexture;
extern Texture color5SpriteSheetTexture;

//Play button sprites
extern SDL_Rect playSpriteClips[BUTTON_SPRITE_TOTAL];
extern Texture playSpriteSheetTexture;

//Play Again button sprites
extern SDL_Rect playagainSpriteClips[BUTTON_SPRITE_TOTAL];
extern Texture playagainSpriteSheetTexture;

//Play button sprites
extern SDL_Rect quitSpriteClips[BUTTON_SPRITE_TOTAL];
extern Texture quitSpriteSheetTexture;

//Menu background
extern SDL_Rect menuClip;
extern Texture menuTexture;

//Game background
extern SDL_Rect backgroundClip;
extern Texture backgroundTexture;

//Game Over background
extern SDL_Rect gameoverClip;
extern Texture gameoverMovesTexture;
extern Texture gameoverTimeTexture;

//Background music
extern Mix_Music *music;

//The sound effects that will be used
extern Mix_Chunk *selectSound;
extern Mix_Chunk *sequence1;
extern Mix_Chunk *sequence2;
extern Mix_Chunk *sequence3;

//The fonts that are going to be used
extern TTF_Font *scoreFont;
extern TTF_Font *timerFont;

//The text clips
extern SDL_Rect scoreGameTextClip;
extern SDL_Rect scoreGameShadowClip;
extern SDL_Rect scoreGOverTextClip;
extern SDL_Rect scoreGOverShadowClip;
extern SDL_Rect multiplierClip;
extern SDL_Rect multiShadowClip;
extern SDL_Rect timerClip;
extern SDL_Rect timerShadowClip;

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