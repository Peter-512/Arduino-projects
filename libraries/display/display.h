#include <avr/io.h>

#define NUMBER_OF_SEGMENTS 8
#define NUMBER_OF_SEGMENT_DISPLAYS 4
#define SEGMENT1 0
#define SEGMENT2 1
#define SEGMENT3 2
#define SEGMENT4 3

#define sbi(register, bit) (register |= _BV(bit))
#define cbi(register, bit) (register &= ~_BV(bit))

void initDisplay();
void writeNumberToSegment(uint8_t segment, uint8_t value);
void writeNumber(int number);
void writeNumberAndWait(int number, int delay);
void writeLetterToSegment(uint8_t segment, char letter);
void writeWord(char word[]);
void writeWordAndWait(char word[], int delay);
void writeLongWord(char word[], uint8_t len);
void writeSymbolToSegment(uint8_t segment, uint8_t hexcode);