# Morse (Trainer) Project

## Morse

Morse is defined by a combination of long and short signals so I started by creating two functions, one for a "dit" (short signal) and one for a "dah" (long signal).

---

```c
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
```

---

```c
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
```

---

I then wrote the `morseChar(char character, int charCounter)` function that takes a single character (as well as an integer which is important for the morse trainer as we'll se later on).  
This function casts the character to an integer so we can get its ASCII value as well as checks if its an uppercase letter (>= 97) and makes it lowercase by subtracting 32.  
A massive switch statement then executes the necessary functions according to the morse alphabet along with an intra-character space.  
If no case applies the aformentioned integer will be decremented.  
At the end, it returns the counter after incrementing it.

---

```c
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
.
.
.
```

---

Finally we can call the `morse(char string[])` method, which loops through each character in the string that it gets and calls the `morseChar()` for each character.

---

```c
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
```

## Morse Trainer

To train your morse proficiency, i wrote the `morseTrainer()` function, which sets the random seed, initializes USART for serial monitor communication and then calls a few self made functions.  
First it calls the `countdown()` method to let the user know to get ready. After that it initializes a counter variable to 0 and starts a loop that countinues while that counter is smaller than 10.  
It creates a randomized number between 32 and 90 (both inclusive) and casts it to a character.  
It then calls the `morseChar()` method from earlier to display the character using the LEDs and sets the counter variable to its returning value.  
After that it waits for 2 seconds before printing the solution to the serial monitor and continuing with the next character.  
Once 10 characters have been tested, it plays the `endSequence()`.

---

```c
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
```

---

```c
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
```

---

```c
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
```
