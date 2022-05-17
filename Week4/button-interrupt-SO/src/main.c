// https://stackoverflow.com/questions/32427630/how-to-detect-how-long-a-button-was-pressed-in-arduino

/**
 * @file main.c
 * @author Peter Buschenreiter
 * @brief This program counts the time BUTTON1 is idle and pressed and prints those times to the serial monitor
 * @version 0.1
 * @date 2022-05-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <button-lib.h>
#include <usart.h>
#include <avr/interrupt.h>
#include <boolean.h>
#define MULTIPLE 250

int buttonPin = BUTTON1;

uint32_t timerCounter = 0;
bool buttonState = false;	  // current state of the button
bool lastButtonState = false; // previous state of the button
int startPressed = 0;		  // the moment the button was pressed
int endPressed = 0;			  // the moment the button was released
int holdTime = 0;			  // how long the button was held
int idleTime = 0;			  // how long the button was idle

void updateState()
{

	if (buttonState) // the button has been just pressed
	{
		startPressed = timerCounter;
		idleTime = startPressed;

		printf("The button was idle for %.2f seconds.\n", (float)idleTime / 1000);
	}
	else // the button has been just released
	{
		endPressed = timerCounter;
		holdTime = endPressed - startPressed;

		printf("The button was held for %.2f seconds.\n", (float)holdTime / 1000);
		timerCounter = 0;
	}
}

// This ISR runs every 4 ms
ISR(TIMER2_COMPA_vect)
{
	timerCounter += 4;
}

void initTimer()
{
	TCCR2A |= _BV(WGM21);
	TIMSK2 |= _BV(OCIE0A);
	OCR2A = 249;
	sei();
	TCCR2B |= _BV(CS22) | _BV(CS21);
}

ISR(PCINT1_vect)
{
	buttonState = buttonPushed(buttonPin); // read the button input
	if (buttonState != lastButtonState)	   // button state changed
	{
		updateState();
	}
	lastButtonState = buttonState; // save state for next loop
}

int main(int argc, char const *argv[])
{
	initUSART();
	enableButton(buttonPin);
	getButtonsReadyForInterrupts();
	initTimer();

	while (true)
	{
	}

	return 0;
}