# 4-Letter Word Quiz

I wanted to create the 4-Letter Word Quiz with all the features from ArduinoWordle but without the limitations of the SerialCommunication.

Unfortunately, I didn't have enough time to fix a bug I ran into that made it impossible to set the letters using the potentiometer in the same way I had done in ArduinoWordle.  
A lot of the ISRs are similar or identical so I will just quickly go over additional functions.

## Logging

```c
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

void saveAttempt()
{
	char *string = malloc(5);
	convert(currentGuessASCII, string);

	strcpy(puzzle.attempts[attempts].guess, string);
	int totalSeconds = seconds + minutes * SECONDS_IN_MINUTE;
	puzzle.attempts[attempts].time = totalSeconds;
}
```

I created a puzzle struct which holds an attempts pointer which is a struct itself, which holds that guessed word and the time that has elapsed since the beginning of the game (in seconds).

## Interrupts

Additionally to the interrupts in ArduinoWordle, I also have timer running to keep track of the time the game is running, which I start once the category was chosen.

```c
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
```

As long as I'm still chosoing a category, I'm also incrementing a seed variable.
