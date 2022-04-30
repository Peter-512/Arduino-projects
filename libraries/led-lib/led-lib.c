/**
 * @file led-lib.c
 * @author Peter Buschenreiter
 * @brief
 * @version 0.1
 * @date 2022-04-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <led-lib.h>
#include <avr/io.h>

/**
 * @brief Enables an LED for output and suppresses the initial turning on
 *
 * @param index 0 -> first LED, 1 -> second LED, 2 -> third LED, 3 -> fourth LED
 */
void enableOneLed(int index)
{
    if (index >= 0 || index < NUMBER_OF_LEDS)
    {
        DDRB |= (1 << (PB2 + index));
        lightDownOneLed(index); // to suppress automatic turning on of the LED
    }
}

/**
 * @brief Enables multiple LEDs at the same time and suppresses the initial turning on
 *
 * @param bitmask The 4 LSBs that are set correspong to the indexes that should be enabled
 */
void enableMultipleLeds(uint8_t bitmask)
{
    if (bitmask >= 0b00000000 || bitmask <= 0b00001111)
    {
        DDRB = bitmask << PB2;
        lightDownMultipleLeds(bitmask); // to suppress automatic turning on of the LEDs
    }
}

/**
 * @brief Enables all LEDs at the same time and suppresses the initial turning on
 *
 */
void enableAllLeds()
{
    DDRB = 0b00111100;
    lightDownAllLeds(); // to suppress automatic turning on of the LEDs
}

/**
 * @brief Turns on one LED at specified index
 *
 * @param index 0 -> first LED, 1 -> second LED, 2 -> third LED, 3 -> fourth LED
 */
void lightUpOneLed(int index)
{
    if (index >= 0 || index < NUMBER_OF_LEDS)
    {
        PORTB &= ~(1 << (PB2 + index));
    }
}

/**
 * @brief Turns on multiple LEDs at the same time
 *
 * @param bitmask  The 4 LSBs that are set correspong to the indexes that should be turned on
 */
void lightUpMultipleLeds(uint8_t bitmask)
{
    if (bitmask >= 0b00000000 || bitmask <= 0b00001111)
    {
        PORTB = ~bitmask << PB2;
    }
}

/**
 * @brief Turns on all LEDs at the same time
 *
 */
void lightUpAllLeds()
{
    PORTB = 0b00000000;
}

/**
 * @brief Turns off one LED at specified index
 *
 * @param index 0 -> first LED, 1 -> second LED, 2 -> third LED, 3 -> fourth LED
 */
void lightDownOneLed(int index)
{
    if (index >= 0 || index < NUMBER_OF_LEDS)
    {
        PORTB |= (1 << (PB2 + index));
    }
}

/**
 * @brief Turns off multiple LEDs at the same time
 *
 * @param bitmask The 4 LSBs that are set correspong to the indexes that should be turned off
 */
void lightDownMultipleLeds(uint8_t bitmask)
{
    if (bitmask >= 0b00000000 || bitmask <= 0b00001111)
    {
        PORTB = bitmask << PB2;
    }
}

/**
 * @brief Turns off all LEDs at the same time
 *
 */
void lightDownAllLeds()
{
    PORTB = 0b00111100;
}

/**
 * @brief Toggles one LED at specified index
 *
 * @param index 0 -> first LED, 1 -> second LED, 2 -> third LED, 3 -> fourth LED
 */
void lightToggleOneLed(int index)
{
    if (index >= 0 || index < NUMBER_OF_LEDS)
    {
        PORTB ^= (1 << (PB2 + index));
    }
}

/**
 * @brief Toggles all LEDs at the specified index
 *
 */
void lightToggleAllLeds()
{
    PORTB = ~PORTB;
}