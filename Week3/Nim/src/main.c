#include <avr/io.h>
#include <avr/interrupt.h>
#include <button-lib.h>
#include <display.h>
#include <stdlib.h>
#include <potentiometer.h>
#include <util/delay.h>
#include <led-lib.h>
#include <usart.h>
#define START_NUMBER 21
#define MAX_NUMBER 3
#define true 1
#define false !true

uint8_t button;
uint8_t firstButtonPush = true;
uint8_t matchesToTake = 1;
char players[] = "PC";
char *currentPlayer = players;
int matches;
int maxNumber;
uint16_t seed;

/**
 * @brief Returns if the user is the current active player
 *
 * @return uint8_t 0 or 1
 */
uint8_t isPlayerCurrentPlayer()
{
	return *currentPlayer == players[0];
}

/**
 * @brief Increments the currentPlayer pointer and moves it back if it reaches the \0 char
 *
 */
void swapPlayers()
{
	currentPlayer++;
	if (!*currentPlayer)
	{
		currentPlayer -= 2;
	}
}

void saveTurn()
{
	// TODO figure out how to model the data im saving and how to allocate the memory properly
}

/**
 * @brief Does one of the actions based on the button that was pushed.
 *
 * BUTTON1: decrement matchesToTake
 * BUTTON2: finish turn
 * BUTTON3: increment matchesToTake
 *
 */
void doAction()
{
	switch (button)
	{
	case BUTTON1:
		// TODO maybe extra logic here and on BUTTON3 so you cant take more than sticks are left? sounds tricky though
		if (isPlayerCurrentPlayer())
		{
			matchesToTake = --matchesToTake ? matchesToTake : maxNumber;
		}
		break;
	case BUTTON2:
		saveTurn();
		swapPlayers();
		matches -= matchesToTake;
		if (!isPlayerCurrentPlayer())
		{
			matchesToTake = (matches - 1) % (maxNumber + 1);
			matchesToTake = matchesToTake ? matchesToTake : rand() % maxNumber + 1;
		}
		else
		{
			matchesToTake = 1;
		}
		break;
	case BUTTON3:
		if (isPlayerCurrentPlayer())
		{
			matchesToTake = matchesToTake % maxNumber + 1;
		}
		break;
	default:
		break;
	}
}

/**
 * @brief Construct a new ISR object
 *
 */
ISR(PCINT1_vect)
{
	button = whichButtonPushed();
	if (!firstButtonPush)
	{
		doAction();
	}
	else
	{
		firstButtonPush = false;
		seed = getPotentiometerValue();
		srand(seed);
		currentPlayer += rand() % 2; // * starting player gets chosen randomly by default
		if (button == BUTTON3)		 // * BUTTON3 activates random mode
		{
			matches = rand() % 79 + 21; // * 21 - 99
			maxNumber = rand() % 7 + 3; // *  3 -  9
		}
		else
		{
			matches = START_NUMBER;
			maxNumber = MAX_NUMBER;
		}
		if (!isPlayerCurrentPlayer())
		{
			matchesToTake = (matches - 1) % (maxNumber + 1);
		}
		matchesToTake = matchesToTake ? matchesToTake : rand() % maxNumber + 1;
	}
}

/**
 * @brief Initializes all neccessary things, waits for first button push and displays the seed on the display
 *
 */
void setup()
{
	initUSART();
	enableAllButtons();
	getButtonsReadyForInterrupts();
	enableAllLeds();
	initADC();
	while (!firstButtonPush)
	{
		for (int j = 0; j < 4; j++)
		{
			lightToggleOneLed(j);
			_delay_ms(100);
		}
	}
	lightDownAllLeds();
	initDisplay();
	writeNumberAndWait(seed, 2000);
}

/**
 * @brief Shows current game state on display
 *
 */
void displayScreen()
{
	writeNumberToSegment(0, matchesToTake);
	writeLetterToSegment(1, *currentPlayer);
	writeNumberToSegment(2, (matches / 10) % 10);
	writeNumberToSegment(3, matches % 10);
}

int main(int argc, char const *argv[])
{
	setup();

	while (matches > 0)
	{
		displayScreen();
	}

	cli();

	if (isPlayerCurrentPlayer())
	{
		writeLongWord("congratulations you won   ", 24);
		// ! this is a bit hacky, as the extra spaces are there so the pointer i move inside the method doesnt move into the next memory location
	}
	else
	{
		writeLongWord("you suck try again", 19);
	}
	return 0;
}
