#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "graphics.h"
#include "vector.h"
#include "cube.h"
#include "utils.h"
#include "config.h"
#include "mesh.h"

#define CONTROL_MOVEMENT .03f

//=========================================================
// FUNCTION PROTOTYPES
//=========================================================
void process_input(void);

//=========================================================
// VARIABLES
//=========================================================
cube3d_t* cube;

int main(int argc, char* args[])
{
	running = init_graphics();
	if (!running)
	{
		return -1;
	}

	load_mesh_from_file("f22.obj");

	while (running)
	{
		process_input();

		update();

		render();
	}

	free_resources();

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

			//X AXIS
			if (event.key.keysym.sym == SDLK_d)
			{
				rotate_mesh((float) CONTROL_MOVEMENT, X_AXIS);
			}

			if (event.key.keysym.sym == SDLK_a)
			{
				rotate_mesh((float) - CONTROL_MOVEMENT, X_AXIS);
			}

			//Y AXIS
			if (event.key.keysym.sym == SDLK_LEFT)
			{
				rotate_mesh((float) CONTROL_MOVEMENT, Y_AXIS);
			}
			if (event.key.keysym.sym == SDLK_RIGHT)
			{
				rotate_mesh((float) - CONTROL_MOVEMENT, Y_AXIS);
			}

			//Z AXIS
			if (event.key.keysym.sym == SDLK_DOWN)
			{
				rotate_mesh((float) CONTROL_MOVEMENT, Z_AXIS);
			}
			if (event.key.keysym.sym == SDLK_UP)
			{
				rotate_mesh((float) - CONTROL_MOVEMENT, Z_AXIS);
			}

			break;
	}
}