#include <potentiometer.h>
#include <display.h>
#include <boolean.h>
#include <button-lib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <led-lib.h>
#include <avr/io.h>
#define UNDERSCORE 0xf7
#define DOT 0x7f
// #define DEBUG
#ifdef DEBUG
#include <usart.h>
#endif
#define WORD_LENGTH 4
#define ALPHABET_SIZE 26
#define POTENTIO_DEBOUNCE 4
#define abs(x) (x < 0 ? ~x + 1 : x)

uint8_t selected = 0;
int chosenWord[] = {-1, -1, -1, -1};
uint16_t previousADCValue = 0;
uint16_t currentASCII = 0;
char secret[] = "KILL";
uint8_t secretASCII[4];

void initialize()
{
#ifdef DEBUG
	initUSART();
#endif
	initADC();
	initPotentioInterrupts();
	initDisplay();
	enableAllButtons();
	enableAllLeds();
	getButtonsReadyForInterrupts();
}

void convertSecretIntoASCII()
{
	for (uint8_t i = 0; i < strlen(secret); i++)
	{
		secretASCII[i] = (uint8_t)secret[i];
	}
}

void confirm()
{
	for (uint8_t i = 0; i < strlen(secret); i++)
	{
		if (secretASCII[i] == chosenWord[i])
		{
			lightUpOneLed(i);
		}
		else
		{
			lightDownOneLed(i);
		}
	}
	return;
}

/**
 * @brief Construct a new ISR object
 */
ISR(PCINT1_vect)
{
	uint8_t button = whichButtonPushed();
	switch (button)
	{
	case BUTTON1:
		chosenWord[selected] = currentASCII;
		selected = (selected + WORD_LENGTH - 1) % WORD_LENGTH;
		break;

	case BUTTON2:
		chosenWord[selected] = currentASCII;
		confirm();
		break;

	case BUTTON3:
		chosenWord[selected] = currentASCII;
		selected = (selected + 1) % WORD_LENGTH;
		break;

	default:
		break;
	}
}

ISR(ADC_vect)
{
	uint16_t value = ADC;
	if (abs(value) > POTENTIO_DEBOUNCE)
	{
		currentASCII = ((value / POTENTIO_DEBOUNCE)) % ALPHABET_SIZE + 65;
#ifdef DEBUG
		printf("Value: %c\n", currentASCII);
#endif
		previousADCValue = value;
	}
}

int main()
{
	initialize();

	while (true)
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
	return 0;
}