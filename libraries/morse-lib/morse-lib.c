/**
 * @file morse-lib.c
 * @author Peter Buschenreiter
 * @brief
 * @version 0.1
 * @date 2022-04-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <morse-lib.h>
#include <led-lib.h>
#define __DELAY_BACKWARD_COMPATIBLE__ // alows delay functions to take variables as arguments
#include <util/delay.h>
#include <stdlib.h>
#include <basic-exercises-lib.h>
#include <usart.h>
#define UNIT 200

/**
 * @brief Counts down by first turning on all LEDs then one off at a time
 *
 */
void countdown()
{
    enableAllLeds();
    lightUpAllLeds();
    _delay_ms(1000);
    for (int i = 0; i < 4; i++)
    {
        lightDownOneLed(i);
        _delay_ms(1000);
    }
}

/**
 * @brief Displays one dit with a duration of one UNIT and and an intra-character space
 *
 */
void dit()
{
    lightUpAllLeds();
    _delay_ms(UNIT);
    lightDownAllLeds();
    _delay_ms(UNIT);
}

/**
 * @brief Displays one dah with a duration of three UNITs and and an intra-character space
 *
 */
void dah()
{
    lightUpAllLeds();
    _delay_ms(UNIT * 3);
    lightDownAllLeds();
    _delay_ms(UNIT);
}

/**
 * @brief Displays the character by blinking the LEDs according to its morse code
 *
 * @param character to be displayed
 * @param charCounter the counter from the surrounding loop (we only increment it if a valid character has been passed to the function, useful for morseTrainer when we pass a random character)
 * @return int charCounter
 */
int morseChar(char character, int charCounter)
{
    int ascii = (int)character;
    if (ascii >= 97)
    {
        ascii -= 32;
    }
    switch (ascii)
    {
    case 65: // A
        dit();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 66: // B
        dah();
        dit();
        dit();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 67: // C
        dah();
        dit();
        dah();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 68: // D
        dah();
        dit();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 69: // E
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 70: // F
        dit();
        dit();
        dah();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 71: // G
        dah();
        dah();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 72: // H
        dah();
        dah();
        dah();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 73: // I
        dah();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 74: // J
        dit();
        dah();
        dah();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 75: // K
        dah();
        dit();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 76: // L
        dit();
        dah();
        dit();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 77: // M
        dah();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 78: // N
        dah();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 79: // O
        dah();
        dah();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 80: // P
        dit();
        dah();
        dah();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 81: // Q
        dah();
        dah();
        dit();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 82: // R
        dit();
        dah();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 83: // S
        dit();
        dit();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 84: // T
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 85: // U
        dit();
        dit();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 86: // V
        dit();
        dit();
        dit();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 87: // W
        dit();
        dah();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 88: // X
        dah();
        dit();
        dit();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 89: // Y
        dah();
        dit();
        dah();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 90: // Z
        dah();
        dah();
        dit();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 32: // " " [SPACE]
        _delay_ms(UNIT * 4);
        break;

    case 48: // 0
        dah();
        dah();
        dah();
        dah();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 49: // 1
        dit();
        dah();
        dah();
        dah();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 50: // 2
        dit();
        dit();
        dah();
        dah();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 51: // 3
        dit();
        dit();
        dit();
        dah();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 52: // 4
        dit();
        dit();
        dit();
        dit();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 53: // 5
        dit();
        dit();
        dit();
        dit();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 54: // 6
        dah();
        dit();
        dit();
        dit();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 55: // 7
        dah();
        dah();
        dit();
        dit();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 56: // 8
        dah();
        dah();
        dah();
        dit();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 57: // 9
        dah();
        dah();
        dah();
        dah();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 38: // &
        dit();
        dah();
        dit();
        dit();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 39: // ' [APOSTROPHY, SINGLE QUOTE]
        dit();
        dah();
        dah();
        dah();
        dah();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 64: // @
        dit();
        dah();
        dah();
        dit();
        dah();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 41: // )
        dah();
        dit();
        dah();
        dah();
        dit();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 40: // (
        dah();
        dit();
        dah();
        dah();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 58: // :
        dah();
        dah();
        dah();
        dit();
        dit();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 44: // , [COMMA]
        dah();
        dah();
        dit();
        dit();
        dah();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 61: // =
        dah();
        dit();
        dit();
        dit();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 33: // !
        dah();
        dit();
        dah();
        dit();
        dah();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 46: // . [PERIOD]
        dit();
        dah();
        dit();
        dah();
        dit();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 45: // - [MINUS, HYPHEN]
        dah();
        dit();
        dit();
        dit();
        dit();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 42: // * [MULTIPLICATION SIGN] (instead of x)
        dah();
        dit();
        dit();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 37: // %
        dah();
        dah();
        dah();
        dah();
        dah();
        _delay_ms(UNIT * 2);
        dah();
        dit();
        dit();
        dah();
        dit();
        _delay_ms(UNIT * 2);
        dah();
        dah();
        dah();
        dah();
        dah();
        _delay_ms(UNIT * 2);
        break;

    case 43: // +
        dit();
        dah();
        dit();
        dah();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 34: // " [DOUBLE QUOTE]
        dit();
        dah();
        dit();
        dit();
        dah();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 63: // ?
        dit();
        dit();
        dah();
        dah();
        dit();
        dit();
        _delay_ms(UNIT * 2);
        break;

    case 47: // / [FORWARD SLASH]
        dah();
        dit();
        dit();
        dah();
        dit();
        _delay_ms(UNIT * 2);
        break;

    default:
        charCounter--;
        break;
    }
    return ++charCounter;
}

/**
 * @brief Displays the countdown, then the supplied string in morse code with the LEDs and the end sequence at the end
 *
 * @param string to be displayed as morse
 */
void morse(char string[])
{
    countdown();
    int i = 0;

    while (string[i])
    {
        i = morseChar(string[i], i);
    }
    _delay_ms(1000);
    endSequence();
}

/**
 * @brief Lights the LEDs in a frivolous dance sequence
 *
 */
void endSequence()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i % 2 == 0)
            {
                lightToggleOneLed(j);
            }
            else
            {
                lightToggleOneLed(6 - (j + 3)); // reverses the order
            }
            _delay_ms(100);
        }
    }
}

/**
 * @brief Training morse code.
 * Counts down. Displays 10 characters in morse code and waits 2 seconds between each one before showing the answer in the serial monitor.
 * Afterwards displays the end sequence.
 */
void morseTrainer()
{
    srand(0);
    initUSART();
    countdown();
    int i = 0;
    while (i < 10)
    {
        char character = (char)(rand() % 59 + 32);
        i = morseChar(character, i);
        _delay_ms(2000);
        printf("%c", character);
    }
    endSequence();
}