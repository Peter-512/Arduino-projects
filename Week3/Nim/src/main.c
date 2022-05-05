#include <avr/io.h>
#include <avr/interrupt.h>
#include <button-lib.h>
#include <display.h>
#include <stdlib.h>
#include <potentiometer.h>
#include <util/delay.h>
#include <led-lib.h>
#define START_NUMBER 21
#define MAX_NUMBER 3
#define true 1
#define false !true

// #define DEBUG
#ifdef DEBUG
#include <usart.h>
#endif

uint8_t firstButtonPush = false;
uint8_t matchesToTake = 1;
char players[] = "PC";
char *currentPlayer = players;
uint8_t matches = START_NUMBER;

void swapPlayers()
{
	currentPlayer++;
	if (!*currentPlayer)
	{
		currentPlayer -= 2;
	}
}

/**
 * @brief Construct a new ISR object
 *
 *
 */
ISR(PCINT1_vect)
{
	uint8_t button = whichButtonPushed();
	if (firstButtonPush)
	{
		switch (button)
		{
		case BUTTON1:
			matchesToTake--; // ? this is an ugly solution but im too tired
			if (matchesToTake == 0)
			{
				matchesToTake += MAX_NUMBER;
			}
			break;
		case BUTTON2:
			swapPlayers();
			matches -= matchesToTake;
			if (*currentPlayer == players[1]) // * if currentPlayer is C
			{
				matchesToTake = (matches - 1) % (MAX_NUMBER + 1);
			}
			else
			{
				matchesToTake = 1;
			}
			break;
		case BUTTON3:
			matchesToTake = matchesToTake % MAX_NUMBER + 1;
			break;
		default:
			break;
		}
	}
	if (button < NUMBER_OF_BUTTONS)
	{
		firstButtonPush = true;
	}
}

void setup()
{
	enableAllButtons();
	getButtonsReadyForInterrupts();
	enableAllLeds();
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
	initADC();
	uint16_t seed = getPotentiometerValue();
	srand(seed);
	writeNumberAndWait(seed, 2000);
}

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
	while (1)
	{
		writeLetterToSegment(0, *currentPlayer);
		writeLetterToSegment(1, 'W');
		writeLetterToSegment(2, 'O');
		writeLetterToSegment(3, 'N');
	}
	return 0;
}
