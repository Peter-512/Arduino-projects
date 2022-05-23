#include <avr/io.h>

void initADC()
{
    ADMUX |= _BV(REFS0);
    ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
    ADCSRA |= _BV(ADEN);
}

void initPotentioInterrupts()
{
    ADCSRA |= _BV(ADIE);
}

uint16_t getPotentiometerValue()
{
    ADCSRA |= _BV(ADSC);
    loop_until_bit_is_clear(ADCSRA, ADSC);
    return ADC;
}
