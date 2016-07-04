#include "Game.h"

Animation anim;

Game::Game()
{
}


Game::~Game()
{
}

bool Game::isAdjacent(LButton *button0, LButton *button1)
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

void Game::swapSquares(LButton *gPressedButtons[2], LButton gButtons[TOTAL_BUTTONS])
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
		SDL_Point button0Position = gPressedButtons[0]->getPosition();
		SDL_Point button1Position = gPressedButtons[1]->getPosition();
		int button0type = gPressedButtons[0]->getType();
		int button1type = gPressedButtons[1]->getType();

		anim.swapSquaresAnim(gPressedButtons, gButtons);
		
		//swap the types
		gPressedButtons[0]->setType(button1type);
		gPressedButtons[1]->setType(button0type);
		//swap the positions
		gPressedButtons[0]->setPosition(button0Position.x, button0Position.y);
		gPressedButtons[1]->setPosition(button1Position.x, button1Position.y);

		if (!checkSequence(gButtons)) //if there's no sequence, revert the swap
		{
			button0Position = gPressedButtons[0]->getPosition();
			button1Position = gPressedButtons[1]->getPosition();
			button0type = gPressedButtons[0]->getType();
			button1type = gPressedButtons[1]->getType();

			anim.swapSquaresAnim(gPressedButtons, gButtons);

			//swap the types
			gPressedButtons[0]->setType(button1type);
			gPressedButtons[1]->setType(button0type);
			//swap the positions
			gPressedButtons[0]->setPosition(button0Position.x, button0Position.y);
			gPressedButtons[1]->setPosition(button1Position.x, button1Position.y);
		}
		else
		{
			//anim.sequenceRemoveAnim(gButtons);
			dropDownSquares(gButtons);
			//generateNewSquares(gButtons);
			/*while (checkSequence(gButtons))
			{
				dropDownSquares(gButtons);
				generateNewSquares(gButtons);
			}*/
		}
		gPressedButtons[0]->setPressed(false);
		gPressedButtons[1]->setPressed(false);
		pressedCount = 0;
	}
}

bool Game::checkSequence(LButton gButtons[TOTAL_BUTTONS])
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
			if (x == 0 && gButtons[x + y * 8].getType()!=-1)
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
						removeSequence(location, y, count, 0, gButtons);
						sequenceFound = true;
					}
					type = gButtons[x + y * 8].getType();
					count = 1;
				}
			}
			if (x == 7 && count > 2)
			{
				removeSequence(location, y, count, 0, gButtons);
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
				if (gButtons[i + j * 8].getType() == type && gButtons[i + j * 8].getType() != -1 )
				{
					count++;
					if (count > 2) {
						location = j;
					}
				}
				else
				{
					if (count > 2 && gButtons[i + j * 8].getType() != -1) {
						removeSequence(i, location, count, 1, gButtons);
						sequenceFound = true;
					}
					type = gButtons[i + j * 8].getType();
					count = 1;
				}
			}
			if (j == 7 && count > 2 && gButtons[i + j * 8].getType() != -1)
			{
				removeSequence(i, location, count, 1, gButtons);
				sequenceFound = true;
			}
		}
	}

	anim.sequenceRemoveAnim(gButtons);

	return sequenceFound;
}

//0 = horizontal; 1 = vertical
void Game::removeSequence(int x, int y, int lenght, int orientation, LButton gButtons[TOTAL_BUTTONS])
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

void Game::dropDownSquares(LButton gButtons[TOTAL_BUTTONS])
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
					gButtons[x + k * 8].updateDrop++;
				}
			}
		}
	}

	// Now that each square has its new position in their updateDrop property,
	// let's set the correct variables for the animation process
	for (int x = 0; x < 8; x++)
	{
		for (int y = 7; y >= 0; y--)
		{
			// If the square is not empty and has to fall
			if (gButtons[x + y * 8].toUpdate() == true && gButtons[x + y * 8].isRemoved() == false)
			{
				int y0 = gButtons[x + y * 8].updateDrop;

				//set the variables for the animation process
				gButtons[x + y * 8].origX = gButtons[x + y * 8].getPosition().x;
				gButtons[x + y * 8].origY = gButtons[x + y * 8].getPosition().y;
				gButtons[x + y * 8].destX = gButtons[x + y * 8].getPosition().x; //only moves in Y
				gButtons[x + y * 8].destY = (y + y0) * OFFSET_MULTIPLIER + OFFSET_Y;

			}
		}
	}

	bool animationDone = false;
	int tcurrent = SDL_GetTicks();
	//play the animations till they're all finished
	while (!animationDone)
	{
		animationDone = true;
		for (int x = 0; x < 8; x++)
		{
			for (int y = 7; y >= 0; y--)
			{
				// If the square is not empty and has to fall
				if (gButtons[x + y * 8].toUpdate() == true && gButtons[x + y * 8].isRemoved() == false)
				{
					//updates the animation and computes if all the animations are done
					animationDone = animationDone & anim.animate(tcurrent, 150 * gButtons[x + y * 8].updateDrop, &gButtons[x + y * 8]);
				}
			}
		}
		if (!animationDone)
		{
			anim.render(gButtons);
		}
	}
	
	//fix the positions and types
	for (int x = 0; x < 8; x++)
	{
		for (int y = 7; y >= 0; y--)
		{
			// If the square is not empty and has to fall
			if (gButtons[x + y * 8].toUpdate() == true && gButtons[x + y * 8].isRemoved() == false)
			{
				int y0 = gButtons[x + y * 8].updateDrop;
				////swap the piece with the respective empty space
				gButtons[x + (y + y0) * 8].setType(gButtons[x + y * 8].getType());
				gButtons[x + y * 8].setType(-1);
				gButtons[x + y * 8].setRemoved(true);
				gButtons[x + (y + y0) * 8].setRemoved(false);

				////fix the position
				gButtons[x + y * 8].setPosition(gButtons[x + y * 8].origX, gButtons[x + y * 8].origY);

				////revert variables to default values
				gButtons[x + y * 8].setToUpdate(false);
				gButtons[x + (y + y0) * 8].setToUpdate(false);
				gButtons[x + y * 8].updateDrop = 0;
				gButtons[x + (y + y0) * 8].updateDrop = 0;
			}
		}
	}

		// Finally, let's count how many new empty spaces there are so we can fill
		// them with new random gems
		//int emptySpaces = 0;

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

void Game::generateNewSquares(LButton gButtons[TOTAL_BUTTONS])
{
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (gButtons[x + y * 8].isRemoved())
			{
				gButtons[x + y * 8].setType(rand() % 5);
				gButtons[x + y * 8].setRemoved(false);
				gButtons[x + y * 8].setToUpdate(false);
				gButtons[x + y * 8].destY = y;
			}
		}
	}
}
