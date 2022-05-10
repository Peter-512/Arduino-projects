#include <avr/io.h>

void initTimer0()
{
    // STEP 1: choose the WAVE FORM and by consequence the Mode of Operation
    // We choose FAST PWM; TCNT0 counts to 255
    TCCR0A |= _BV(WGM00) | _BV(WGM01); // WGM00 = 1 and WGM01 = 1 --> Fast PWM Mode

    // STEP 2: *always* set a PRESCALER, otherwise the timer won't count
    // The counting speed is determined by the CPU clock (16 Mhz) divided by this factor
    TCCR0B |= _BV(CS02) | _BV(CS00); // CS02 = 1 and CS00 = 1 --> prescaler factor is now 1024 (= every 64 us)

    // STEP 3: enable INTERRUPTS
    // Enable interrupts for 2 cases: TCNT0 == TOP and TCNT0 == OCR0A
    TIMSK0 |= _BV(TOIE0);  // enable overflow interrupt
    TIMSK0 |= _BV(OCIE0A); // enable OCRA interrupt

    sei(); // enable interrupts globally
}