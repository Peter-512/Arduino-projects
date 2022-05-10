/**
 * @file button-lib.c
 * @author Peter Buschenreiter
 * @brief Useful functions and defines to work with buttons on an arduino shield
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <usart.h>
#include <button-lib.h>
#define true 1
#define false !true
// #define DEBUG

/**
 * @brief Enabling a button for input
 *
 * @param button 0 -> first button, 1 -> second button, 2 -> third button
 */
void enableButton(int button)
{
    if (button < 0 || button >= NUMBER_OF_BUTTONS)
    {
        return;
    }
    DDRC &= ~_BV(PC1 + button);
    PORTC |= _BV(PC1 + button);
#ifdef DEBUG
    initUSART();
    printf("DDRC: ");
    printBinaryByte(DDRC);
    putchar('\n');
    printf("PORTC: ");
    printBinaryByte(PORTC);
    putchar('\n');
#endif
}

/**
 * @brief Enabling all buttons at the same time
 *
 */
void enableAllButtons()
{
    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        enableButton(i);
    }
}

/**
 * @brief Get all enabled buttons ready for interrupts and turn on global interrupts system
 *
 */
void getButtonsReadyForInterrupts()
{
    PCICR |= _BV(PCIE1);
    PCMSK1 |= PORTC; 
    sei();
}

/**
 * @brief Returns true if the corresponding button is pressed
 *
 * @param button to check if it is pressed
 * @return int 1 for true, 0 for false
 */
int buttonPushed(int button)
{
    if (button < 0 || button >= NUMBER_OF_BUTTONS)
    {
        return false;
    }
    if (bit_is_clear(PINC, (PC1 + button)))
    {
#ifdef DEBUG
        printBinaryByte(PINC);
        printf("\n");
#endif
        _delay_ms(1);
        if (bit_is_clear(PINC, (PC1 + button)))
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Returns true if the button is released
 *
 * @param button to check
 * @return int 1 for true, 0 for false
 */
int buttonReleased(int button)
{
    return !buttonPushed(button);
}

/**
 * @brief Returns true if any button is pressed
 *
 * @return int 1 for true, 0 for false
 */
int anyButtonPushed()
{
    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        if (buttonPushed(i))
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Checks which button is pushed.
 * Returns which button has been pressed 0 -> BUTTON1, 1 -> BUTTON2, 2 -> BUTTON3
 *
 * @return int Returns the amount of defined buttons in case no button has been pressed
 */
int whichButtonPushed()
{
    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        if (buttonPushed(i))
        {
            return i;
        }
    }
    return NUMBER_OF_BUTTONS;
}