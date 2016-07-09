#include "Board.h"
#include "Window.h"

Animation anim;

Board::Board()
{
}


Board::~Board()
{
}

bool Board::isAdjacent(Gem *gem0, Gem *gem1)
{
	if (gem1->getPosition().x == gem0->getPosition().x + OFFSET_MULTIPLIER && gem1->getPosition().y == gem0->getPosition().y ||
		gem1->getPosition().x == gem0->getPosition().x - OFFSET_MULTIPLIER && gem1->getPosition().y == gem0->getPosition().y ||
		gem1->getPosition().y == gem0->getPosition().y + OFFSET_MULTIPLIER && gem1->getPosition().x == gem0->getPosition().x ||
		gem1->getPosition().y == gem0->getPosition().y - OFFSET_MULTIPLIER && gem1->getPosition().x == gem0->getPosition().x)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Board::swapGems(Gem *pressedGems[2], Gem gems[TOTAL_GEMS], Window *window)
{
	if (!isAdjacent(pressedGems[0], pressedGems[1]))
	{
		pressedGems[0]->setPressed(false);
		pressedGems[0] = pressedGems[1];
		pressedCount = 1;
		return;
	}
	else
	{
		SDL_Point gem0Position = pressedGems[0]->getPosition();
		SDL_Point gem1Position = pressedGems[1]->getPosition();
		int gem0type = pressedGems[0]->getType();
		int gem1type = pressedGems[1]->getType();
		
		//swap the types
		pressedGems[0]->setType(gem1type);
		pressedGems[1]->setType(gem0type);

		//animate the swap
		anim.swapGemsAnim(pressedGems, gems, window);

		//swap the positions
		pressedGems[0]->setPosition(gem0Position.x, gem0Position.y);
		pressedGems[1]->setPosition(gem1Position.x, gem1Position.y);

		pressedGems[0]->unselectTexture();
		pressedGems[1]->unselectTexture();

		if (!checkSequence(gems, window, false)) //if there's no sequence, revert the swap
		{
			gem0Position = pressedGems[0]->getPosition();
			gem1Position = pressedGems[1]->getPosition();
			gem0type = pressedGems[0]->getType();
			gem1type = pressedGems[1]->getType();

			//swap the types
			pressedGems[0]->setType(gem1type);
			pressedGems[1]->setType(gem0type);

			//animate the swap
			anim.swapGemsAnim(pressedGems, gems, window);

			//swap the positions
			pressedGems[0]->setPosition(gem0Position.x, gem0Position.y);
			pressedGems[1]->setPosition(gem1Position.x, gem1Position.y);
		}
		else
		{
			dropDownGems(gems, window);
			while (checkSequence(gems, window, false))
			{
				dropDownGems(gems, window);
			}
		}
		multiplier = 1;
		pressedGems[0]->setPressed(false);
		pressedGems[1]->setPressed(false);
		pressedCount = 0;
	}
}

bool Board::checkSequence(Gem gems[TOTAL_GEMS], Window *window, bool simple)
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
			if (x == 0 && gems[x + y * 8].getType()!=-1)
			{

				type = gems[x + y * 8].getType();
				count = 1;
			}
			else
			{
				if (gems[x + y * 8].getType() == type && gems[x + y * 8].getType() != -1)
				{
					count++;
					if (count > 2) {
						location = x;
					}
				}
				else
				{
					if (count > 2 && gems[x + y * 8].getType() != -1) {
						if (!simple)
						{
							removeSequence(location, y, count, 0, gems);
							calculateScore(count);
						}
						sequenceFound = true;
					}
					type = gems[x + y * 8].getType();
					count = 1;
				}
			}
			if (x == 7 && count > 2)
			{
				if (!simple)
				{
					removeSequence(location, y, count, 0, gems);
					calculateScore(count);
				}
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
			if (j == 0 && gems[i + j * 8].getType() != -1)
			{
				type = gems[i + j * 8].getType();
				count = 1;
			}
			else
			{
				if (gems[i + j * 8].getType() == type && gems[i + j * 8].getType() != -1 )
				{
					count++;
					if (count > 2) {
						location = j;
					}
				}
				else
				{
					if (count > 2 && gems[i + j * 8].getType() != -1) {
						if (!simple)
						{
							removeSequence(i, location, count, 1, gems);
							calculateScore(count);
						}
						sequenceFound = true;
					}
					type = gems[i + j * 8].getType();
					count = 1;
				}
			}
			if (j == 7 && count > 2 && gems[i + j * 8].getType() != -1)
			{
				if (!simple)
				{
					removeSequence(i, location, count, 1, gems);
					calculateScore(count);
				}
				sequenceFound = true;
			}
		}
	}

	if (sequenceFound && !simple)
	{
		anim.sequenceRemoveAnim(gems, window);
	}

	return sequenceFound;
}

//0 = horizontal; 1 = vertical
void Board::removeSequence(int x, int y, int lenght, int orientation, Gem gems[TOTAL_GEMS])
{
	if (orientation == 0)
	{
		for (int remove = x; remove > x - lenght; remove--)
		{
			gems[remove + y * 8].setRemoved(true);
		}
	}
	else
	{
		for (int remove = y; remove > y - lenght; remove--)
		{
			gems[x + remove * 8].setRemoved(true);
		}
	}

	//play the correct sound
	switch (multiplier)
	{
	case 1:
		Mix_PlayChannel(-1, sequence1, 0);
		break;
	case 2:
		Mix_PlayChannel(-1, sequence2, 0);
		break;
	default:
		Mix_PlayChannel(-1, sequence3, 0);
		break;
	}
}

