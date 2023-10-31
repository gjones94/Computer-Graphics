#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "graphics.h"
#include "vector.h"
#include "colors.h"
#include "mesh.h"

#define CONTROL_MOVEMENT .03f

//=========================================================
// FUNCTION PROTOTYPES
//=========================================================
void process_input(void);

//=========================================================
// VARIABLES
//=========================================================

int main(int argc, char* args[])
{
	running = init_graphics();

	if (!running)
	{
		return -1;
	}

	bool success = load_mesh_from_file("f22.obj");

	if (success)
	{
		while (running)
		{
			process_input();

			update();

			render();
		}
	}

	free_resources();

	return 0;
}

void process_input(void)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	mesh_t* mesh = meshes[0];

	switch (event.type)
	{
		case SDL_QUIT:
			running = false;
			break;
		case SDL_KEYDOWN:
			
			if (event.key.keysym.sym == SDLK_w)
			{
				printf("Wireframe enabled\n");
				wireframe_enabled = true;
			}
			else if (event.key.keysym.sym == SDLK_q)
			{
				printf("Wireframe disabled\n");
				wireframe_enabled = false;
			}

			if (event.key.keysym.sym == SDLK_f)
			{
				printf("fill enabled\n");
				fill_enabled = true;
			}
			else if (event.key.keysym.sym == SDLK_d)
			{
				printf("fill disabled\n");
				fill_enabled = false;
			}

			if (event.key.keysym.sym == SDLK_c)
			{
				printf("culling enabled\n");
				backface_culling_enabled = true;
			}
			else if (event.key.keysym.sym == SDLK_x)
			{
				printf("culling disabled\n");
				backface_culling_enabled = false;
			}

			if (event.key.keysym.sym == SDLK_n)
			{
				printf("normal enabled\n");
				normal_enabled = true;
			}
			else if (event.key.keysym.sym == SDLK_b)
			{
				printf("normal disabled\n");
				normal_enabled = false;
			}

			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				running = false;
			}

			////X AXIS
			//if (event.key.keysym.sym == SDLK_d)
			//{
			//	rotate_mesh(mesh, (float) CONTROL_MOVEMENT, X_AXIS);
			//}

			//if (event.key.keysym.sym == SDLK_a)
			//{
			//	rotate_mesh(mesh, (float) - CONTROL_MOVEMENT, X_AXIS);
			//}

			////Y AXIS
			//if (event.key.keysym.sym == SDLK_LEFT)
			//{
			//	rotate_mesh(mesh, (float) CONTROL_MOVEMENT, Y_AXIS);
			//}
			//if (event.key.keysym.sym == SDLK_RIGHT)
			//{
			//	rotate_mesh(mesh, (float) - CONTROL_MOVEMENT, Y_AXIS);
			//}

			////Z AXIS
			//if (event.key.keysym.sym == SDLK_DOWN)
			//{
			//	rotate_mesh(mesh, (float) CONTROL_MOVEMENT, Z_AXIS);
			//}
			//if (event.key.keysym.sym == SDLK_UP)
			//{
			//	rotate_mesh(mesh, (float) - CONTROL_MOVEMENT, Z_AXIS);
			//}

			break;
	}
}