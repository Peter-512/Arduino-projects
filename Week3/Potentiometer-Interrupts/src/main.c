#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/interrupt.h>
#include <avr/io.h>
#include <led-lib.h>
#include <stdio.h>
#include <usart.h>
#include <util/delay.h>

void initADC()
{
	ADMUX |= _BV(REFS0);
	ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
	ADCSRA |= _BV(ADEN);
	ADCSRA |= _BV(ADIE);
}
uint16_t previousADCValue = 400;
ISR(ADC_vect)
{
	uint16_t value = ADC;
	if ((previousADCValue - value) * (previousADCValue - value) > 4)
	{
		previousADCValue = value;
		printf("Value:%d\n", ((int)(value / 4) % 26));
		if (value > 1000)
		{
			lightUpOneLed(LED1);
		}
		else
		{
			lightDownOneLed(LED1);
		}
	}
}

int main()
{
	initUSART();
	initADC();
	enableAllLeds();
	lightDownAllLeds();
	sei();
	while (1)
	{
		lightUpOneLed(LED3);
		_delay_ms(50);
		lightDownOneLed(LED3);
		_delay_ms(50);
		ADCSRA |= _BV(ADSC); // start first ADC  }
	}
	return 0;
}