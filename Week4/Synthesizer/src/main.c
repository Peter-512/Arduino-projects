#include <potentiometer.h>
#include <led-lib.h>
#include <button-lib.h>
#include <avr/interrupt.h>
#include <buzzer.h>
#include <display.h>
#include <avr/io.h>
#include <boolean.h>
#define DURATION 1

bool playing = false;

/**
 * @brief Construct a new ISR object
 *
 *
 */
ISR(PCINT1_vect)
{
	if (anyButtonPushed())
	{
		playing = !playing;
	}
}

int main(int argc, char const *argv[])
{
	initDisplay();
	initADC();
	enableAllButtons();
	getButtonsReadyForInterrupts();
	enableBuzzer();

	while (1)
	{
		uint16_t potentiometerValue = getPotentiometerValue();
		writeNumber(potentiometerValue);

		if (playing)
		{
			playTone(potentiometerValue, DURATION);
		}
	}

	return 0;
}
