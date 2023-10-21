#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "graphics.h"
#include "vector.h"
#include "cube.h"

//Function Prototypes
void process_input(void);
void update(void);

cube3d_t* cube;

int main(int argc, char* args[])
{
	cube = create_cube(0, 0, 0, 2, .10);
	print_vertices(cube->vertices, cube->num_vertices);

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

void update(void)
{

}