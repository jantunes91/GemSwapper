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
#include "Button.h"

//Starts up SDL and creates window
bool init();

//Loads media
bool loadGame();

//Frees media and shuts down SDL
void closeGame();

//The window we'll be rendering to
SDL_Window* Window = NULL;

//The window renderer
//SDL_Renderer* gRenderer = NULL;

//The PLAY button
Button playButton;

//The PLAY AGAIN button
Button playagainButton;

//The QUIT button
Button quitButton;

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

bool loadMenu()
{
	//Load menu background image
	if (!menuTexture.loadFromFile("Images/menu.png", renderer))
	{
		printf("Failed to load menu background texture!\n");
		return false;
	}
	else
	{
		//Set where to render
		menuClip.h = SCREEN_HEIGHT;
		menuClip.w = SCREEN_WIDTH;
	}

	//Load sprites for Play button
	if (!playSpriteSheetTexture.loadFromFile("Images/playsprite.png", renderer))
	{
		printf("Failed to load play button sprite texture!\n");
		return false;
	}
	else
	{
		//Set sprites
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			playSpriteClips[i].x = 0;
			playSpriteClips[i].y = i * 92;
			playSpriteClips[i].w = 271;
			playSpriteClips[i].h = 92;
		}
	}

	//Set the correct position for the buttons
	playButton.setPosition(376, 500);
	playButton.setDimensions(271, 92);
	playButton.setID("play");

	return true;
}

bool loadGameOver()
{
	//Load Game Over background image
	if (!gameoverTexture.loadFromFile("Images/gameover.png", renderer))
	{
	printf("Failed to load gameover background texture!\n");
	return false;
}
	else
	{
		//Set where to render
		gameoverClip.h = SCREEN_HEIGHT;
		gameoverClip.w = SCREEN_WIDTH;
	}

	//Load sprites for Play Again button
	if (!playagainSpriteSheetTexture.loadFromFile("Images/playagainsprite.png", renderer))
	{
		printf("Failed to load play again button sprite texture!\n");
		return false;
	}
	else
	{
		//Set sprites
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			playagainSpriteClips[i].x = 0;
			playagainSpriteClips[i].y = i * 101;
			playagainSpriteClips[i].w = 574;
			playagainSpriteClips[i].h = 101;
		}
	}

	//Load sprites for Quit button
	if (!quitSpriteSheetTexture.loadFromFile("Images/quitsprite.png", renderer))
	{
		printf("Failed to load Quit button sprite texture!\n");
		return false;
	}
	else
	{
		//Set sprites
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			quitSpriteClips[i].x = 0;
			quitSpriteClips[i].y = i * 101;
			quitSpriteClips[i].w = 247;
			quitSpriteClips[i].h = 101;
		}
	}

	//Set the correct position for the buttons
	playagainButton.setPosition(78, 572);
	playagainButton.setDimensions(574, 101);
	playagainButton.setID("playagain");

	quitButton.setPosition(707, 572);
	quitButton.setDimensions(247, 101);
	quitButton.setID("quit");

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
		scoreGOverTextClip.y = 280;
		scoreGameTextClip.h = 45;

		scoreGameShadowClip.y = 285;
		scoreGameShadowClip.h = 45;
	}

	return true;
}

