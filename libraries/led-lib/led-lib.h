#include <avr/io.h>

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