#include <led-lib.h>
#include <avr/io.h>

#define NUMBER_OF_LEDS 4

void enableOneLed(int index)
{
    if (index >= 0 || index < NUMBER_OF_LEDS)
    {
        DDRB |= (1 << (PB2 + index));
        lightDownOneLed(index); // to suppress automatic turning on of the LED
    }
}

void enableMultipleLeds(uint8_t bitmask)
{
    if (bitmask >= 0b00000000 || bitmask <= 0b00001111)
    {
        DDRB = bitmask << PB2;
        lightDownMultipleLeds(bitmask); // to suppress automatic turning on of the LEDs
    }
}

void enableAllLeds()
{
    DDRB = 0b00111100;
    lightDownAllLeds(); // to suppress automatic turning on of the LEDs
}

void lightUpOneLed(int index)
{
    if (index >= 0 || index < NUMBER_OF_LEDS)
    {
        PORTB &= ~(1 << (PB2 + index));
    }
}

void lightUpMultipleLeds(uint8_t bitmask)
{
    if (bitmask >= 0b00000000 || bitmask <= 0b00001111)
    {
        PORTB = ~bitmask << PB2;
    }
}

void lightUpAllLeds()
{
    PORTB = 0b00000000;
}

void lightDownOneLed(int index)
{
    if (index >= 0 || index < NUMBER_OF_LEDS)
    {
        PORTB |= (1 << (PB2 + index));
    }
}

void lightDownMultipleLeds(uint8_t bitmask)
{
    if (bitmask >= 0b00000000 || bitmask <= 0b00001111)
    {
        PORTB = bitmask << PB2;
    }
}

void lightDownAllLeds()
{
    PORTB = 0b00111100;
}

void lightToggleOneLed(int index)
{
    if (index >= 0 || index < NUMBER_OF_LEDS)
    {
        PORTB ^= (1 << (PB2 + index));
    }
}

void lightToggleAllLeds()
{
    PORTB = ~PORTB;
}