bool loadGame()
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
	if (!color1SpriteSheetTexture.loadFromFile("Images/color1sprite.png",renderer))
	{
		printf("Failed to load color 1 sprite texture!\n");
		return false;
	}
	else
	{
		//Set sprites
		for (int i = 0; i < GEM_SPRITE_TOTAL; ++i)
		{
			color1SpriteClips[i].x = 0;
			color1SpriteClips[i].y = i * 50;
			color1SpriteClips[i].w = GEM_WIDTH;
			color1SpriteClips[i].h = GEM_HEIGHT;
		}
	}

	//Load sprites for Color 2
	if (!color2SpriteSheetTexture.loadFromFile("Images/color2sprite.png", renderer))
	{
		printf("Failed to load color 2 sprite texture!\n");
		return false;
	}
	else
	{
		//Set sprites
		for (int i = 0; i < GEM_SPRITE_TOTAL; ++i)
		{
			color2SpriteClips[i].x = 0;
			color2SpriteClips[i].y = i * 50;
			color2SpriteClips[i].w = GEM_WIDTH;
			color2SpriteClips[i].h = GEM_HEIGHT;
		}
	}

	//Load sprites for Color 3
	if (!color3SpriteSheetTexture.loadFromFile("Images/color3sprite.png", renderer))
	{
		printf("Failed to load color 3 sprite texture!\n");
		return false;
	}
	else
	{
		//Set sprites
		for (int i = 0; i < GEM_SPRITE_TOTAL; ++i)
		{
			color3SpriteClips[i].x = 0;
			color3SpriteClips[i].y = i * 50;
			color3SpriteClips[i].w = GEM_WIDTH;
			color3SpriteClips[i].h = GEM_HEIGHT;
		}
	}

	//Load sprites for Color 4
	if (!color4SpriteSheetTexture.loadFromFile("Images/color4sprite.png", renderer))
	{
		printf("Failed to load color 4 sprite texture!\n");
		return false;
	}
	else
	{
		//Set sprites
		for (int i = 0; i < GEM_SPRITE_TOTAL; ++i)
		{
			color4SpriteClips[i].x = 0;
			color4SpriteClips[i].y = i * 50;
			color4SpriteClips[i].w = GEM_WIDTH;
			color4SpriteClips[i].h = GEM_HEIGHT;
		}
	}

	//Load sprites for Color 5
	if (!color5SpriteSheetTexture.loadFromFile("Images/color5sprite.png", renderer))
	{
		printf("Failed to load color 5 sprite texture!\n");
		return false;
	}
	else
	{
		//Set sprites
		for (int i = 0; i < GEM_SPRITE_TOTAL; ++i)
		{
			color5SpriteClips[i].x = 0;
			color5SpriteClips[i].y = i * 50;
			color5SpriteClips[i].w = GEM_WIDTH;
			color5SpriteClips[i].h = GEM_HEIGHT;
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
	selectSound = Mix_LoadWAV("Sounds/select.wav");
	sequence1 = Mix_LoadWAV("Sounds/sequence1.wav");
	sequence2 = Mix_LoadWAV("Sounds/sequence2.wav");
	sequence3 = Mix_LoadWAV("Sounds/sequence3.wav");

	//If there was a problem loading the sound effects
	if ((sequence1 == NULL) || (sequence2 == NULL) || (sequence3 == NULL) || (selectSound == NULL))
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
		scoreGameTextClip.x = 65;
		scoreGameTextClip.y = 260;
		scoreGameTextClip.w = 100;
		scoreGameTextClip.h = 45;

		scoreGameShadowClip.x = 70;
		scoreGameShadowClip.y = 265;
		scoreGameShadowClip.w = 100;
		scoreGameShadowClip.h = 45;

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
			//gems[x + y * 8].origX = gems[x + y * 8].getPosition().x;
			//gems[x + y * 8].origY = (y - 8) * OFFSET_MULTIPLIER + OFFSET_Y;
			gems[x + y * 8].destX = gems[x + y * 8].getPosition().x;
			gems[x + y * 8].destY = gems[x + y * 8].getPosition().y;
		}
	}

	return true;
}

void renderMenu()
{
	//Clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	//Render background
	menuTexture.render(0, 0, &menuClip);

	//Render PLAY button
	playButton.render();

	//Update screen
	SDL_RenderPresent(renderer);
}

void renderGameOver()
{
	//Clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	//Render background
	gameoverTexture.render(0, 0, &gameoverClip);

	//Render PLAY AGAIN button
	playagainButton.render();

	//Render QUIT button
	quitButton.render();

	//Convert the score from int to char*
	std::string scoreString = std::to_string(score);
	int scoreLength = scoreString.length();
	char const *scoreArray = scoreString.c_str();
	//Render the score and the shadow
	SDL_Surface *scoreSurface = TTF_RenderText_Solid(font, scoreArray, textColor);
	SDL_Surface *scoreShadowSurface = TTF_RenderText_Solid(font, scoreArray, shadowColor);
	//Convert it to texture
	SDL_Texture *scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
	SDL_Texture *scoreShadowTexture = SDL_CreateTextureFromSurface(renderer, scoreShadowSurface);
	//Free the surfaces
	SDL_FreeSurface(scoreSurface);
	SDL_FreeSurface(scoreShadowSurface);
	//Calculate clip dimensions
	scoreGOverTextClip.x = 0;//SCREEN_WIDTH / 2 - 45 * (scoreLength / 2);
	scoreGOverShadowClip.x = 0;// scoreGOverTextClip.x + 5;
	scoreGOverTextClip.w = scoreGOverShadowClip.w = 45 * scoreLength;
	//Apply the score to the screen	
	SDL_RenderCopy(renderer, scoreShadowTexture, NULL, &scoreGOverShadowClip);
	SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreGOverTextClip);

	//Update screen
	SDL_RenderPresent(renderer);

	//Free the textures
	SDL_DestroyTexture(scoreTexture);
	SDL_DestroyTexture(scoreShadowTexture);
}

