/**
 * @file main.c
 * @author Peter Buschenreiter
 * @brief Simon Says game played on an arduino shield
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <button-lib.h>
#include <led-lib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <usart.h>
#include <stdlib.h>
#include <morse-lib.h>
#include <basic-exercises-lib.h>
#define true 1
#define false !true
#define SEQUENCE_LENGTH 10
#define START_DELAY 100
#define BETWEEN_DELAY 500
#define PUZZLE_DELAY 1000
// #define DEBUG

uint8_t buttonPushed = false;
uint8_t button;

/**
 * @brief Construct a new ISR object
 * Turns of all LEDs, saves the button that has been pressed into a global variable and sets a global flag to true
 *
 */
ISR(PCINT1_vect)
{
	lightDownAllLeds();
	button = whichButtonPushed();
	if (button < NUMBER_OF_BUTTONS)
	{
		lightUpOneLed(button);
		buttonPushed = true;
	}
}

/**
 * @brief Fills an array with numbers between 0 and the amount of buttons defined
 *
 * @param array to fill
 * @param len length of the array
 * @return uint8_t* the filled array
 */
uint8_t *generatePuzzle(uint8_t array[], uint8_t len)
{
	for (uint8_t i = 0; i < SEQUENCE_LENGTH; i++)
	{
		array[i] = rand() % NUMBER_OF_BUTTONS;
	}
	return array;
}

/**
 * @brief Prints the array nicely formatted to the serial monitor
 *
 * @param array to print
 * @param len length of array
 */
void printPuzzle(uint8_t array[], uint8_t len)
{
	printf("[");
	for (uint8_t i = 0; i < len; i++)
	{
		printf("%d ", array[i]);
	}
	printf("\b]\n");
}

/**
 * @brief Blinks the corresponding LEDs in sequence of the array for a certain length of it
 *
 * @param array to decide which LEDs to turn on
 * @param len how many array elements should be shown
 */
void playPuzzle(uint8_t *array, uint8_t len)
{
	lightDownAllLeds();
	lightUpOneLed(LED4);
	for (uint8_t i = 0; i < len; i++)
	{
		lightUpOneLed(array[i]);
		_delay_ms(PUZZLE_DELAY);
		lightDownOneLed(array[i]);
		_delay_ms(PUZZLE_DELAY / 10);
	}
	lightDownOneLed(LED4);
}

/**
 * @brief Traverses an array and checks via interrupts if the correct sequence has been entered.
 * BUTTON1 -> LED1, BUTTON2 -> LED2, BUTTON3 -> LED3
 *
 * @param array to check against the input
 * @param len how far to check
 * @return uint8_t
 */
uint8_t readInput(uint8_t *array, uint8_t len)
{
	uint8_t current = 0;
	while (current < len)
	{
		buttonPushed = false;
		sei();
		while (!buttonPushed)
		{
#ifndef DEBUG
			_delay_us(1);
#endif
#ifdef DEBUG
			printf("Waiting for input...\n");
#endif
		}
		cli();
		if (button == array[current])
		{
			printf("Button #%d was correct!\n", button + 1);
			current++;
		}
		else
		{
			printf("Button #%d was incorrect!\n", button + 1);
			return false;
		}
	}
	return true;
}

int main(int argc, char const *argv[])
{
	// Preparations to play
	initUSART();
	enableAllLeds();
	enableAllButtons();
	getButtonsReadyForInterrupts();

	// Seeding the random generator
	int seed = 0;
	while (!buttonPushed)
	{
		lightUpOneLed(LED4);
		_delay_ms(START_DELAY);
		lightDownOneLed(LED4);
		_delay_ms(START_DELAY);
		seed++;
	}
	cli();
#ifdef DEBUG
	printf("Seed: %d\n", seed);
#endif
	srand(seed);

	// Creating the random series
	uint8_t sequence[SEQUENCE_LENGTH];
	generatePuzzle(sequence, SEQUENCE_LENGTH);

#ifdef DEBUG
	printPuzzle(sequence, SEQUENCE_LENGTH);
#endif

	countdown();
	_delay_ms(BETWEEN_DELAY);

	// Playing the puzzle
	for (uint8_t i = 1; i < SEQUENCE_LENGTH; i++)
	{
		playPuzzle(sequence, i);

		// Reading the input of the user
		if (!readInput(sequence, i))
		{
			blinkAllLights(2);
			printf("Oops, you made a mistake. Try again.\n");
			i--;
		}
		_delay_ms(BETWEEN_DELAY);
	}

	// Winning sequence
	lightDownAllLeds();
	endSequence();
	printf("\nYou made it all the way!\n");

	return 0;
}