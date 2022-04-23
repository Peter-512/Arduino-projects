#include <led-chaos-lib.h>
#include <led-lib.h>
#include <dimmed-led-lib.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <stdlib.h>

/**
 * @brief Infinite loop that turns on a random LED with a random brightness for a random duration between 100ms and 1000ms
 *
 */
void chaosLights()
{
    srand(0);
    enableAllLeds();
    while (1)
    {
        dimLed(rand() % 4, rand() % 100, rand() % 900 + 100);
    }
}

/**
 * @brief Infinite loop that toggles a random LED with a random delay between 100ms and 500ms
 *
 */
void chaosLightsExtra()
{
    srand(0);
    enableAllLeds();
    while (1)
    {
        lightToggleOneLed(rand() % 4);
        _delay_ms(rand() % 400 + 100);
    }
}