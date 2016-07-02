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
#include "LButton.h"
#include "LTexture.h"
#include "Game.h"
#include "RenderVariables.h"

//int pressedCount;

////Mouse button sprites
//SDL_Rect gColor1SpriteClips[BUTTON_SPRITE_TOTAL];
//SDL_Rect gColor2SpriteClips[BUTTON_SPRITE_TOTAL];
//SDL_Rect gColor3SpriteClips[BUTTON_SPRITE_TOTAL];
//SDL_Rect gColor4SpriteClips[BUTTON_SPRITE_TOTAL];
//SDL_Rect gColor5SpriteClips[BUTTON_SPRITE_TOTAL];
//LTexture gColor1SpriteSheetTexture;
//LTexture gColor2SpriteSheetTexture;
//LTexture gColor3SpriteSheetTexture;
//LTexture gColor4SpriteSheetTexture;
//LTexture gColor5SpriteSheetTexture;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

////Swaps two squares
//void swapSquares();
//
////Checks if there's a sequence and removes it from the board
//bool checkSequence();
//
////Removes a previously found sequence, given it's end and it's lenght
//void removeSequence(int x, int y, int lenght, int orientation);
//
//void dropDownSquares();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
//SDL_Renderer* gRenderer = NULL;

//Buttons objects
LButton gButtons[TOTAL_BUTTONS];

//Pressed Buttons
LButton *gPressedButtons[2];

//Game instance
Game game;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("Miniclip Challenge", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprites for Color 1
	if (!gColor1SpriteSheetTexture.loadFromFile("Images/color1sprite.png",gRenderer))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else
	{
		//Set sprites
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			gColor1SpriteClips[i].x = 0;
			gColor1SpriteClips[i].y = i * 50;
			gColor1SpriteClips[i].w = BUTTON_WIDTH;
			gColor1SpriteClips[i].h = BUTTON_HEIGHT;
		}
	}

	//Load sprites for Color 2
	if (!gColor2SpriteSheetTexture.loadFromFile("Images/color2sprite.png", gRenderer))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else
	{
		//Set sprites
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			gColor2SpriteClips[i].x = 0;
			gColor2SpriteClips[i].y = i * 50;
			gColor2SpriteClips[i].w = BUTTON_WIDTH;
			gColor2SpriteClips[i].h = BUTTON_HEIGHT;
		}
	}

	//Load sprites for Color 3
	if (!gColor3SpriteSheetTexture.loadFromFile("Images/color3sprite.png", gRenderer))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else
	{
		//Set sprites
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			gColor3SpriteClips[i].x = 0;
			gColor3SpriteClips[i].y = i * 50;
			gColor3SpriteClips[i].w = BUTTON_WIDTH;
			gColor3SpriteClips[i].h = BUTTON_HEIGHT;
		}
	}

	//Load sprites for Color 4
	if (!gColor4SpriteSheetTexture.loadFromFile("Images/color4sprite.png", gRenderer))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else
	{
		//Set sprites
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			gColor4SpriteClips[i].x = 0;
			gColor4SpriteClips[i].y = i * 50;
			gColor4SpriteClips[i].w = BUTTON_WIDTH;
			gColor4SpriteClips[i].h = BUTTON_HEIGHT;
		}
	}

	//Load sprites for Color 5
	if (!gColor5SpriteSheetTexture.loadFromFile("Images/color5sprite.png", gRenderer))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else
	{
		//Set sprites
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			gColor5SpriteClips[i].x = 0;
			gColor5SpriteClips[i].y = i * 50;
			gColor5SpriteClips[i].w = BUTTON_WIDTH;
			gColor5SpriteClips[i].h = BUTTON_HEIGHT;
		}
	}


	if (success) {
		for (int y = 0; y < 8; y++)
		{
			for (int x = 0; x < 8; x++)
			{
				gButtons[x + y * 8].setPosition(x * OFFSET_MULTIPLIER + OFFSET_X, y * OFFSET_MULTIPLIER + OFFSET_Y);
				//gButtons[x + y * 8].updateY = y * OFFSET_MULTIPLIER + OFFSET_Y;
				//gButtons[x + y * 8].setPosition(x * OFFSET_MULTIPLIER + OFFSET_X, OFFSET_Y - OFFSET_MULTIPLIER);
				//gButtons[x + y * 8].setToUpdate(true);
				//gButtons[x + y * 8].fallPriority = 7 - y;
			}
		}
	}

	return success;
}

void render()
{
	//Clear screen
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	//Render buttons
	for (int i = 0; i < TOTAL_BUTTONS; ++i)
	{
		gButtons[i].render(); //change to render with animation
	}

	//Update screen
	SDL_RenderPresent(gRenderer);
}

void close()
{
	//Free loaded images
	gColor1SpriteSheetTexture.free();
	gColor2SpriteSheetTexture.free();
	gColor3SpriteSheetTexture.free();
	gColor4SpriteSheetTexture.free();
	gColor5SpriteSheetTexture.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
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


			//plays the initial animation
			//render board

			while (game.checkSequence(gButtons))
			{
				game.dropDownSquares(gButtons);
				game.generateNewSquares(gButtons);
				//render board
			}

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
					for (int i = 0; i < TOTAL_BUTTONS; ++i)
					{
						gButtons[i].handleEvent(&e,gPressedButtons);
						if (pressedCount == 2)
						{
							game.swapSquares(gPressedButtons, gButtons);
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