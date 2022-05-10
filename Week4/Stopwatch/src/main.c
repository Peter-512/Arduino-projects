#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "display.h"
#include "usart.h"
// Global variables
// Enter here your global variables
void initTimer()
{
	// enter your code
}
void startTimer()
{
	// enter your code
}
void stopTimer()
{
	// enter your code
}
void tick()
{
	// enter your code
}
void writeTimeAndWait(uint8_t minuten, uint8_t seconden, int delay)
{
	// enter your code
}
// This ISR runs every 4 ms
ISR(TIMER2_COMPA_vect)
{
}

int main()
{
	initUSART();
	initDisplay();
	writeLongWord("Start the stopwatch by pressing button S1, stop by pressing button S2, and reset with S3", 89);

	while (1)
	{
		// Don't forget to initialize and start your timer
		// Also, don't forget sei()
	}
	return 0;
}