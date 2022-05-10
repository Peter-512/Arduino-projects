#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <display.h>
#include <usart.h>
#include <button-lib.h>
#include <boolean.h>
#define MULTIPLE 250
#define SECONDS_IN_MINUTE 60
#define DEBUG

uint8_t counter = 0;
uint8_t seconds = 0;
uint8_t minutes = 0;

void initTimer()
{
	TCCR2A |= _BV(WGM21);
	TIMSK2 |= _BV(OCIE0A);
	OCR2A = 249;
	sei();
}

void startTimer()
{
	TCCR2B |= _BV(CS22) | _BV(CS21);
}

void stopTimer()
{
	TCCR2B = 0;
}

void resetTimer()
{
	counter = 0;
	seconds = 0;
	minutes = 0;
}

void tick()
{
	seconds++;
	seconds %= SECONDS_IN_MINUTE;

	if (!seconds)
	{
		minutes++;
	}
}

void displayTime()
{
	int time = minutes * 100 + seconds;
	writeNumber(time);

	writeSymbolToSegment(SEGMENT2, 0x40); // display a dot between minutes and seconds
}

ISR(PCINT1_vect)
{
	uint8_t button = whichButtonPushed();
	if (button < NUMBER_OF_BUTTONS)
	{
		switch (button)
		{
		case BUTTON1:
			startTimer();
			break;

		case BUTTON2:
			stopTimer();
			break;

		case BUTTON3:
			resetTimer();
			break;

		default:
			break;
		}
	}
}

// This ISR runs every 4 ms
ISR(TIMER2_COMPA_vect)
{
	counter++;
	counter %= MULTIPLE;

	if (!counter)
	{
		tick();
	}
}

int main()
{
	initUSART();
	initDisplay();
	initTimer();
	enableAllButtons();
	getButtonsReadyForInterrupts();
	writeLongWord("Start with button one stop with two and reset with three", 57);

	while (1)
	{
		displayTime();
	}
	return 0;
}