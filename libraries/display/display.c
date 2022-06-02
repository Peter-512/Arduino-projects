#include <avr/io.h>
#include <display.h>
#include <util/delay.h>
#include <ctype.h>
#define BLANK 0xff
#define LOW 0
#define HIGH 1
/* Define shift register pins used for seven segment display */
#define LATCH_DIO PD4
#define CLK_DIO PD7
#define DATA_DIO PB0

#define LSBFIRST 0
#define MSBFIRST 1
// #define DEBUG
#ifdef DEBUG
#include <usart.h>
#endif

/* Segment byte maps for numbers 0 to 9 */
const uint8_t SEGMENT_MAP[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99,
							   0x92, 0x82, 0xF8, 0X80, 0X90};

// * Segment byte maps for characters A-Z
const uint8_t CHAR_MAP[] = {0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e, 0xc2,
							0x8b, 0xcf, 0xe1, 0x8a, 0xc7, 0xea, 0xc8,
							0xc0, 0x8c, 0x98, 0xcc, 0x92, 0x87, 0xc1,
							0xd1, 0xd5, 0x89, 0x91, 0xb4};

/* Byte maps to select digit 1 to 4 */
const uint8_t SEGMENT_SELECT[] = {0xF1, 0xF2, 0xF4, 0xF8};

void initDisplay()
{
	sbi(DDRD, LATCH_DIO);
	sbi(DDRD, CLK_DIO);
	sbi(DDRB, DATA_DIO);
}

// loop through seven segments of LED display and shift the correct bits in the
// data register
void shift(uint8_t val, uint8_t bitorder)
{
	uint8_t bit;

	for (uint8_t i = 0; i < NUMBER_OF_SEGMENTS; i++)
	{
		if (bitorder == LSBFIRST)
		{
			bit = !!(val & _BV(i));
		}
		else
		{
			bit = !!(val & _BV(7 - i));
		}
		// write bit to register
		if (bit == HIGH)
			sbi(PORTB, DATA_DIO);
		else
			cbi(PORTB, DATA_DIO);

		// Trigger the clock pin so the display updates
		sbi(PORTD, CLK_DIO);
		cbi(PORTD, CLK_DIO);
	}
}

void writeSymbolToSegment(uint8_t segment, uint8_t hexcode)
{
	cbi(PORTD, LATCH_DIO);
	shift(hexcode, MSBFIRST);
	shift(SEGMENT_SELECT[segment], MSBFIRST);
	sbi(PORTD, LATCH_DIO);
}

void writeLetterToSegment(uint8_t segment, char letter)
{
	letter = toupper(letter);
#ifdef DEBUG
	initUSART();
	printf("Letter: %c", letter);
	putchar('\n');
#endif
	uint8_t ascii = (uint8_t)letter;
#ifdef DEBUG
	initUSART();
	printf("Letter in ascii code: %d", (int)ascii);
	putchar('\n');
#endif
	ascii -= 65;
	writeSymbolToSegment(segment, isalpha(letter) ? CHAR_MAP[ascii] : BLANK);
}

void writeWord(char word[])
{
	for (uint8_t i = 0; i < NUMBER_OF_SEGMENT_DISPLAYS; i++)
	{
		writeLetterToSegment(i, word[i]);
	}
}

void clearDisplay()
{
	for (uint8_t i = 0; i < NUMBER_OF_SEGMENTS; i++)
	{
		writeSymbolToSegment(i, BLANK);
	}
}

void writeWordAndWait(char word[], int delay)
{
	for (int i = 0; i < delay; i++)
	{
		writeLetterToSegment(i % NUMBER_OF_SEGMENT_DISPLAYS, word[i % NUMBER_OF_SEGMENT_DISPLAYS]);
		_delay_ms(1);
	}
	clearDisplay();
}

void writeLongWord(char word[], uint8_t len)
{
	uint8_t i = 0;
	while (i < len)
	{
		for (uint8_t j = 0; i == 0 ? j < 250 : j < 75; j++)
		{
			for (uint8_t k = 0; k < NUMBER_OF_SEGMENT_DISPLAYS; k++)
			{
				writeLetterToSegment(k, (k + i < len) ? word[k + i] : BLANK);
				_delay_ms(1);
			}
		}
		i++;
	}
}

// Writes a digit to a certain segment. Segment 0 is the leftmost.
void writeNumberToSegment(uint8_t segment, uint8_t value)
{
	writeSymbolToSegment(segment, SEGMENT_MAP[value]);
}

// Writes a nuber between 0 and 9999 to the display. To be used in a loop...
void writeNumber(int number)
{
	if (number < 0)
	{
		writeNumberToSegment(SEGMENT1, 0);
		return;
	}
	if (number > 9999)
	{
		writeLetterToSegment(SEGMENT3, 'o');
		writeLetterToSegment(SEGMENT4, 'f');
		return;
	}
	writeNumberToSegment(SEGMENT1, number / 1000);
	writeNumberToSegment(SEGMENT2, (number / 100) % 10);
	writeNumberToSegment(SEGMENT3, (number / 10) % 10);
	writeNumberToSegment(SEGMENT4, number % 10);
}

// Writes a number between 0 and 9999 to the display and makes sure that it stays there a certain number of millisecs.
// Note: the timing is approximate; the amount of time writeNumberToSegment takes is not accounted for...
void writeNumberAndWait(int number, int delay)
{
	for (int i = 0; i < delay / 4; i++)
	{
		if (number < 0)
		{
			writeNumberToSegment(SEGMENT1, 0);
			_delay_ms(4);
		}
		else if (number > 9999)
		{
			writeLetterToSegment(SEGMENT3, 'o');
			writeLetterToSegment(SEGMENT4, 'f');
			_delay_ms(2);
		}
		else
		{
			writeNumberToSegment(SEGMENT1, number / 1000);
			_delay_ms(1);
			writeNumberToSegment(SEGMENT2, (number / 100) % 10);
			_delay_ms(1);
			writeNumberToSegment(SEGMENT3, (number / 10) % 10);
			_delay_ms(1);
			writeNumberToSegment(SEGMENT4, number % 10);
			_delay_ms(1);
		}
	}
	clearDisplay();
}
