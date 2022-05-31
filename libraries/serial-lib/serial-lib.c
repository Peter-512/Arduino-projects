#include <avr/io.h>

void initSerial()
{
	UCSR0B |= _BV(RXCIE0); // this ensures an interrupt is triggered when serial data is received.
}