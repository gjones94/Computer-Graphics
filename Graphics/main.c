#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "graphics.h"
#include "vector.h"
#include "cube.h"
#include "utils.h"
#include "config.h"

#define CONTROL_MOVEMENT .02f

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

	cube = create_cube(0, 0, 0, 2, .25);

	while (running)
	{
		process_input();

		draw(cube->vertices, cube->num_vertices);

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

			//X AXIS
			if (event.key.keysym.sym == SDLK_a)
			{
				rotate_cube(cube, CONTROL_MOVEMENT, X_AXIS);
			}

			if (event.key.keysym.sym == SDLK_z)
			{
				rotate_cube(cube, -CONTROL_MOVEMENT, X_AXIS);
			}

			//Y AXIS
			if (event.key.keysym.sym == SDLK_s)
			{
				rotate_cube(cube, CONTROL_MOVEMENT, Y_AXIS);
			}
			if (event.key.keysym.sym == SDLK_x)
			{
				rotate_cube(cube, -CONTROL_MOVEMENT, Y_AXIS);
			}

			//Z AXIS
			if (event.key.keysym.sym == SDLK_d)
			{
				rotate_cube(cube, CONTROL_MOVEMENT, Z_AXIS);
			}
			if (event.key.keysym.sym == SDLK_c)
			{
				rotate_cube(cube, -CONTROL_MOVEMENT, Z_AXIS);
			}

			break;
	}
}