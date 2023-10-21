#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "graphics.h"

int main(int argc, char* args[])
{
	bool success = init_graphics();
	if (!success)
	{
		return -1;
	}

	start_loop();

	return 0;
}
