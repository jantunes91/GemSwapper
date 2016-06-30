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

int pressedCount;

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

//Swaps two squares
void swapSquares();

//Checks if there's a sequence and removes it from the board
bool checkSequence();

//Removes a previously found sequence, given it's end and it's lenght
void removeSequence(int x, int y, int lenght, int orientation);

void dropDownSquares();

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
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
			}
		}
	}

	return success;
}

bool isAdjacent(LButton *button0, LButton *button1)
{
	if (button1->getPosition().x == button0->getPosition().x + OFFSET_MULTIPLIER && button1->getPosition().y == button0->getPosition().y ||
		button1->getPosition().x == button0->getPosition().x - OFFSET_MULTIPLIER && button1->getPosition().y == button0->getPosition().y ||
		button1->getPosition().y == button0->getPosition().y + OFFSET_MULTIPLIER && button1->getPosition().x == button0->getPosition().x ||
		button1->getPosition().y == button0->getPosition().y - OFFSET_MULTIPLIER && button1->getPosition().x == button0->getPosition().x)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void swapSquares()
{
	if (!isAdjacent(gPressedButtons[0], gPressedButtons[1]))
	{
		gPressedButtons[0]->setPressed(false);
		gPressedButtons[0] = gPressedButtons[1];
		pressedCount = 1;
		return;
	}
	else
	{
		int tempType;
		tempType = gPressedButtons[0]->getType();
		gPressedButtons[0]->setType(gPressedButtons[1]->getType());
		gPressedButtons[1]->setType(tempType);
		if(!checkSequence()) //if there's no sequence, revert the swap
		{
			tempType = gPressedButtons[0]->getType();
			gPressedButtons[0]->setType(gPressedButtons[1]->getType());
			gPressedButtons[1]->setType(tempType);
		}
		else
		{
			dropDownSquares();
			/*while (checkSequence())
			{
				dropDownSquares();
				
			}*/
		}
		gPressedButtons[0]->setPressed(false);
		gPressedButtons[1]->setPressed(false);
		pressedCount = 0;
	}
}

bool checkSequence()
{
	bool sequenceFound = false;

	// Checks for duplicates in rows
	int count = 0;
	int location = -1;
	int type = -2;
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (x == 0 && gButtons[x + y * 8].getType() != -1)
			{
				type = gButtons[x + y * 8].getType();
				count = 1;
			}
			else
			{
				if (gButtons[x + y * 8].getType() == type && gButtons[x + y * 8].getType() != -1)
				{
					count++;
					if (count > 2) {
						location = x;
					}
				}
				else
				{
					if (count > 2 && gButtons[x + y * 8].getType() != -1) {
						removeSequence(location, y, count, 0);
						sequenceFound = true;
					}
					type = gButtons[x + y * 8].getType();
					count = 1;
				}
			}
			if (x == 7 && count > 2)
			{
				removeSequence(location, y, count, 0);
				sequenceFound = true;
			}
		}
	}

	// Checks for duplicates in columns
	count = 0;
	location = -1;
	type = -2;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (j == 0 && gButtons[i + j * 8].getType() != -1)
			{
				type = gButtons[i + j * 8].getType();
				count = 1;
			}
			else
			{
				if (gButtons[i + j * 8].getType() == type && gButtons[i + j * 8].getType() != -1)
				{
					count++;
					if (count > 2) {
						location = j;
					}
				}
				else
				{
					if (count > 2 && gButtons[i + j * 8].getType() != -1) {
						removeSequence(i, location, count, 1);
						sequenceFound = true;
					}
					type = gButtons[i + j * 8].getType();
					count = 1;
				}
			}
			if (j == 7 && count > 2)
			{
				removeSequence(i, location, count, 1);
				sequenceFound = true;
			}
		}
	}

	/*if (location != -1) {
		for (int remove = count; remove >= 0; remove--)
		{
			board[remove][square.y].destroy = true; // Removes the sequence
			sequenceFound = true;
		}
	}*/
	
	return sequenceFound;
}

