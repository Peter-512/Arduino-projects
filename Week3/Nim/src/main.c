#include <avr/io.h>
#include <button-lib.h>
#include <display.h>
#include <stdlib.h>
#include <potentiometer.h>
#define START_NUMBER 21
#define MAX_NUMBER 3

#define DEBUG
#ifdef DEBUG
#include <usart.h>
#endif

void setup()
{
	initDisplay();
	initADC();
	uint16_t seed = getPotentiometerValue();
	srand(seed);
	writeNumberAndWait(seed, 10000);
}

int main(int argc, char const *argv[])
{
	uint8_t currentMatchAmount = START_NUMBER;
	setup();
	return 0;
}
