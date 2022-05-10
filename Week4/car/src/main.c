#include <avr/io.h>
#include <stdlib.h>
#include <usart.h>

int main(int argc, char const *argv[])
{
	initUSART();

	char makes[3][10] = {"Tesla", "BMW", "Ford"};
	char models[3][10] = {"3", "X9", "Mustang"};
	uint16_t years[3] = {2020, 2025, 1969};

	typedef struct
	{
		char make[10];
		char model[10];
		uint16_t year;
	} Car;

	Car *cars[3];

	for (uint8_t i = 0; i < 3; i++)
	{
		cars[i] = malloc(sizeof(Car));
		strcpy((*cars[i]).make, makes[i]);
		strcpy((*cars[i]).model, models[i]);
		(*cars[i]).year = years[i];

		printf("Make: %s\n", (*cars[i]).make);
		printf("Model: %s\n", (*cars[i]).model);
		printf("Year: %d\n", (*cars[i]).year);
	}

	printf("Size of cars pointer: %d", sizeof cars * sizeof (Car));

	return 0;
}
