#include "Game.h"

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
		//button1 = NULL;
		pressedCount = 1;
		return;
	}
	else
	{
		int tempType;
		tempType = gPressedButtons[0]->getType();
		gPressedButtons[0]->setType(gPressedButtons[1]->getType());
		gPressedButtons[1]->setType(tempType);
		if (!checkSequence(gButtons)) //if there's no sequence, revert the swap
		{
			tempType = gPressedButtons[0]->getType();
			gPressedButtons[0]->setType(gPressedButtons[1]->getType());
			gPressedButtons[1]->setType(tempType);
		}
		else
		{
			dropDownSquares(gButtons);
			generateNewSquares(gButtons);
			while (checkSequence(gButtons))
			{
				dropDownSquares(gButtons);
				generateNewSquares(gButtons);
			}
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
						removeSequence(i, location, count, 1, gButtons);
						sequenceFound = true;
					}
					type = gButtons[i + j * 8].getType();
					count = 1;
				}
			}
			if (j == 7 && count > 2)
			{
				removeSequence(i, location, count, 1, gButtons);
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
				gButtons[x + (y + y0) * 8].setType(gButtons[x + y * 8].getType());
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
				gButtons[x + y * 8].updateY = y;
			}
		}
	}
}
