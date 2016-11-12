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
		gem1->getPosition().y == gem0->getPosition().y - OFFSET_MULTIPLIER && gem1->getPosition().x == gem0->getPosition().x) {
		return true;
	}
	else {
		return false;
	}
}

void Board::swapGems(Gem *pressedGems[2], Gem gems[TOTAL_GEMS], Window *window)
{
	//if the selected gems are not adjacent, deselect the first one and only select the second
	if (!isAdjacent(pressedGems[0], pressedGems[1])) {
		pressedGems[0]->setPressed(false);
		pressedGems[0] = pressedGems[1];
		pressedCount = 1;
		return;
	}
	else {
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

		if (!checkSequence(gems, window, false)) { //if there's no sequence, revert the swap
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
		else {
			dropDownGems(gems, window);
			//we want to make sure we handle all the sequences that may result from the swap
			while (checkSequence(gems, window, false)) {
				dropDownGems(gems, window);
			}
		}
		//after the swap is done, revert to default values
		multiplier = 1;
		pressedGems[0]->setPressed(false);
		pressedGems[1]->setPressed(false);
		pressedCount = 0;
	}
}

//the justCheck flag tells if the sequence is to be removed or simply checked for existence
bool Board::checkSequence(Gem gems[TOTAL_GEMS], Window *window, bool justCheck)
{
	bool sequenceFound = false;

	// Checks for duplicates in rows
	int count = 0;
	int location = -1;
	int type = -2;
	for (int y = 0; y < COLUMN_SIZE; y++) {
		for (int x = 0; x < LINE_SIZE; x++) {

			if (x == 0) {
				type = gems[x + y * COLUMN_SIZE].getType();
				count = 1;
			}
			else {
				if (gems[x + y * COLUMN_SIZE].getType() == type ) {
					count++;
					if (count >= MIN_SEQUENCE_SIZE) { location = x; }
				}
				else {
					if (count >= MIN_SEQUENCE_SIZE) {
						if (!justCheck)	{
							removeSequence(location, y, count, 0, gems);
							calculateScore(count);
						}
						sequenceFound = true;
					}
					type = gems[x + y * COLUMN_SIZE].getType();
					count = 1;
				}
			}
			if (x == LINE_SIZE - 1 && count >= MIN_SEQUENCE_SIZE) {
				if (!justCheck) {
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
	for (int i = 0; i < LINE_SIZE; i++) {
		for (int j = 0; j < COLUMN_SIZE; j++) {

			if (j == 0)	{
				type = gems[i + j * COLUMN_SIZE].getType();
				count = 1;
			}
			else {
				if (gems[i + j * COLUMN_SIZE].getType() == type) {
					count++;
					if (count >= MIN_SEQUENCE_SIZE) {
						location = j;
					}
				}
				else {
					if (count >= MIN_SEQUENCE_SIZE) {
						if (!justCheck) {
							removeSequence(i, location, count, 1, gems);
							calculateScore(count);
						}
						sequenceFound = true;
					}
					type = gems[i + j * COLUMN_SIZE].getType();
					count = 1;
				}
			}
			if (j == COLUMN_SIZE - 1 && count >= MIN_SEQUENCE_SIZE) {
				if (!justCheck) {
					removeSequence(i, location, count, 1, gems);
					calculateScore(count);
				}
				sequenceFound = true;
			}
		}
	}

	if (sequenceFound && !justCheck) { anim.sequenceRemoveAnim(gems, window); }

	return sequenceFound;
}


void Board::removeSequence(int x, int y, int lenght, int orientation, Gem gems[TOTAL_GEMS])
{
	if (orientation == HORIZONTAL) {
		for (int remove = x; remove > x - lenght; remove--) {
			gems[remove + y * COLUMN_SIZE].setRemoved(true);
		}
	}
	else {
		for (int remove = y; remove > y - lenght; remove--) {
			gems[x + remove * COLUMN_SIZE].setRemoved(true);
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
	for (int x = 0; x < LINE_SIZE; x++) {
		// We go from the bottom up
		for (int y = COLUMN_SIZE - 1; y >= 0; y--) {
			// If the current gem is empty, every gem above it should fall one position
			if (gems[x + y * COLUMN_SIZE].isRemoved()) {
				emptySpacesPerColumn[x]++;
				for (int k = y - 1; k >= 0; k--) {
					gems[x + k * COLUMN_SIZE].setToUpdate(true);
					gems[x + k * COLUMN_SIZE].updateDrop++;
					int updatedrop22 = gems[2 + 2 * 8].updateDrop;
				}
			}
		}
	}

	// Now that each gem has its new position in their updateDrop property,
	// let's set the correct variables for the animation process
	for (int x = 0; x < LINE_SIZE; x++) {
		for (int y = 7; y >= 0; y--) {
			int newGemOffset = -1;
			// If the gem is not empty and has to fall
			if (gems[x + y * COLUMN_SIZE].toUpdate() == true && gems[x + y * COLUMN_SIZE].isRemoved() == false) {
				int y0 = gems[x + y * COLUMN_SIZE].updateDrop;

				//set the variables for the animation process on the destiny position
				gems[x + (y + y0) * 8].origX = x * OFFSET_MULTIPLIER + OFFSET_X;
				gems[x + (y + y0) * 8].origY = y * OFFSET_MULTIPLIER + OFFSET_Y;

				////swap the piece with the respective empty space
				gems[x + (y + y0) * 8].setType(gems[x + y * COLUMN_SIZE].getType());
				gems[x + y * COLUMN_SIZE].setType(-1);
				gems[x + (y + y0) * 8].setRemoved(false);
				gems[x + (y + y0) * 8].setToUpdate(true);
				gems[x + y * COLUMN_SIZE].setRemoved(true);

			}
		}
	}

	for (int x = 0; x < LINE_SIZE; x++) {
		for (int y = COLUMN_SIZE - 1; y >= 0; y--) {
			int newGemOffset = -1;
			// If the gem is removed
			if (gems[x + y * COLUMN_SIZE].isRemoved()) {
				//set the variables for the animation process on the origin position for a new piece to fall
				gems[x + y * COLUMN_SIZE].origX = x * OFFSET_MULTIPLIER + OFFSET_X;
				gems[x + y * COLUMN_SIZE].origY = newGemOffset * OFFSET_MULTIPLIER + OFFSET_Y;
				gems[x + y * COLUMN_SIZE].updateDrop = -newGemOffset;
				newGemOffset--;
			}
		}
	}

	//Generate new gems and give them the correct position for the animation to work
	generateNewGems(gems);

	//Play the animation
	anim.dropGemsAnim(gems, window);
	
	//fix the positions and types
	for (int x = 0; x < LINE_SIZE; x++) {
		for (int y = COLUMN_SIZE - 1; y >= 0; y--) {
			// If the gem is not empty and has to fall
			if (gems[x + y * COLUMN_SIZE].toUpdate() == true && gems[x + y * COLUMN_SIZE].isRemoved() == false) {

				////fix the position
				gems[x + y * COLUMN_SIZE].setPosition(x * OFFSET_MULTIPLIER + OFFSET_X, y * OFFSET_MULTIPLIER + OFFSET_Y);

				////revert variables to default values
				gems[x + y * COLUMN_SIZE].setToUpdate(false);
				gems[x + y * COLUMN_SIZE].updateDrop = 0;
			}
		}
	}
}

void Board::generateNewGems(Gem gems[TOTAL_GEMS])
{
	for (int x = 0; x < LINE_SIZE; x++)
	{
		for (int y = COLUMN_SIZE - 1; y >= 0; y--)
		{
			if (gems[x + y * COLUMN_SIZE].isRemoved())
			{
				gems[x + y * COLUMN_SIZE].setType(rand() % 5);
				gems[x + y * COLUMN_SIZE].setRemoved(false);
				gems[x + y * COLUMN_SIZE].setToUpdate(true);
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
	for (int x = 0; x < LINE_SIZE; x++)
	{
		for (int y = 0; y < COLUMN_SIZE; y++)
		{
			tempGems[x + y * COLUMN_SIZE] = gems[x + y * COLUMN_SIZE];
		}
	}

	int tempType;

	//Test horizontal swaps
	for (int x = 0; x < LINE_SIZE - 1; x++)
	{
		for (int y = 0; y < COLUMN_SIZE; y++)
		{
			//Make a copy of the gems board
			for (int x = 0; x < LINE_SIZE; x++)
			{
				for (int y = 0; y < COLUMN_SIZE; y++)
				{
					tempGems[x + y * COLUMN_SIZE] = gems[x + y * COLUMN_SIZE];
				}
			}

			//Make swap
			tempType = tempGems[x + y * COLUMN_SIZE].getType();
			tempGems[x + y * COLUMN_SIZE].setType(tempGems[(x + 1) + y * COLUMN_SIZE].getType());
			tempGems[(x + 1) + y * COLUMN_SIZE].setType(tempType);

			//Check if there's a sequence
			result = checkSequence(tempGems, window, true);
			if (result)
			{
				return result;
			}
		}
	}

	//Test vertical swaps
	for (int x = 0; x < LINE_SIZE; x++)
	{
		for (int y = 0; y < COLUMN_SIZE - 1; y++)
		{
			//Make a copy of the gems board
			for (int x = 0; x < LINE_SIZE; x++)
			{
				for (int y = 0; y < COLUMN_SIZE; y++)
				{
					tempGems[x + y * COLUMN_SIZE] = gems[x + y * COLUMN_SIZE];
				}
			}

			//Make swap
			tempType = tempGems[x + y * COLUMN_SIZE].getType();
			tempGems[x + y * COLUMN_SIZE].setType(tempGems[x + (y + 1) * COLUMN_SIZE].getType());
			tempGems[x + ( y + 1) * COLUMN_SIZE].setType(tempType);

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
