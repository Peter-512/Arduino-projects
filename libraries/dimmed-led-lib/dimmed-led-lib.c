#include <dimmed-led-lib.h>
#include <led-lib.h>
#define __DELAY_BACKWARD_COMPATIBLE__ // alows delay functions to take variables as arguments
#include <util/delay.h>

/**
 * @brief Dim an LED to a certain percentage
 *
 * @param lednumber LED you want to dim
 * @param percentage of full brightness
 * @param duration how long it should be dimmed
 */
void dimLed(int lednumber, int percentage, int duration)
{
    enableOneLed(lednumber); // enable correct LED

    double onDuration = ((double)percentage / 100.0); // calculate the duration the LED is on
    double offDuration = 1 - onDuration;              // calculate the duration the LED is off

    while (duration-- > 0)
    {
        lightUpOneLed(lednumber);
        _delay_ms(onDuration);

        lightDownOneLed(lednumber);
        _delay_ms(offDuration);
    }
}

/**
 * @brief Fade in an LED over a specified duration
 *
 * @param lednumber to fade in
 * @param duration over which to fade in
 */
void fadeInLed(int lednumber, int duration)
{
    for (int i = 0; i < 100; i++)
    {
        dimLed(lednumber, i, duration / 100);
    }
    lightUpOneLed(lednumber);
}

/**
 * @brief Fade out an LED over a specified duration
 *
 * @param lednumber to fade out
 * @param duration over which to fade out
 */
void fadeOutLed(int lednumber, int duration)
{
    for (int i = 100; i > 0; i--)
    {
        dimLed(lednumber, i, duration / 100);
    }
    lightDownOneLed(lednumber);
}