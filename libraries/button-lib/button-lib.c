#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <usart.h>
#define true 1
#define false !true
// #define DEBUG

#define NUMBER_OF_BUTTONS 3

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

void enableAllButtons()
{
    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        enableButton(i);
    }
}

void getButtonsReadyForInterrupts()
{
    PCICR |= _BV(PCIE1);
    PCMSK1 = PORTC;
    sei();
}

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

int buttonReleased(int button)
{
    return !buttonPushed(button);
}