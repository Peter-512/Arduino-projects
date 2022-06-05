#include <potentiometer.h>
#include <display.h>
#include <boolean.h>
#include <button-lib.h>
#include <avr/interrupt.h>
#include <led-lib.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <buzzer.h>
#include <avr/io.h>
#include <ctype.h>
#include <usart.h>
#define UNDERSCORE 0xf7
#define DOT 0x7f
#define ALPHABET_SIZE 26
#define SECONDS_IN_MINUTE 60
#define POTENTIO_DEBOUNCE 4
#define WORD_LENGTH 4
#define MULTIPLE 250
#define SIZE(x) (sizeof(x))
#define LEN(x) (SIZE(x) / SIZE(x[0]))
#define NEW_SIZE(type, len) ((len + 1) * SIZE(type))
#define DEBUG

char *animals[] = {"goat", "frog", "chic", "crab", "duck", "fish", "foal", "lamb", "lion", "mule", "worm", "tuna", "seal", "swan", "toad", "orca", "kiwi", "wren", "crow", "gull"};
char *names[] = {"Abel", "Adan", "Aden", "Alan", "Aldo", "Alec", "Alex", "Amir", "Amos", "Andy", "Ares", "Arlo", "Aron", "Axel", "Ayan", "Beau", "Ivan", "Luka", "Lyam", "Lyle", "Mack", "Marc", "Mark", "Mike", "Milo", "Paul"};
char *countries[] = {"Chad", "Cuba", "Fiji", "Iran", "Iraq", "Laos", "Mali", "Niue", "Oman", "Peru", "Togo"};
char *categories[] = {"ANML", "NAME", "CTRY"};

volatile bool chooseCategory;
uint8_t currentCategory = 0;
uint16_t seed = 0;
uint8_t timerCounter = 0;
uint8_t seconds = 0;
uint8_t minutes = 0;
uint8_t selectedSegment = 4;
volatile bool isCorrect = false;
uint8_t attempts = 0;
uint16_t previousADCValue;
int currentGuessASCII[] = {95, 95, 95, 95};
int currentLetterASCII;

typedef struct
{
	char guess[5]; // guessed word
	uint16_t time; // elapsed time since start of the game
} ATTEMPT;

typedef struct
{
	char *category;		// chosen category
	char word[5];		// secret word
	ATTEMPT *attempts;	// all attempts
	uint16_t totalTime; // in seconds
} PUZZLE;

PUZZLE puzzle;

void initTimer()
{
	TCCR2A |= _BV(WGM21);
	TIMSK2 |= _BV(OCIE0A);
	OCR2A = 249;
	sei();
	TCCR2B |= _BV(CS22) | _BV(CS21);
}

void tick()
{
	seconds++;
	seconds %= SECONDS_IN_MINUTE;

	if (!seconds)
	{
		minutes++;
	}
}

