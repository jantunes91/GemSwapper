/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <Windows.h>
#include "Constants.h"
#include "Gem.h"
#include "Texture.h"
#include "Board.h"
#include "Variables.h"

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* Window = NULL;

//The window renderer
//SDL_Renderer* gRenderer = NULL;

//Buttons objects
Gem gems[TOTAL_GEMS];

//Pressed Buttons
Gem *pressedGems[2];

//Board instance
Board board;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	//Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning: Linear texture filtering not enabled!");
	}

	//Create window
	Window = SDL_CreateWindow("GemSwapper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (Window == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Create vsynced renderer for window
	renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		printf("SDL_Mixer could not initialize! SDL_Mixer Error: %s\n", Mix_GetError());
		return false;
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		printf("SDL_TTF could not initialize! SDL_Mixer Error: %s\n", TTF_GetError());
		return false;
	}

	return true;
}

bool loadMedia()
{
	//Load background image
	if (!backgroundTexture.loadFromFile("Images/background.png", renderer))
	{
		printf("Failed to load background texture!\n");
		return false;
	}
	else
	{
		//Set where to render
		backgroundClip.h = SCREEN_HEIGHT;
		backgroundClip.w = SCREEN_WIDTH;
	}

	//Load sprites for Color 1
	if (!gColor1SpriteSheetTexture.loadFromFile("Images/color1sprite.png",renderer))
	{
		printf("Failed to load color 1 sprite texture!\n");
		return false;
	}
	else
	{
		//Set sprites
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			gColor1SpriteClips[i].x = 0;
			gColor1SpriteClips[i].y = i * 50;
			gColor1SpriteClips[i].w = GEM_WIDTH;
			gColor1SpriteClips[i].h = GEM_HEIGHT;
		}
	}

	//Load sprites for Color 2
	if (!gColor2SpriteSheetTexture.loadFromFile("Images/color2sprite.png", renderer))
	{
		printf("Failed to load color 2 sprite texture!\n");
		return false;
	}
	else
	{
		//Set sprites
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			gColor2SpriteClips[i].x = 0;
			gColor2SpriteClips[i].y = i * 50;
			gColor2SpriteClips[i].w = GEM_WIDTH;
			gColor2SpriteClips[i].h = GEM_HEIGHT;
		}
	}

	//Load sprites for Color 3
	if (!gColor3SpriteSheetTexture.loadFromFile("Images/color3sprite.png", renderer))
	{
		printf("Failed to load color 3 sprite texture!\n");
		return false;
	}
	else
	{
		//Set sprites
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			gColor3SpriteClips[i].x = 0;
			gColor3SpriteClips[i].y = i * 50;
			gColor3SpriteClips[i].w = GEM_WIDTH;
			gColor3SpriteClips[i].h = GEM_HEIGHT;
		}
	}

	//Load sprites for Color 4
	if (!gColor4SpriteSheetTexture.loadFromFile("Images/color4sprite.png", renderer))
	{
		printf("Failed to load color 4 sprite texture!\n");
		return false;
	}
	else
	{
		//Set sprites
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			gColor4SpriteClips[i].x = 0;
			gColor4SpriteClips[i].y = i * 50;
			gColor4SpriteClips[i].w = GEM_WIDTH;
			gColor4SpriteClips[i].h = GEM_HEIGHT;
		}
	}

	//Load sprites for Color 5
	if (!gColor5SpriteSheetTexture.loadFromFile("Images/color5sprite.png", renderer))
	{
		printf("Failed to load color 5 sprite texture!\n");
		return false;
	}
	else
	{
		//Set sprites
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			gColor5SpriteClips[i].x = 0;
			gColor5SpriteClips[i].y = i * 50;
			gColor5SpriteClips[i].w = GEM_WIDTH;
			gColor5SpriteClips[i].h = GEM_HEIGHT;
		}
	}

	//Load the music
	music = Mix_LoadMUS("Sounds/music.wav");

	//If there was a problem loading the music
	if (music == NULL)
	{
		printf("Failed to load the background music!\n");
		return false;
	}

	//Load the sound effects
	selectGem = Mix_LoadWAV("Sounds/select.wav");
	sequence1 = Mix_LoadWAV("Sounds/sequence1.wav");
	sequence2 = Mix_LoadWAV("Sounds/sequence2.wav");
	sequence3 = Mix_LoadWAV("Sounds/sequence3.wav");

	//If there was a problem loading the sound effects
	if ((sequence1 == NULL) || (sequence2 == NULL) || (sequence3 == NULL) || (selectGem == NULL))
	{
		printf("Failed to load the sound effects!\n");
		return false;
	}

	//Open the font
	font = TTF_OpenFont("Fonts/EndlessBossBattle.ttf", 28);

	//If there was a problem loading the font
	if (font == NULL)
	{
		printf("Failed to load the type font!\n");
		return false;
	}
	else
	{
		scoreTextClip.x = 65;
		scoreTextClip.y = 260;
		scoreTextClip.w = 100;
		scoreTextClip.h = 45;

		scoreShadowClip.x = 70;
		scoreShadowClip.y = 265;
		scoreShadowClip.w = 100;
		scoreShadowClip.h = 45;

		multiplierClip.x = 65;
		multiplierClip.y = 370;
		multiplierClip.w = 35;
		multiplierClip.h = 45;

		multiShadowClip.x = 70;
		multiShadowClip.y = 375;
		multiShadowClip.w = 35;
		multiShadowClip.h = 45;
	}

	//Set the correct positions for the gems
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			gems[x + y * 8].setPosition(x * OFFSET_MULTIPLIER + OFFSET_X, y * OFFSET_MULTIPLIER + OFFSET_Y);
			gems[x + y * 8].destX = gems[x + y * 8].getPosition().x;
			gems[x + y * 8].destY = gems[x + y * 8].getPosition().y;
		}
	}

	return true;
}

