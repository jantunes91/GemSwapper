#include "Variables.h"

//Mouse gem sprites
SDL_Rect gColor1SpriteClips[BUTTON_SPRITE_TOTAL];
SDL_Rect gColor2SpriteClips[BUTTON_SPRITE_TOTAL];
SDL_Rect gColor3SpriteClips[BUTTON_SPRITE_TOTAL];
SDL_Rect gColor4SpriteClips[BUTTON_SPRITE_TOTAL];
SDL_Rect gColor5SpriteClips[BUTTON_SPRITE_TOTAL];
Texture gColor1SpriteSheetTexture;
Texture gColor2SpriteSheetTexture;
Texture gColor3SpriteSheetTexture;
Texture gColor4SpriteSheetTexture;
Texture gColor5SpriteSheetTexture;

//Window background
SDL_Rect backgroundClip;
Texture backgroundTexture;

//The music that will be played
Mix_Music *music = NULL;

//The sound effects that will be used
Mix_Chunk *selectGem = NULL;
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