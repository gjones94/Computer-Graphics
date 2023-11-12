#include <SDL.h>
#include <stdio.h>
#include "utilities.h"

float get_angle_radians(float degrees)
{
	return (float) (degrees) * (float) (M_PI / 180.0f);
}

void print_bits(uint32_t byte_4)
{
	int bit_counter = 0;
	for (unsigned int i = 1 << 31; i > 0; i = i >> 1)
	{
		if (bit_counter % 8 == 0 && bit_counter != 0)
		{
			printf(" ");
		}

		if (i & byte_4)
		{
			printf("1");
		}
		else 
		{
			printf("0");
		}
		bit_counter++;
	}
	printf("\n");
}
