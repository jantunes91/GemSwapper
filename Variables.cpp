#include "Variables.h"

//Gem sprites
SDL_Rect color1SpriteClips[GEM_SPRITE_TOTAL];
SDL_Rect color2SpriteClips[GEM_SPRITE_TOTAL];
SDL_Rect color3SpriteClips[GEM_SPRITE_TOTAL];
SDL_Rect color4SpriteClips[GEM_SPRITE_TOTAL];
SDL_Rect color5SpriteClips[GEM_SPRITE_TOTAL];
Texture color1SpriteSheetTexture;
Texture color2SpriteSheetTexture;
Texture color3SpriteSheetTexture;
Texture color4SpriteSheetTexture;
Texture color5SpriteSheetTexture;

//Play button sprites
SDL_Rect playSpriteClips[BUTTON_SPRITE_TOTAL];
Texture playSpriteSheetTexture;

//Play Again button sprites
SDL_Rect playagainSpriteClips[BUTTON_SPRITE_TOTAL];
Texture playagainSpriteSheetTexture;

//Play button sprites
SDL_Rect quitSpriteClips[BUTTON_SPRITE_TOTAL];
Texture quitSpriteSheetTexture;

//Menu background
SDL_Rect menuClip;
Texture menuTexture;

//Game background
SDL_Rect backgroundClip;
Texture backgroundTexture;

//Game Over background
SDL_Rect gameoverClip;
Texture gameoverMovesTexture;
Texture gameoverTimeTexture;

//The music that will be played
Mix_Music *music = NULL;

//The sound effects that will be used
Mix_Chunk *selectSound = NULL;
Mix_Chunk *sequence1 = NULL;
Mix_Chunk *sequence2 = NULL;
Mix_Chunk *sequence3 = NULL;

//The fonts that are going to be used
TTF_Font *scoreFont;
TTF_Font *timerFont;

//The text clips
SDL_Rect scoreGameTextClip;
SDL_Rect scoreGameShadowClip;
SDL_Rect scoreGOverTextClip;
SDL_Rect scoreGOverShadowClip;
SDL_Rect multiplierClip;
SDL_Rect multiShadowClip;
SDL_Rect timerClip;
SDL_Rect timerShadowClip;

//The text color
SDL_Color textColor = { 0,0,0 };
SDL_Color shadowColor = { 171,164,155 };

//The current score
int score = 0;

//The current multiplier
int multiplier = 0;

//The window renderer
SDL_Renderer *renderer = NULL;

//
int pressedCount = 0;

//Stores all the gems
//class Gem gems[TOTAL_BUTTONS];