void renderGame()
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
	SDL_Surface *scoreSurface = TTF_RenderText_Solid(font, scoreArray, textColor);
	SDL_Surface *scoreShadowSurface = TTF_RenderText_Solid(font, scoreArray, shadowColor);
	//Convert it to texture
	SDL_Texture *scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
	SDL_Texture *scoreShadowTexture = SDL_CreateTextureFromSurface(renderer, scoreShadowSurface);
	//Free the surfaces
	SDL_FreeSurface(scoreSurface);
	SDL_FreeSurface(scoreShadowSurface);
	//Apply the score to the screen	
	SDL_RenderCopy(renderer, scoreShadowTexture, NULL, &scoreGameShadowClip);
	SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreGameTextClip);

	//Convert the multiplier from int to char*
	std::string multiString = std::to_string(multiplier);
	multiString += "x";
	char const *multiArray = multiString.c_str();
	//Render the multiplier and the shadow
	SDL_Surface *multiplierSurface = TTF_RenderText_Solid(font, multiArray, textColor);
	SDL_Surface *multiShadowSurface = TTF_RenderText_Solid(font, multiArray, shadowColor);
	//Convert it to texture
	SDL_Texture *multiplierTexture = SDL_CreateTextureFromSurface(renderer, multiplierSurface);
	SDL_Texture *multiShadowTexture = SDL_CreateTextureFromSurface(renderer, multiShadowSurface);
	//Free the surfaces
	SDL_FreeSurface(multiplierSurface);
	SDL_FreeSurface(multiShadowSurface);
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

	//Free the textures
	SDL_DestroyTexture(scoreTexture);
	SDL_DestroyTexture(scoreShadowTexture);
	SDL_DestroyTexture(multiplierTexture);
	SDL_DestroyTexture(multiShadowTexture);

}

void closeMenu()
{
	//Free loaded images
	playSpriteSheetTexture.free();
	menuTexture.free();
}

void closeGameOver()
{
	//Free the font that was used
	TTF_CloseFont(font);

	//Free loaded images
	playagainSpriteSheetTexture.free();
	quitSpriteSheetTexture.free();
	gameoverTexture.free();
}

void closeGame()
{
	//Free the font that was used
	TTF_CloseFont(font);

	//Free loaded images
	color1SpriteSheetTexture.free();
	color2SpriteSheetTexture.free();
	color3SpriteSheetTexture.free();
	color4SpriteSheetTexture.free();
	color5SpriteSheetTexture.free();
	backgroundTexture.free();

	//Free loaded sounds
	Mix_FreeMusic(music);
	Mix_FreeChunk(sequence1);
	Mix_FreeChunk(sequence2);
	Mix_FreeChunk(sequence3);
	Mix_FreeChunk(selectSound);
}

void closeWindow()
{
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
	//Seed for the random function
	srand((unsigned)time(NULL));

	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Menu loop flag
		bool playGame = false;

		//Game loop flag
		bool quitGame = false;

		//Game Over loop flag
		bool gameOver = false;

		//Event handler
		SDL_Event e;

		//Load Menu
		if (!loadMenu())
		{
			printf("Failed to load menu!\n");
		}
		else
		{
			//Menu loop
			while (!playGame)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)
					{
						quitGame = true;
						playGame = true;
						break;
					}

					playGame = playButton.handleEvent(&e);
				}
				renderMenu();
			}
			closeMenu();
		}

		//Load Game
		if (!loadGame())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			if (!quitGame)
			{
				//render the initial board
				renderGame();

				//play the background music on loop
				Mix_PlayMusic(music, -1);

				//this delay is just for eventual sequences on the initial
				//board to not vanish right when we load the game
				SDL_Delay(500);

				//clear initial sequences and drop new gems
				//loop only stops when there's no more sequences to clear
				while (board.checkSequence(gems,false))
				{
					board.dropDownGems(gems);
				}
				quitGame = !board.checkAvailableMoves(gems);
				multiplier = 1;
			}


			//While application is running
			while (!quitGame && !gameOver)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)
					{
						quitGame = true;
					}

					//Handle button events
					for (int i = 0; i < TOTAL_GEMS; ++i)
					{
						gems[i].handleEvent(&e,pressedGems);
						if (pressedCount == 2)
						{
							board.swapGems(pressedGems, gems);
							gameOver = board.checkAvailableMoves(gems);
						}
					}
				}
				renderGame();
			}
		}

		//Load Menu
		if (!loadGameOver())
		{
			printf("Failed to load game over screen!\n");
		}
		else
		{
			//Menu loop
			while (gameOver && !quitGame)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)
					{
						quitGame = true;
						break;
					}
					bool playAgain = playagainButton.handleEvent(&e);
					if (playAgain)
					{
						quitGame = true;
						//closeGameOver();
						//closeGame();
						//call main function again
						break;
					}

					quitGame = quitButton.handleEvent(&e);
				}
				renderGameOver();
			}
			closeGameOver();
		}
	}

	//Free resources and close SDL
	closeGame();
	closeWindow();

	return 0;
}