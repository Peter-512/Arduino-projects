/**
 * @file basic-exercises-lib.h
 * @author Peter Buschenreiter
 * @brief
 * @version 0.1
 * @date 2022-04-23
 *
 * @copyright Copyright (c) 2022
 *
 * Write a program to light up the 4 LEDs of the shield consecutively
 * Write a program to make one of the LEDs light exactly 10 times
 * Turn exercise 2 into a function, where the parameters are the LED number and the number of flashes. (add the function to your library)
 * Write a program that makes an LED burn longer and longer (start with 10 ms and rise up to 1000 ms, incrementing with 50 ms in every iteration.
 * Write a program that uses an array of 10 digits between 0 and 4. Walk through the array and light up the corresponding led.
 * Write a program that creates an empty array and then fills it with the numbers from 10 to 1000 with a step value of 50 ms. Use this array to light up a led, just like in exercise 4.
 * Write a program that makes the leds light up based on a string. The string contains a sequence of the letters a, b, c and d. An 'a' lights the first LED, 'b' the second and so on.
 * Write a program that lights up the leds based on a string. But this time the string of letters is random. With the letter 'a' all 4 LEDs light up once, with a letter 'b' they light up twice, and so on.
 *
 */

void lightUpConsecutively(int interval);

void blinkNTimes(int lednumber, int n, int interval, int duration);

void lightBlinkIncremental(int lednumber);

void lightToggleByArray(int *lednumbers, int size);

int *fillArray(int *arr);

void toggleLedByString(char string[]);

void blinkAllLights(int amount);

void blinkLightsByString(char string[]);