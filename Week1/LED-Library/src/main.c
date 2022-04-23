#include <led-lib.h>
#include <util/delay.h>

int main(void)
{
	// enableOneLed(1);
	// lightUpOneLed(0b00001111);

	// _delay_ms(500);

	// enableOneLed(2);
	// lightToggleOneLed(2);

	// _delay_ms(500);

	// enableAllLeds();
	// lightUpAllLeds();

	// _delay_ms(500);

	// lightDownAllLeds();

	// enableMultipleLeds(0b00001001);
	// lightUpMultipleLeds(0b00001001);

	enableAllLeds();
	lightToggleAllLeds();

	_delay_ms(500);

	lightToggleAllLeds();
	_delay_ms(500);
	lightToggleAllLeds();
	_delay_ms(500);
	lightToggleAllLeds();

	return 0;
}