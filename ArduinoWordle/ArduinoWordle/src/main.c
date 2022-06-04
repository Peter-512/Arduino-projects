#include <potentiometer.h>
#include <display.h>
#include <boolean.h>
#include <button-lib.h>
#include <avr/interrupt.h>
#include <led-lib.h>
#include <serial-lib.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <buzzer.h>
#include <avr/io.h>
#include <usart.h>
#define UNDERSCORE 0xf7
#define DOT 0x7f
#define ALPHABET_SIZE 26
#define POTENTIO_DEBOUNCE 2
#define WORD_LENGTH 4

uint8_t selected = 0;
int chosenWord[] = {-1, -1, -1, -1};
int16_t previousADCValue = 0;
uint8_t currentASCII;
uint8_t wordIndex = 0;
int guessCounter;

void initialize()
{
	initUSART();
	initADC();
	initPotentioInterrupts();
	initDisplay();
	enableAllButtons();
	enableAllLeds();
	initSerial();
	enableBuzzer();
	getButtonsReadyForInterrupts();
}

void convertAsciiToChar(char *string)
{
	for (int i = 0; i < WORD_LENGTH; i++)
	{
		string[i] = (char)chosenWord[i];
	}
}

void confirm()
{
	char *string = calloc(WORD_LENGTH + 1, sizeof(char));
	convertAsciiToChar(string);
	printf("%s", string);
	return;
}

ISR(USART_RX_vect) // The interrupt routine for receiving data
{
	uint8_t byte = UDR0; // serial data is located in the UDR0 register
	wordIndex++;
}

ISR(PCINT1_vect)
{
	uint8_t button = whichButtonPushed();
	switch (button)
	{
	case BUTTON1:
		chosenWord[selected] = currentASCII;
		selected = (selected + WORD_LENGTH - 1) % WORD_LENGTH;
		if (chosenWord[selected] == -1)
		{
			currentASCII = 65;
		}
		else
		{
			currentASCII = chosenWord[selected];
		}
		break;

	case BUTTON2:
		chosenWord[selected] = currentASCII;
		guessCounter++;
		confirm();
		break;

	case BUTTON3:
		chosenWord[selected] = currentASCII;
		selected = (selected + 1) % WORD_LENGTH;
		if (chosenWord[selected] == -1)
		{
			currentASCII = 65;
		}
		else
		{
			currentASCII = chosenWord[selected];
		}
		break;

	default:
		break;
	}
}

ISR(ADC_vect)
{
	int16_t value = ADC;
	if ((value - previousADCValue) > POTENTIO_DEBOUNCE)
	{
		currentASCII = (currentASCII + 1 - 65) % ALPHABET_SIZE + 65;
		previousADCValue = value;
	}
	else if ((value - previousADCValue) < -POTENTIO_DEBOUNCE)
	{
		currentASCII = (currentASCII + ALPHABET_SIZE - 1 - 65) % ALPHABET_SIZE + 65;
		previousADCValue = value;
	}
}

int main()
{
	initialize();
	guessCounter = 0;

	while (wordIndex <= WORD_LENGTH - 1)
	{
		for (int i = 0; i < NUMBER_OF_LEDS; i++)
		{
			lightToggleOneLed(i);
			_delay_ms(100);
		}
	}

	lightDownAllLeds();

	currentASCII = 64;
	while (wordIndex < WORD_LENGTH + 6)
	{
		for (int i = 0; i < NUMBER_OF_SEGMENT_DISPLAYS; i++)
		{
			if (i != selected)
			{
				if (chosenWord[i] < 0)
				{
					writeSymbolToSegment(i, UNDERSCORE);
				}
				else
				{
					writeLetterToSegment(i, (char)(chosenWord[i]));
				}
			}
			else
			{
				writeLetterToSegment(selected, (char)currentASCII);
				writeSymbolToSegment(i, DOT);
			}
		}
		ADCSRA |= _BV(ADSC);
	}
	printf("%d", guessCounter);

	playTone(30, 100);
	_delay_ms(100);
	playTone(30, 100);
	_delay_ms(100);
	playTone(30, 100);
	_delay_ms(100);
	playTone(50, 100);
	_delay_ms(500);
	playTone(30, 100);
	_delay_ms(100);
	playTone(50, 1000);

	return 0;
}