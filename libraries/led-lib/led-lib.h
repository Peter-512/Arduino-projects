#include <avr/io.h>
#define LED1 0
#define LED2 1
#define LED3 2
#define LED4 3
#define NUMBER_OF_LEDS 4

void enableOneLed(int index);

void enableMultipleLeds(uint8_t bitmask);

void enableAllLeds();

void lightUpOneLed(int index);

void lightUpMultipleLeds(uint8_t bitmask);

void lightUpAllLeds();

void lightDownOneLed(int index);

void lightDownMultipleLeds(uint8_t bitmask);

void lightDownAllLeds();

void lightToggleOneLed(int index);

void lightToggleAllLeds();