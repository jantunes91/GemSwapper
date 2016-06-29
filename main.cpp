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

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

//Button constants
const int BUTTON_WIDTH = 50;
const int BUTTON_HEIGHT = 50;
const int TOTAL_BUTTONS = 64;
const int OFFSET_MULTIPLIER = 65;
const int OFFSET_X = 400;
const int OFFSET_Y = 100;
const int MAX_SELECTED = 2;

int pressedCount;

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	//BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 3
};

//Texture wrapper class
class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

//The mouse button
class LButton
{
public:
	//Initializes internal variables
	LButton();

	//Sets top left position
	void setPosition(int x, int y);

	SDL_Point getPosition();

	void setPressed(bool isPressed);

	int getType();

	void setType(int tempType);

	void setRemoved(bool isRemoved);

	bool isRemoved();

	void setToUpdate(bool toUpdate);

	bool toUpdate();

	int updateY;

	//Handles mouse event
	void handleEvent(SDL_Event* e);

	//Shows button sprite
	void render();

private:
	//Top left position
	SDL_Point mPosition;

	//Currently used global sprite
	LButtonSprite mCurrentSprite;

	bool pressed;

	int type;

	bool removed;

	bool updatePosition;
};

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
SDL_Renderer* gRenderer = NULL;

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

//Buttons objects
LButton gButtons[TOTAL_BUTTONS];
//Pressed Buttons
LButton *gPressedButtons[2];

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

LButton::LButton()
{
	mPosition.x = OFFSET_X;
	mPosition.y = OFFSET_Y;

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
	
	type = rand() % 5;

	removed = false;

	updatePosition = false;
	updateY = 0;
}

void LButton::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

SDL_Point LButton::getPosition()
{
	return mPosition;
}

void LButton::setPressed(bool isPressed)
{
	pressed = isPressed;
}

int LButton::getType()
{
	return type;
}

void LButton::setType(int tempType)
{
	type = tempType;
}

void LButton::setRemoved(bool isRemoved)
{
	removed = isRemoved;
}

bool LButton::isRemoved()
{
	return removed;
}

void LButton::setToUpdate(bool toUpdate)
{
	updatePosition = toUpdate;
}

bool LButton::toUpdate()
{
	return updatePosition;
}

void LButton::handleEvent(SDL_Event* e)
{
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if (x < mPosition.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > mPosition.x + BUTTON_WIDTH)
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < mPosition.y)
		{
			inside = false;
		}
		//Mouse below the button
		else if (y > mPosition.y + BUTTON_HEIGHT)
		{
			inside = false;
		}

		//Mouse is outside button
		if (!inside && !pressed)
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				if (!pressed)
				{
					mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				setPressed(true);
				gPressedButtons[pressedCount] = this;				
				pressedCount++;
				if (pressedCount == 2)
				{
					swapSquares();
					//pressedCount = 0;
					if (!gPressedButtons[1]->pressed && !gPressedButtons[0]->pressed) {
						OutputDebugString(TEXT("pressed a falso\n"));
					}
				}
				break;

			/*case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				break;*/
			}
		}
	}
}

void LButton::render()
{
	if (removed)
	{
		type = -1;
	}
	//Show current button sprite
	if (type == 0)
	{
		gColor1SpriteSheetTexture.render(mPosition.x, mPosition.y, &gColor1SpriteClips[mCurrentSprite]);
	}

	if (type == 1)
	{
		gColor2SpriteSheetTexture.render(mPosition.x, mPosition.y, &gColor2SpriteClips[mCurrentSprite]);
	}

	if (type == 2)
	{
		gColor3SpriteSheetTexture.render(mPosition.x, mPosition.y, &gColor3SpriteClips[mCurrentSprite]);
	}

	if (type == 3)
	{
		gColor4SpriteSheetTexture.render(mPosition.x, mPosition.y, &gColor4SpriteClips[mCurrentSprite]);
	}

	if (type == 4)
	{
		gColor5SpriteSheetTexture.render(mPosition.x, mPosition.y, &gColor5SpriteClips[mCurrentSprite]);
	}
}

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
	if (!gColor1SpriteSheetTexture.loadFromFile("Images/color1sprite.png"))
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
	if (!gColor2SpriteSheetTexture.loadFromFile("Images/color2sprite.png"))
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
	if (!gColor3SpriteSheetTexture.loadFromFile("Images/color3sprite.png"))
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
	if (!gColor4SpriteSheetTexture.loadFromFile("Images/color4sprite.png"))
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
	if (!gColor5SpriteSheetTexture.loadFromFile("Images/color5sprite.png"))
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
				gButtons[x + y * 8].setType(rand() % 5);
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
			bool test = true;
			//while (test)
			//{
				dropDownSquares();
				//checkSequence();
			//}
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
					if (count > 2) {
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

				gButtons[x + (y+y0) * 8].setType(gButtons[x + y * 8].getType());
				gButtons[x + y * 8].setType(-1);
				gButtons[x + y * 8].setRemoved(true);
				gButtons[x + (y + y0) * 8].setRemoved(false);
				gButtons[x + y * 8].setToUpdate(false);
				gButtons[x + (y + y0) * 8].setToUpdate(false);
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
						gButtons[i].handleEvent(&e);
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