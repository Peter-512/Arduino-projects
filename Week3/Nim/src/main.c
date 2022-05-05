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

#define DEBUG
#ifdef DEBUG
#include <usart.h>
#endif

uint8_t button; // ? Maybe this could be a local variable of the ISR
uint8_t btnPushed = false;
uint8_t firstButtonPush = true;
uint8_t matchesToTake = 1;
char players[] = "PC";
char *currentPlayer = players;

/**
 * @brief Construct a new ISR object
 *
 *
 */
ISR(PCINT1_vect)
{
	button = whichButtonPushed();
	if (!firstButtonPush)
	{
		switch (button)
		{
		case BUTTON1:
			matchesToTake = (matchesToTake + 1) % MAX_NUMBER + 1;
			break;
		case BUTTON2:
			currentPlayer++;
			if (!*currentPlayer)
			{
				currentPlayer -= 2;
			}
			break;
		case BUTTON3:
			matchesToTake = (matchesToTake - 1 + MAX_NUMBER) % MAX_NUMBER + 1;
			break;

		default:
			break;
		}
	}
	if (button < NUMBER_OF_BUTTONS)
	{
		btnPushed = true;
		firstButtonPush = false;
	}
}

void setup()
{
	enableAllButtons();
	getButtonsReadyForInterrupts();
	enableAllLeds();
	while (!btnPushed)
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
	writeNumberAndWait(seed, 5000);
	btnPushed = false;
}

void displayScreen()
{
	writeLetterToSegment(1, *currentPlayer);
}

int main(int argc, char const *argv[])
{
	uint8_t currentMatchAmount = START_NUMBER;
	setup();

	while (currentMatchAmount > 0)
	{
		displayScreen();
	}
	return 0;
}