void Board::calculateScore(int lenght)
{
	switch (lenght)
	{
	case 3:
		score = score + 50 * multiplier;
		break;
	case 4:
		score = score + 150 * multiplier;
		break;
	case 5:
		score = score + 200 * multiplier;
		break;
	case 6:
		score = score + 250 * multiplier;
		break;
	case 7:
		score = score + 300 * multiplier;
		break;
	case 8:
		score = score + 500 * multiplier;
		break;
	default:
		break;
	}

	multiplier++;
}

void Board::dropDownGems(Gem gems[TOTAL_GEMS], Window *window)
{
	int emptySpacesPerColumn[8] = { 0 };
	for (int x = 0; x < 8; x++)
	{
		// We go from the bottom up
		for (int y = 7; y >= 0; y--)
		{
			// If the current gem is empty, every gem above it should fall one position
			if (gems[x + y * 8].isRemoved())
			{
				emptySpacesPerColumn[x]++;
				for (int k = y - 1; k >= 0; k--)
				{
					gems[x + k * 8].setToUpdate(true);
					gems[x + k * 8].updateDrop++;
					int updatedrop22 = gems[2 + 2 * 8].updateDrop;
				}
			}
		}
	}

	// Now that each gem has its new position in their updateDrop property,
	// let's set the correct variables for the animation process
	for (int x = 0; x < 8; x++)
	{
		for (int y = 7; y >= 0; y--)
		{
			int newGemOffset = -1;
			// If the gem is not empty and has to fall
			if (gems[x + y * 8].toUpdate() == true && gems[x + y * 8].isRemoved() == false)
			{
				int y0 = gems[x + y * 8].updateDrop;

				//set the variables for the animation process on the destiny position
				gems[x + (y + y0) * 8].origX = x * OFFSET_MULTIPLIER + OFFSET_X;
				gems[x + (y + y0) * 8].origY = y * OFFSET_MULTIPLIER + OFFSET_Y;

				////swap the piece with the respective empty space
				gems[x + (y + y0) * 8].setType(gems[x + y * 8].getType());
				gems[x + y * 8].setType(-1);
				gems[x + (y + y0) * 8].setRemoved(false);
				gems[x + (y + y0) * 8].setToUpdate(true);
				gems[x + y * 8].setRemoved(true);

			}
		}
	}

	for (int x = 0; x < 8; x++)
	{
		for (int y = 7; y >= 0; y--)
		{
			int newGemOffset = -1;
			// If the gem is removed
			if (gems[x + y * 8].isRemoved())
			{
				//set the variables for the animation process on the origin position for a new piece to fall
				gems[x + y * 8].origX = x * OFFSET_MULTIPLIER + OFFSET_X;
				gems[x + y * 8].origY = newGemOffset * OFFSET_MULTIPLIER + OFFSET_Y;
				gems[x + y * 8].updateDrop = -newGemOffset;
				newGemOffset--;
			}
		}
	}

	//Generate new gems and give them the correct position for the animation to work
	generateNewGems(gems);

	//Play the animation
	anim.dropGemsAnim(gems, window);
	
	//fix the positions and types
	for (int x = 0; x < 8; x++)
	{
		for (int y = 7; y >= 0; y--)
		{
			// If the gem is not empty and has to fall
			if (gems[x + y * 8].toUpdate() == true && gems[x + y * 8].isRemoved() == false)
			{

				////fix the position
				gems[x + y * 8].setPosition(x * OFFSET_MULTIPLIER + OFFSET_X, y * OFFSET_MULTIPLIER + OFFSET_Y);

				////revert variables to default values
				gems[x + y * 8].setToUpdate(false);
				gems[x + y * 8].updateDrop = 0;
			}
		}
	}
}

void Board::generateNewGems(Gem gems[TOTAL_GEMS])
{
	for (int x = 0; x < 8; x++)
	{
		for (int y = 7; y >= 0; y--)
		{
			if (gems[x + y * 8].isRemoved())
			{
				gems[x + y * 8].setType(rand() % 5);
				gems[x + y * 8].setRemoved(false);
				gems[x + y * 8].setToUpdate(true);
			}
		}
	}
}

bool Board::checkAvailableMoves(Gem gems[TOTAL_GEMS], Window *window)
{
	bool result = false;

	// Checks for available moves in columns
	int count = 0;
	int type = -2;
	Gem tempGems[TOTAL_GEMS];

	//Make a copy of the gems board
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			tempGems[x + y * 8] = gems[x + y * 8];
		}
	}

	int tempType;

	//Test horizontal swaps
	for (int x = 0; x < 7; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			//Make a copy of the gems board
			for (int x = 0; x < 8; x++)
			{
				for (int y = 0; y < 8; y++)
				{
					tempGems[x + y * 8] = gems[x + y * 8];
				}
			}

			//Make swap
			tempType = tempGems[x + y * 8].getType();
			tempGems[x + y * 8].setType(tempGems[(x + 1) + y * 8].getType());
			tempGems[(x + 1) + y * 8].setType(tempType);

			//Check if there's a sequence
			result = checkSequence(tempGems, window, true);
			if (result)
			{
				return result;
			}
		}
	}

	//Test vertical swaps
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 7; y++)
		{
			//Make a copy of the gems board
			for (int x = 0; x < 8; x++)
			{
				for (int y = 0; y < 8; y++)
				{
					tempGems[x + y * 8] = gems[x + y * 8];
				}
			}

			//Make swap
			tempType = tempGems[x + y * 8].getType();
			tempGems[x + y * 8].setType(tempGems[x + (y + 1) * 8].getType());
			tempGems[x + ( y + 1) * 8].setType(tempType);

			//Check if there's a sequence
			result = checkSequence(tempGems, window, true);
			if (result)
			{
				return result;
			}
		}
	}
	return result;
}
