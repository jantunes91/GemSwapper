#include "Variables.h"

//Mouse gem sprites
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

//Mouse button sprites
SDL_Rect playSpriteClips[BUTTON_SPRITE_TOTAL];
Texture playSpriteSheetTexture;

//Menu background
SDL_Rect menuClip;
Texture menuTexture;

//Window background
SDL_Rect backgroundClip;
Texture backgroundTexture;

//The music that will be played
Mix_Music *music = NULL;

//The sound effects that will be used
Mix_Chunk *selectSound = NULL;
Mix_Chunk *sequence1 = NULL;
Mix_Chunk *sequence2 = NULL;
Mix_Chunk *sequence3 = NULL;

//The font that's going to be used
TTF_Font *font = NULL;

//The surface that contains the score rendered in the chosen font
SDL_Surface *scoreSurface;
SDL_Surface *scoreShadowSurface;

//The surface that contains the multiplier rendered in the chosen font
SDL_Surface *multiplierSurface;
SDL_Surface *multiShadowSurface;

//The texture converted from the surface
SDL_Texture* scoreTexture;
SDL_Texture* scoreShadowTexture;
SDL_Texture* multiplierTexture;
SDL_Texture* multiShadowTexture;
SDL_Rect scoreTextClip;
SDL_Rect scoreShadowClip;
SDL_Rect multiplierClip;
SDL_Rect multiShadowClip;

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