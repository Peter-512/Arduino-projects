# Lunar Lander

![Lunar Lander](resources/LunarLander.jpeg)

## Preparations

```c
void initialize()
{
	initUSART();
	initDisplay();
	enableAllButtons();
	getButtonsReadyForInterrupts();
	enableAllLeds();
	enableBuzzer();
	initTimer();
	initLEDTimer();
}
```

Enabling all necessary systems

## Primary Timer

```c
void initTimer()
{
	TCCR2A |= _BV(WGM21);
	TIMSK2 |= _BV(OCIE0A);
	OCR2A = 249;
	sei();
	TCCR2B |= _BV(CS22) | _BV(CS21);
}
```

```c
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
```

I set up the primary timer so that it interrupts every 4ms. I increment the timerCounter every time and once it reaches 250 (MULTIPLE), i set it back to 0. When that happens, I know that exactly 1 second has passed (4ms \* 250 = 1s) and update all the parameters like distance, speed, fuel reserve, burst and currently used fuel cell. Finally, I add 4 (ms) to the buttonCounter which i use to keep track of when I pressed and released a button.

## Getting Inputs

```c
ISR(PCINT1_vect)
{
	buttonState = anyButtonPushed();
	if (buttonState != lastButtonState)
	{
		updateButtonTime();
	}
	lastButtonState = buttonState;
}
```

I first get my current button state. Then I check if it has changed by comparing it with the last button state. If so, then I want to update the button time. Lastly, I set the last button state to the current button state for the next iteration.

```c
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
```

If a button is pushed down, I set the push start time to the current buttonCounter. Otherwise, if it was just released, I set the end time and set the buttonCounter back to 0.

## Updating Parameters

```c
void updateParameters()
{
	if (lastButtonState) // if button is still pushed
	{
		buttonPushEnd = buttonCounter;
	}
	burst = MAX_BURST * (buttonPushEnd - buttonPushStart) / 1000;
	if (burst > fuelReserve)
	{
		burst = fuelReserve;
	}

	buttonCounter = 0;
	buttonPushStart = 0;
	buttonPushEnd = 0;

	currentSpeed += gravity - burst / 5;
	distance -= currentSpeed;
	if (distance < 0)
	{
		distance = 0;
	}
	fuelReserve -= burst;
	currentFuelCell = (int)(fuelReserve / MAX_FUEL_CELL);

	if (logBook)
	{
		saveParameters();
	}
}
```

First, I calculate the correct burst, by using the button start and end times. If the button is still held down at this point, I'm setting the end time to the current buttonCounter. And to not be able to use more fuel than is still available, I'm checking if the burst is bigger than the fuelReserve and setting it to the remaining fuelReserve if that's the case. Then I set all buttonTimer variables to 0.  
The rest of the function calculates all the remaining parameters (with an extra check so distance can't become negative) and then saves those parameters.

## LED Timer

```c
void initLEDTimer()
{
	TCCR0A |= _BV(WGM00) | _BV(WGM01);
	TCCR0B |= _BV(CS02) | _BV(CS00);
	TIMSK0 |= _BV(TOIE0);
	TIMSK0 |= _BV(OCIE0A);
}
```

```c
// This ISR runs every time TCNT0 equals the value in the OCRA register
ISR(TIMER0_COMPA_vect)
{
	lightDownOneLed(currentFuelCell);
}
```

```c
// This ISR runs every time TCNT0 equals the TOP value (255)
ISR(TIMER0_OVF_vect)
{
	if (fuelReserve)
		lightUpOneLed(currentFuelCell);
}
```

Turning the LED of the current fuel cell on, as long as I have fuel left in the current fuel cell, whenever the timer reaches the overflow interrupt.  
Turning it back off when it reaches the OCRA value. This way, the LED will be brighter, the higher the OCRA value is.

## Showing Parameters

```c
void showParameters()
{
	writeNumber(distance);

	// * converting the current fuel cell capacity to a value between 0 and 255
	OCR0A = (fuelReserve % MAX_FUEL_CELL) / 1.5;

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
```

First up, I am writing the distance to the display. Then I calculate how much fuel is left in the current fuel cell and divide it by 1.5 to get the value between 0 and 255 which is needed for the OCR0A.  
After that, a switch case on the current fuel cell determines which LEDs should be fully on or off.

## Logging Parameters

```c
typedef struct
{
	int distance;
	float currentSpeed;
	uint8_t burst;
	int fuelReserve;
} LOG_ENTRY;

LOG_ENTRY *logBook;
```

I start by creating a struct and define it as type LOG_ENTRY. Then I make a pointer of that type called logBook.

```c
logBook = malloc(SIZE(LOG_ENTRY));

createLogEntry();
```

```c
void createLogEntry()
{
	logBook[logBookLength].burst = burst;
	logBook[logBookLength].currentSpeed = currentSpeed;
	logBook[logBookLength].distance = distance;
	logBook[logBookLength].fuelReserve = fuelReserve;
}
```

In my main, right after `initialize()`, I allocate enough space for the first log entry and save the starting values into it.

```c
void saveParameters()
{
	logBookLength++;

	logBook = realloc(logBook, NEW_SIZE(LOG_ENTRY, logBookLength));

	if (logBook)
	{
		createLogEntry();
	}
}
```

```c
#define NEW_SIZE(type, len) ((len + 1) * sizeof(type))
```

As we saw earlier, this function gets called at the end of `updateParameters()`, so once a second.  
I first increment the logBookLength, then dynamically reallocate the required memory utilizing my own `NEW_SIZE` macro.  
If the reallocation was successful, I create another log entry.

## Landing

```c
while (distance > MAX_LANDING_HEIGHT || (distance > 0 && currentSpeed > MAX_LANDING_SPEED))
{
    showParameters();
}

cli();

if (currentSpeed <= MAX_LANDING_SPEED)
{
    playTone(2, 1000);
    writeLongWord("Thats one small step for man one giant leap for mankind", 58);
}
else
{
    for (int i = 1000; i > 100; i--)
    {
        playTone(i, 2);
    }
    playTone(4000, 2500);

    writeLongWord("Houston we have a problem", 26);
}

printLogBook();

free(logBook);
```

This is the rest of the `main` function.  
As long one of two conditions is fulfilled, the Arduino will show all the parameters.

1. distance is bigger than landing height
2. distance is bigger than 0 and current speed is bigger than landing speed

As soon as it breaks out of that loop, the global interrupt system is turned off and a message appears on the display, indicating either success or failure, along with a corresponding sound (two short beeps or falling sound, followed by a long beep respectively).  
Finally the entire logBook is printed to the serial monitor and its memory freed.