void render()
{
	//Clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	//Render background
	backgroundTexture.render(0, 0, &backgroundClip);

	//Convert the score from int to char*
	std::string scoreString = std::to_string(score);
	char const *scoreArray = scoreString.c_str();
	//Render the score and the shadow
	scoreSurface = TTF_RenderText_Solid(font, scoreArray, textColor);
	scoreShadowSurface = TTF_RenderText_Solid(font, scoreArray, shadowColor);
	//Convert it to texture
	scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
	scoreShadowTexture = SDL_CreateTextureFromSurface(renderer, scoreShadowSurface);
	//Apply the score to the screen	
	SDL_RenderCopy(renderer, scoreShadowTexture, NULL, &scoreShadowClip);
	SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreTextClip);

	//Convert the multiplier from int to char*
	std::string multiString = std::to_string(multiplier);
	multiString += "x";
	char const *multiArray = multiString.c_str();
	//Render the multiplier and the shadow
	multiplierSurface = TTF_RenderText_Solid(font, multiArray, textColor);
	multiShadowSurface = TTF_RenderText_Solid(font, multiArray, shadowColor);
	//Convert it to texture
	multiplierTexture = SDL_CreateTextureFromSurface(renderer, multiplierSurface);
	multiShadowTexture = SDL_CreateTextureFromSurface(renderer, multiShadowSurface);
	//Apply the multiplier to the screen	
	SDL_RenderCopy(renderer, multiShadowTexture, NULL, &multiShadowClip);
	SDL_RenderCopy(renderer, multiplierTexture, NULL, &multiplierClip);

	//Render gems
	for (int i = 0; i < TOTAL_GEMS; ++i)
	{
		gems[i].render();
	}

	//Update screen
	SDL_RenderPresent(renderer);
}

void close()
{
	//Free loaded images
	gColor1SpriteSheetTexture.free();
	gColor2SpriteSheetTexture.free();
	gColor3SpriteSheetTexture.free();
	gColor4SpriteSheetTexture.free();
	gColor5SpriteSheetTexture.free();
	backgroundTexture.free();

	//Free loaded sounds
	Mix_FreeMusic(music);
	Mix_FreeChunk(sequence1);
	Mix_FreeChunk(sequence2);
	Mix_FreeChunk(sequence3);
	Mix_FreeChunk(selectGem);

	//Free the font that was used
	TTF_CloseFont(font);

	//Destroy window	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(Window);
	Window = NULL;
	renderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//render the initial board
			render();

			//play the background music on loop
			Mix_PlayMusic(music, -1);

			//this delay is just for eventual sequences on the initial
			//board to not vanish right when we load the game
			SDL_Delay(500);

			//clear initial sequences and drop new gems
			//loop only stops when there's no more sequences to clear
			while (board.checkSequence(gems))
			{
				board.dropDownGems(gems);
			}
			multiplier = 1;

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					//Handle button events
					for (int i = 0; i < TOTAL_GEMS; ++i)
					{
						gems[i].handleEvent(&e,pressedGems);
						if (pressedCount == 2)
						{
							board.swapGems(pressedGems, gems);
						}
					}
				}

				render();
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}