int isVowel(int ch)
{
	int c = toupper(ch);

	return (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');
}

void prepareCurrentGuess()
{
#ifdef DEBUG
	printf("%s", "[");
#endif
	for (size_t i = 0; i < strlen(puzzle.word); i++)
	{
		currentGuessASCII[i] = isVowel((int)puzzle.word[i]) ? toupper((int)puzzle.word[i]) : 95;
#ifdef DEBUG
		printf("%d, ", currentGuessASCII[i]);
#endif
	}
#ifdef DEBUG
	printf("%s", "\b\b]\n");
	printf("%d\n", chooseCategory);
#endif
}

void toUpperString(char *string)
{
	while (*string)
	{
		*string = toupper((unsigned char)*string);
		string++;
	}
}

void selectCategory()
{
	chooseCategory = false;
	srand(seed);
	char *string = calloc(5, SIZE(char));

	switch (currentCategory)
	{
	case 0: // category ANIMAL
		puzzle.category = malloc(SIZE("ANIMAL"));
		strcpy(puzzle.category, "ANIMAL");
		strcpy(string, animals[rand() % LEN(animals)]);
		toUpperString(string);
		strcpy(puzzle.word, string);
#ifdef DEBUG
		printf("%s\n", puzzle.word);
#endif
		break;

	case 1: // category NAME
		puzzle.category = malloc(SIZE("NAME"));
		strcpy(puzzle.category, "NAME");
		strcpy(string, names[rand() % LEN(names)]);
		toUpperString(string);
		strcpy(puzzle.word, string);
#ifdef DEBUG
		printf("%s\n", puzzle.word);
#endif
		break;

	case 2: // category COUNTRY
		puzzle.category = malloc(SIZE("COUNTRY"));
		strcpy(puzzle.category, "COUNTRY");
		strcpy(string, countries[rand() % LEN(countries)]);
		toUpperString(string);
		strcpy(puzzle.word, string);
#ifdef DEBUG
		printf("%s\n", puzzle.word);
#endif
		break;

	default:
		break;
	}
}

void convert(int array[], char string[])
{
	for (int i = 0; i < 4; i++)
	{
		string[i] = (char)array[i];
	}
}

void saveAttempt()
{
	char *string = malloc(5);
	convert(currentGuessASCII, string); // ! DEBUG

	strcpy(puzzle.attempts[attempts].guess, string);
	int totalSeconds = seconds + minutes * SECONDS_IN_MINUTE;
	puzzle.attempts[attempts].time = totalSeconds;
}

void confirmGuess()
{
	// save guess and time into attempt
	attempts++;

	puzzle.attempts = realloc(puzzle.attempts, NEW_SIZE(ATTEMPT, attempts));

	if (puzzle.attempts)
	{
		saveAttempt();
	}
	// TODO check if guess was correct
}

// This ISR runs every 4 ms
ISR(TIMER2_COMPA_vect)
{
	if (chooseCategory)
	{
		seed++;
	}

	timerCounter++;
	timerCounter %= MULTIPLE;

	if (!timerCounter) // one second has passed
	{
		tick();
	}
}

ISR(ADC_vect)
{
	int16_t value = ADC;
	if ((value - previousADCValue) < POTENTIO_DEBOUNCE)
	{
		currentLetterASCII = (currentLetterASCII + 1 - 65) % ALPHABET_SIZE + 65;
		previousADCValue = value;
	}
	else if ((value - previousADCValue) > (-POTENTIO_DEBOUNCE))
	{
		currentLetterASCII = (currentLetterASCII + ALPHABET_SIZE - 1 - 65) % ALPHABET_SIZE + 65;
		previousADCValue = value;
	}
}

ISR(PCINT1_vect)
{
	uint8_t button = whichButtonPushed();
	if (chooseCategory)
	{
		switch (button)
		{
		case BUTTON1:
			currentCategory = (currentCategory + LEN(categories) - 1) % LEN(categories);
			break;

		case BUTTON2:
			selectCategory();
			prepareCurrentGuess();
			break;

		case BUTTON3:
			currentCategory = (currentCategory + 1) % LEN(categories);
			break;

		default:
			break;
		}
	}
	else
	{
		switch (button)
		{
		case BUTTON1:
			if (selectedSegment == 4)
			{
				selectedSegment = 0; // TODO find first _
			}
			else
			{
				currentGuessASCII[selectedSegment] = currentLetterASCII;
				selectedSegment = ((selectedSegment + WORD_LENGTH) - 1) % WORD_LENGTH;
			}
			if (currentLetterASCII == 95)
			{
				currentLetterASCII = 65;
			}
			else
			{
				currentLetterASCII = currentGuessASCII[selectedSegment];
			}
			break;

		case BUTTON2:
			confirmGuess();
			break;

		case BUTTON3:
			if (selectedSegment == 4)
			{
				selectedSegment = 0; // TODO find last _
			}
			else
			{
				currentGuessASCII[selectedSegment] = currentLetterASCII;
				selectedSegment = (selectedSegment + 1) % WORD_LENGTH;
			}
			if (currentLetterASCII == 95)
			{
				currentLetterASCII = 65;
			}
			else
			{
				currentLetterASCII = currentGuessASCII[selectedSegment];
			}
			break;

		default:
			break;
		}
	}
}

void initialize()
{
	initUSART();
	initADC();
	initPotentioInterrupts();
	initDisplay();
	enableAllLeds();
	enableBuzzer();
	initTimer();
	enableAllButtons();
	getButtonsReadyForInterrupts();
}

int main(int argc, char const *argv[])
{
	initialize();

	puzzle.attempts = malloc(SIZE(ATTEMPT));

#ifndef DEBUG
	writeLongWord("Choose a category", 18);
#endif

	chooseCategory = true;
	while (chooseCategory)
	{
		writeWord(categories[currentCategory]);
	}

#ifdef DEBUG
	printf("Category: %s\nWord: %s\n", puzzle.category, puzzle.word);
#endif

	previousADCValue = getPotentiometerValue();
	currentLetterASCII = 95;
	while (!isCorrect)
	{
		for (size_t i = 0; i < NUMBER_OF_SEGMENT_DISPLAYS; i++)
		{
			if (i == selectedSegment)
			{
				writeLetterToSegment(i, (char)currentLetterASCII);
				writeSymbolToSegment(i, DOT);
			}
			else
			{
				if (currentGuessASCII[i] == 95)
				{
					writeSymbolToSegment(i, UNDERSCORE);
				}
				else
				{
					writeLetterToSegment(i, currentGuessASCII[i]);
				}
			}
		}
		ADCSRA |= _BV(ADSC);
	}

	return 0;
}