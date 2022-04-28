#include <led-lib.h>
#include <button-lib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <usart.h>

#define LED_PORT PORTB
#define BUTTON_PORT PORTC
#define BUTTON_PIN PINC
#define LED_DDR DDRB
#define BUTTON_DDR DDRC
#define BUTTON1 0
#define BUTTON2 1
#define BUTTON3 2
#define LED1 0
#define LED2 1
#define LED3 2
#define LED4 3
#define DEBUG

/* First, review the code in the main function!
 * This ISR is called when Pin Change Interrupt 1 is triggered (PCINT1_vect)
 * This is the interrupt for PORTC, where the switches/buttons are connected... */
ISR(PCINT1_vect)
{
	if (buttonPushed(BUTTON1))
	{
		lightToggleOneLed(LED1);
	}
	if (buttonPushed(BUTTON2))
	{
		lightToggleOneLed(LED2);
	}
	if (buttonPushed(BUTTON3))
	{
		lightToggleOneLed(LED3);
	}
}

int main(int argc, char const *argv[])
{
	enableAllLeds();
	enableAllButtons();
	getButtonsReadyForInterrupts();

	while (1)
	{
		lightUpOneLed(LED4); // turn led1 on
		_delay_ms(100);
		lightDownOneLed(LED4); // turn led1 off
		_delay_ms(100);
	}
	return 0;
}