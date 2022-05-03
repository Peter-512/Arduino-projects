# Simon Says Project

## Preparations to play

I start by initializing everything I need for the game to work.

```c
initUSART();
enableAllLeds();
enableAllButtons();
getButtonsReadyForInterrupts();
```

They all do more or less what they say, but let's look at two in a bit more detail.

---

```c
/**
 * @brief Enables all LEDs at the same time and suppresses the initial turning on
 *
 */
void enableAllLeds()
{
    DDRB = 0b00111100;
    lightDownAllLeds(); // to suppress automatic turning on of the LEDs
}
```

---

Here I set the Data Direction Register of PORTB to `0b00111100` to enable all pins that are connected to LEDs on the shield.  
At the beginning the Pin Output Register will be `0b00000000` and because a 0 makes an LED turn on, all 4 LEDs turn on as soon as I enable them. That's why I immediately turn down all LEDs as well.

---

```c
/**
 * @brief Get all enabled buttons ready for interrupts and turn on global interrupts system
 *
 */
void getButtonsReadyForInterrupts()
{
    PCICR |= _BV(PCIE1);
    PCMSK1 = PORTC;
    sei();
}
```

---

I set the Pin Change Interrupt Control Register (PCICR) to the bit value of PCIE1. Then the Pin Change Mask Register (PCMSK1) gets set to PORTC, which enables all enabled buttons for the interrupt system.  
Finally, I also enable the interrupt system globally, using the `sei()` macro.

## Seeding the random generator

The program now starts by making the fourth LED blink, and incrementing a seed variable with every loop, until a global `button_pushed` variable becomes `true`, which happens via an interrupt.  
Right after that it sets the random seed and turns off the global interrupt system by calling the `cli()` macro.

---

```c
int seed = 0;
while (!button_pushed)
{
	lightUpOneLed(LED4);
	_delay_ms(START_DELAY);
	lightDownOneLed(LED4);
	_delay_ms(START_DELAY);
	seed++;
}
cli();
srand(seed);
```

---

## Creating the random series

I create an array of size 10 and call a method that fills it with random numbers from 0 to 2 (both inclusive).

---

```c
/**
 * @brief Fills an array with numbers between 0 and the amount of buttons defined
 *
 * @param array to fill
 * @param len length of the array
 * @return uint8_t* the filled array
 */
uint8_t *generatePuzzle(uint8_t array[], uint8_t len)
{
	for (uint8_t i = 0; i < SEQUENCE_LENGTH; i++)
	{
		array[i] = rand() % NUMBER_OF_BUTTONS;
	}
	return array;
}
```

---

To confirm that the array has been filled correctly, I wrote the `printPuzzle()` function, which loops through all elements and prints them nicely formatted to the serial monitor.

---

```c
/**
 * @brief Prints the array nicely formatted to the serial monitor
 *
 * @param array to print
 * @param len length of array
 */
void printPuzzle(uint8_t array[], uint8_t len)
{
	printf("[");
	for (uint8_t i = 0; i < len; i++)
	{
		printf("%d ", array[i]);
	}
	printf("\b]\n");
}
```

## Playing the puzzle

The following function loops through the puzzle sequence array that I constructed earlier and lights the corresponding LED.
The len parameter that get passed to it indicates how far along the player has made it through the sequence so far.

| Number | LED  |
| ------ | ---- |
| 0      | LED1 |
| 1      | LED2 |
| 2      | LED3 |

During that, LED4 is constantly lit, to indicate to the player that the sequence is playing. At the end, LED4 turns off.

---

```c
/**
 * @brief Blinks the corresponding LEDs in sequence of the array for a certain length of it
 *
 * @param array to decide which LEDs to turn on
 * @param len how many array elements should be shown
 */
void playPuzzle(uint8_t *array, uint8_t len)
{
	lightDownAllLeds();
	lightUpOneLed(LED4);
	for (uint8_t i = 0; i < len; i++)
	{
		lightUpOneLed(array[i]);
		_delay_ms(PUZZLE_DELAY);
		lightDownOneLed(array[i]);
		_delay_ms(PUZZLE_DELAY / 10);
	}
	lightDownOneLed(LED4);
}
```

---

## Reading the input of the user

By far the most difficult aspect was to get this to work with the interrupt as well.

```c
/**
 * @brief Construct a new ISR object
 * Turns of all LEDs, saves the button that has been pressed into a global variable and sets a global flag to true
 *
 */
ISR(PCINT1_vect)
{
	lightDownAllLeds();
	button = whichButtonPushed();
	if (button < NUMBER_OF_BUTTONS)
	{
		lightUpOneLed(button);
		button_pushed = true;
	}
}
```

---

Let's break down what's happening here. In my Interrupt Service Routine, I first turn off all LEDs that might still be on. Then I set a global button variable to whichever button is pressed.

| Button  | Number |
| ------- | ------ |
| BUTTON1 | 0      |
| BUTTON2 | 1      |
| BUTTON3 | 2      |

That function will return 3 if no button is pressed. If a valid button number is returned though, I set the `button_pushed` variable to `true`;

---

```c
/**
 * @brief Traverses an array and checks via interrupts if the correct sequence has been entered.
 * BUTTON1 -> LED1, BUTTON2 -> LED2, BUTTON3 -> LED3
 *
 * @param array to check against the input
 * @param len how far to check
 * @return uint8_t
 */
uint8_t readInput(uint8_t *array, uint8_t len)
{
	uint8_t current = 0;
	while (current < len)
	{
		button_pushed = false;
		sei();
		while (!button_pushed)
		{
			_delay_us(1);
		}
		cli();
		if (button == array[current])
		{
			printf("Button #%d was correct!\n", button + 1);
			current++;
		}
		else
		{
			printf("Button #%d was incorrect!\n", button + 1);
			return false;
		}
	}
	return true;
}
```

Here I start by looping over the array with a while loop. I then turn the global interrupt system back on and loop as long as no button is pushed to keep the program stuck as long as no input is registered.  
Right after that I turn the interrupt system back off again.  
I then check if the button that was pressed is the same as the current sequence number and if so print a message to the serial monitor and increment the `current` variable.  
Otherwise I immediately return `false`.  
If it makes it to the end of the sequence as indicated by the `len` parameter, I return `true`.

---

```c
if (!readInput(sequence, i))
		{
			blinkAllLights(2);
			printf("Oops, you made a mistake. Try again.\n");
			i--;
		}
		_delay_ms(BETWEEN_DELAY);
```

---

In case the sequence wasn't entered correctly, I blink all LEDs twice to indicate an error and print a message to the serial monitor.

## Winning sequence

When the user gets through the entire sequence, I play the endSequence from the morse project and print a message to the serial monitor.
