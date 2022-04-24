/**
 * @file basic-exercises-lib.c
 * @author Peter Buschenreiter
 * @brief
 * @version 0.1
 * @date 2022-04-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <basic-exercises-lib.h>
#include <led-lib.h>
#include <dimmed-led-lib.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

/**
 * @brief Turns on the 4 LEDs consecutively
 *
 * @param interval time between 2 LEDs turning on in milliseconds
 */
void lightUpConsecutively(int interval)
{
    enableAllLeds();
    for (int i = 0; i < 4; i++)
    {
        lightUpOneLed(i);
        _delay_ms(interval);
    }
}

/**
 * @brief Makes an LED blink n times
 *
 * @param lednumber LED that should blink
 * @param n amount of blinks
 * @param interval between blinks
 * @param duration duration of a single blink
 */
void blinkNTimes(int lednumber, int n, int interval, int duration)
{
    enableOneLed(lednumber);
    for (int i = 0; i < n; i++)
    {
        dimLed(lednumber, 100, duration);
        _delay_ms(interval);
    }
    lightDownOneLed(lednumber);
}

/**
 * @brief LED blinks with durations from 100ms to 1000ms with 50ms increments and 200ms in between blinks
 *
 * @param lednumber
 */
void lightBlinkIncremental(int lednumber)
{
    enableOneLed(lednumber);
    for (int i = 100; i <= 1000; i += 50)
    {
        lightUpOneLed(lednumber);
        _delay_ms(i);
        lightDownOneLed(lednumber);
        _delay_ms(200);
    }
}

/**
 * @brief Walks throught the array of lednumbers and toggles the respective LEDs
 *
 * @param lednumbers array of numbers between 0 and 3
 */
void lightToggleByArray(int *lednumbers, int size)
{
    enableAllLeds();
    for (int i = 0; i < size; i++)
    {
        lightToggleOneLed(lednumbers[i]);
        _delay_ms(500);
    }
}

/**
 * @brief Fills an array[20] with the values from 10 to 1000 in steps of 50
 *
 * @param arr the array that should be filled
 * @return int* the pointer to the array
 */
int *fillArray(int *arr)
{
    int size = 20;
    int increment = 50;
    int current = 10;

    for (int i = 0; i < size; i++)
    {
        arr[i] = current;
        current += increment;
    }
    return arr;
}

/**
 * @brief Traverses a string and calculates an index for each char and toggles the respective LED
 *
 * @param string
 */
void toggleLedByString(char string[])
{
    enableAllLeds();
    int i = 0;
    int lednumber;
    while (string[i])
    {
        lednumber = (((int)string[i]) - 1) % 4; // -1 to offset so that ((int) "a") % 4 == 0
        lightToggleOneLed(lednumber);
        _delay_ms(200);
        i++;
    }
}

/**
 * @brief Blinks all LEDs a certain amount of times
 *
 * @param amount
 */
void blinkAllLights(int amount)
{
    enableAllLeds();
    for (int i = 0; i < amount * 2; i++)
    {
        lightToggleAllLeds();
        _delay_ms(200);
    }
}

/**
 * @brief Traversesa string and calculates a number for each char and blinks all LEDs that many times
 *
 * @param string
 */
void blinkLightsByString(char string[])
{
    enableAllLeds();
    int i = 0;
    int amount;
    while (string[i])
    {
        amount = ((int)string[i]) - 96; // offset so that "a" blinks one time
        blinkAllLights(amount);
        _delay_ms(500);
        i++;
    }
}