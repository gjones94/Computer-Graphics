#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "graphics.h"
#include "vector.h"
#include "cube.h"
#include "utils.h"
#include "config.h"

#define BUFFER_SIZE 200

//=========================================================
// FUNCTION PROTOTYPES
//=========================================================
void process_input(void);
void update(void);

//=========================================================
// VARIABLES
//=========================================================
cube3d_t* cube;
char* logger;

int main(int argc, char* args[])
{

	running = init_graphics();
	if (!running)
	{
		return -1;
	}

	while (running)
	{
		process_input();

		update();

		render();
	}

	return 0;
}

void process_input(void)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		running = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
		{
			running = false;
		}
		break;
	}
}