//0 = horizontal; 1 = vertical
void removeSequence(int x, int y, int lenght, int orientation)
{
	if (orientation == 0)
	{
		for (int remove = x; remove > x - lenght; remove--)
		{
			gButtons[remove + y * 8].setRemoved(true);
		}
	}
	else
	{
		for (int remove = y; remove > y - lenght; remove--)
		{
			gButtons[x + remove * 8].setRemoved(true);
		}
	}

}

void dropDownSquares()
{
	for (int x = 0; x < 8; x++)
	{
		// We go from the bottom up
		for (int y = 7; y >= 0; y--)
		{
			// If the current square is empty, every square above it should fall one position
			if (gButtons[x + y * 8].isRemoved())
			{
				for (int k = y - 1; k >= 0; k--)
				{
					gButtons[x + k * 8].setToUpdate(true);
					gButtons[x + k * 8].updateY++;
				}
			}
		}

		// Now that each square has its new position in their updateY property,
		// let's move them to that final position

		for (int y = 7; y >= 0; y--)
		{
			bool testupdate = gButtons[x + y * 8].toUpdate();
			bool testremoved = gButtons[x + y * 8].isRemoved();
			// If the square is not empty and has to fall, move it to the new position
			if (gButtons[x + y * 8].toUpdate() == true && gButtons[x + y * 8].isRemoved() == false)
			{
				int y0 = gButtons[x + y * 8].updateY;

				//swap the piece with the respective empty space
				gButtons[x + (y+y0) * 8].setType(gButtons[x + y * 8].getType());
				gButtons[x + y * 8].setType(-1);
				gButtons[x + y * 8].setRemoved(true);
				gButtons[x + (y + y0) * 8].setRemoved(false);

				//revert variables to default values
				gButtons[x + y * 8].setToUpdate(false);
				gButtons[x + (y + y0) * 8].setToUpdate(false);
				gButtons[x + y * 8].updateY = 0;
				gButtons[x + (y + y0) * 8].updateY = 0;
			}
		}

		// Finally, let's count how many new empty spaces there are so we can fill
		// them with new random gems
		int emptySpaces = 0;

		/*// We start counting from top to bottom. Once we find a square, we stop counting
		for (int y = 0; y < 8; ++y)
		{
			if (!board[x][y].destroy)
				break;

			emptySpaces++;
		}
		
		// Again from top to bottom, fill the emtpy squares, assigning them a
		// proper position outta screen for the animation to work
		for (int y = 0; y < 8; y++)
		{
			if (gButtons[x + y * 8].isRemoved())
			{
				gButtons[x + y * 8].setType(rand() % 5);
				gButtons[x + y * 8].setRemoved(false);
				gButtons[x + y * 8].updatePosition = false;
				gButtons[x + y * 8].updateY = y;
				/*board[x][y].x = x;
				board[x][y].y = y;
				board[x][y].updateY = y;
				board[x][y].updatePosition = false;
				board[x][y].destroy = false;
			}
		}*/
	}
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

			while (checkSequence())
			{
				dropDownSquares();
				//generate new gems
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
						gButtons[i].handleEvent(&e,gPressedButtons,pressedCount);
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render buttons
				for (int i = 0; i < TOTAL_BUTTONS; ++i)
				{
					gButtons[i].render();
				}

				//Render buttons
				/*for (int i = 0; i < TOTAL_BUTTONS; ++i)
				{
					if (gButtons[i].isRemoved())
					{
						gButtons[i].setType(-1); //will draw nothing (empty space)
					}

					switch (gButtons[i].getType())
					{
					case 0:
						gButtons[i].render(gColor1SpriteSheetTexture, gColor1SpriteClips, gRenderer);
						break;
					case 1:
						gButtons[i].render(gColor2SpriteSheetTexture, gColor2SpriteClips, gRenderer);
						break;
					case 2:
						gButtons[i].render(gColor3SpriteSheetTexture, gColor3SpriteClips, gRenderer);
						break;
					case 3:
						gButtons[i].render(gColor4SpriteSheetTexture, gColor4SpriteClips, gRenderer);
						break;
					case 4:
						gButtons[i].render(gColor5SpriteSheetTexture, gColor5SpriteClips, gRenderer);
						break;
					default:
						break;
					}
				}*/

				//Update screen
				SDL_RenderPresent(gRenderer);
				//OutputDebugString(TEXT("screen updated\n"));
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}