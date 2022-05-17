#include <led-lib.h>
#include <dimmed-led-lib.h>
#include <boolean.h>
#include <avr/io.h>
#include <buzzer.h>
#include <display.h>
#include <button-lib.h>
#include <avr/interrupt.h>
#define MULTIPLE 250
// #define DEBUG
#ifdef DEBUG
#include <usart.h>
#endif
#define MOON
#ifndef MOON
#define EARTH
#endif
#define MAX_LANDING_SPEED 5
#define MAX_LANDING_HEIGHT 3
#define MAX_BURST 50
#define STARTING_FUEL 1500
#define MAX_FUEL_CELL (STARTING_FUEL / NUMBER_OF_LEDS)

float currentSpeed = 100; // speed in m/s (meters per second)
#ifdef MOON
const float gravity = 1.622; // acceleration in m/s²
#endif
#ifdef EARTH
float gravity = 9.807; // acceleration in m/s²
#endif
int distance = 9999;			   // distance to the lunar surface in meters - m
float fuelReserve = STARTING_FUEL; // liter
float burst = 0;
uint8_t timerCounter = 0;
bool buttonState = false;
bool lastButtonState = false;
uint16_t buttonCounter = 0;
uint16_t buttonPushStart = 0;
uint16_t buttonPushEnd = 0;
uint8_t currentFuelCell = NUMBER_OF_LEDS;
uint32_t ledCounter = 0;

void updateParameters()
{
#ifdef DEBUG
	printf("Button pressed:  %s\n", lastButtonState ? "true" : "false");
#endif

	if (lastButtonState) // if button is still pushed
	{
		buttonPushEnd = buttonCounter;
	}
	burst = MAX_BURST * (buttonPushEnd - buttonPushStart) / 1000;
	if (burst > fuelReserve)
	{
		burst = fuelReserve;
	}

#ifdef DEBUG
	printf("ButtonPushStart: %d\n", buttonPushStart);
	printf("ButtonPushEnd:   %d\n", buttonPushEnd);
	printf("Burst:           %.2f\n", burst);
#endif

	buttonCounter = 0;
	buttonPushStart = 0;
	buttonPushEnd = 0;
	currentSpeed += gravity - burst / 5;
	distance -= currentSpeed;
	fuelReserve -= burst;
	currentFuelCell = (int)(fuelReserve / MAX_FUEL_CELL);

#ifdef DEBUG
	printf("Distance:        %d\n", distance);
	printf("Current speed:   %.2f \n", currentSpeed);
	printf("Full fuel cells: %d \n", currentFuelCell);
	printf("Fuel reserve:    %.2f \n\n", fuelReserve);
#endif
}

void updateButtonTime()
{
	if (buttonState)
	{
		buttonPushStart = buttonCounter;
	}
	else
	{
		buttonPushEnd = buttonCounter;
		buttonCounter = 0;
	}
}

// This ISR runs every 4 ms
ISR(TIMER2_COMPA_vect)
{
	timerCounter++;
	timerCounter %= MULTIPLE;

	if (!timerCounter) // one second has passed
	{
		updateParameters();
	}

	buttonCounter += 4;
}

ISR(PCINT1_vect)
{
	buttonState = anyButtonPushed();
	if (buttonState != lastButtonState)
	{
		updateButtonTime();
	}
	lastButtonState = buttonState;
}

// This ISR runs every time TCNT0 equals the value in the OCRA register
ISR(TIMER0_COMPA_vect)
{
	lightDownOneLed(currentFuelCell);
	ledCounter++;
}

// This ISR runs every time TCNT0 equals the TOP value (255)
ISR(TIMER0_OVF_vect)
{
	if (fuelReserve)
		lightUpOneLed(currentFuelCell);
	ledCounter++;
}

void initTimer()
{
	TCCR2A |= _BV(WGM21);
	TIMSK2 |= _BV(OCIE0A);
	OCR2A = 249;
	sei();
	TCCR2B |= _BV(CS22) | _BV(CS21);
}

void initLEDTimer()
{
	TCCR0A |= _BV(WGM00) | _BV(WGM01);
	TCCR0B |= _BV(CS02) | _BV(CS00);
	TIMSK0 |= _BV(TOIE0);
	TIMSK0 |= _BV(OCIE0A);
}

void initialize()
{
#ifdef DEBUG
	initUSART();
#endif
	initDisplay();
	enableAllButtons();
	getButtonsReadyForInterrupts();
	enableAllLeds();
	enableBuzzer();
	initTimer();
	initLEDTimer();
}

void showParameters()
{
	writeNumber(distance);

	OCR0A = ((int)fuelReserve % MAX_FUEL_CELL) / 1.5; // * converting the current fuel cell capacity to a value between 0 and 255

	switch (currentFuelCell)
	{
	case 0:
	{
		lightDownOneLed(LED2);
		break;
	}
	case 1:
	{
		lightUpOneLed(LED1);
		lightDownOneLed(LED3);
		break;
	}
	case 2:
	{
		lightDownOneLed(LED4);
		lightUpMultipleLeds(0b00000011);
		break;
	}
	case 3:
	{
		lightUpMultipleLeds(0b00000111);
		break;
	}
	case 4:
	{
		lightUpAllLeds();
		break;
	}
	}
}

int main(int argc, char const *argv[])
{
	initialize();
	while (distance > MAX_LANDING_HEIGHT || (distance > 0 && currentSpeed > MAX_LANDING_SPEED))
	{
		showParameters();
	}

	cli();
	if (currentSpeed <= MAX_LANDING_SPEED)
	{
		writeLongWord("Thats one small step for man one giant leap for mankind", 58);
	}
	else
	{
		writeLongWord("Houston we have a problem", 26);
	}
	return 0;
}
