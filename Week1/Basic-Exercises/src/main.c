#include <basic-exercises-lib.h>
#include <led-lib.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

int main(int argc, char const *argv[])
{
	// lightUpConsecutively(300);

	// lightUpNTimes(2, 5, 300, 600);

	// lightBlinkIncremental(3);

	// int lednumbers[] = {2, 3, 1, 0, 0, 0, 2, 3, 2, 3, 0, 2, 1, 1, 2, 3, 4, 1};
	// lightToggleByArray(lednumbers, sizeof(lednumbers) / sizeof(*lednumbers));

	// int size = 20;
	// int arr[size];
	// int *durations = fillArray(arr);
	// int lednumber = 2;
	// enableOneLed(lednumber);
	// for (int i = 0; i < size; i++)
	// {
	// 	lightUpOneLed(lednumber);
	// 	_delay_ms(durations[i]);
	// 	lightDownOneLed(lednumber);
	// 	_delay_ms(400);
	// }

	// lightUpByString("abcd");
	// lightUpByString("heciqhewwdckjq");

	// blinkAllLights(3);

	// blinkLightsByString("abcd");
	// blinkLightsByString("qhcbehbuoqhdbuw");

	return 0;
}
