#include <stdio.h>
#include "graphics.h"
#include "config.h"
#include "vector.h"
#include "cube.h"
#include "utils.h"
#include "vector.h"

//=========================================================
// SDL INITIALIZATION
//=========================================================
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* buffer_texture = NULL;
uint32_t* buffer = NULL;

//=========================================================
// GRAPHICS INITIALIZATION
//=========================================================
cube3d_t* cube = NULL;
vec3d_t camera_position; 
float angle = 1.0f;
int originX = 0;
int originY = 0;
bool running = false;

//=========================================================
// PRIVATE FUNCTION PROTOTYPES
//=========================================================
static void set_dimensions();
static bool init_window();
static bool init_buffers(void);
static void init_objects();
static void init_3d();

//=========================================================
// PUBLIC FUNCTIONS
//=========================================================
bool init_graphics()
{
	set_dimensions();

	bool success = init_window();
	if (success)
	{
		success = init_buffers();
	}

	init_objects();
	init_3d();

	return success;
}

void render()
{
	//draw_grid(10, GREY);

	render_texture();
	clear_buffer(0xFF000000);

	SDL_RenderPresent(renderer);
}

void render_texture()
{
	SDL_UpdateTexture(buffer_texture, NULL, buffer, ((int)sizeof(uint32_t) * WINDOW_WIDTH));
	SDL_RenderCopy(renderer, buffer_texture, NULL, NULL);
}

void clear_buffer(uint32_t color)
{
	for (int i = 0; i < WINDOW_HEIGHT; i++)
	{
		for (int j = 0; j < WINDOW_WIDTH; j++)
		{
			buffer[(i * WINDOW_WIDTH) + j] = color;
		}
	}
}

void free_resources()
{
	free(buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void draw_grid(unsigned int spacing, uint32_t grid_color)
{
	for (int y = 0; y < WINDOW_HEIGHT; y++)
	{
		for (int x = 0; x < WINDOW_WIDTH; x++)
		{
			int pixel = (y * WINDOW_WIDTH) + x;
			if (x % spacing == 0 || y % spacing == 0)
			{
				buffer[pixel] = grid_color;
			}
		}
	}
}

void draw_pixel(int x, int y, uint32_t color)
{
	if (x > 0 && x < WINDOW_WIDTH && y > 0 && y < WINDOW_HEIGHT)
	{
		int pixel_index = (WINDOW_WIDTH * y) + x;
		buffer[pixel_index] = color;
	}
}

void draw_rect(int x, int y, int width, int height, uint32_t color)
{
	for (int i = y; i < y + height; i++)
	{
		for (int j = x; j < x + width; j++)
		{
			draw_pixel(j, i, color);
		}
	}
}

vec3d_t get_projection(vec3d_t vector)
{
	vec3d_t projected =
	{
		.x = (scale(vector.x, FOV_SCALE) / vector.z) + originX,
		.y = (scale(vector.y, FOV_SCALE) / vector.z) + originY,
		.z = vector.z
	};

	return projected;
}

vec3d_t rotate(vec3d_t vector, float angle, Axis axis)
{
	float x = vector.x;
	float y = vector.y;
	float z = vector.z;

	vec3d_t vectorRotated = { .x = x, .y = y, .z = z };

	switch (axis)
	{
		case X_AXIS:
			vectorRotated.y = (y * cos(angle)) - (z * sin(angle));
			vectorRotated.z = (z * cos(angle)) + (y * sin(angle));
			break;
		case Y_AXIS:
			vectorRotated.x = (x * cos(angle)) - (z * sin(angle));
			vectorRotated.z = (z * cos(angle)) + (x * sin(angle));
			break;
		case Z_AXIS:
			vectorRotated.x = (x * cos(angle)) - (y * sin(angle));
			vectorRotated.y = (y * cos(angle)) + (x * sin(angle));
			break;
	}

	return vectorRotated;
}

void update(void)
{
	SDL_Delay(15);
	angle += .01;

	if (angle == 360)
	{
		angle = 0;
	}

	for (int i = 0; i < cube->num_vertices; i++)
	{
		vec3d_t vec3 = cube->vertices[i];

		vec3 = rotate(vec3, angle, X_AXIS);
		vec3 = rotate(vec3, angle, Y_AXIS);
		vec3 = rotate(vec3, angle, Z_AXIS);

		//Translate From Camera  => !!AFTER ROTATION
		vec3.z -= camera_position.z; 

		vec3 = get_projection(vec3);

		draw_rect(vec3.x, vec3.y, 4, 4, BLUE);
	}
}

//=========================================================
// PRIVATE FUNCTIONS
//=========================================================
static void set_dimensions()
{
	originX = WINDOW_WIDTH / 2;
	originY = WINDOW_HEIGHT / 2;
}

static bool init_window()
{
	//init window
	int success = SDL_Init(SDL_INIT_EVERYTHING);
	if (success != 0)
	{
		fprintf(stderr, "Error initializing SDL\n");
		return false;
	}

	//create window
	window = SDL_CreateWindow(
		"Graphics",
		SDL_WINDOWPOS_CENTERED, //X
		SDL_WINDOWPOS_CENTERED, //Y
		WINDOW_WIDTH, //width
		WINDOW_HEIGHT, //height
		SDL_WINDOW_RESIZABLE
	);
	if (!window)
	{
		fprintf(stderr, "Error creating window\n");
		return false;
	}

	//create SDL renderer
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		fprintf(stderr, "Error creating renderer\n");
		return false;
	}

	return true;
}

static bool init_buffers(void)
{
	buffer = (uint32_t*)malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);

	if (!buffer)
	{
		running = false;
		printf("Failed to allocate memory for pixel buffer\n");
		return false;
	}

	buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ABGR8888,
		SDL_TEXTUREACCESS_STREAMING,
		WINDOW_WIDTH,
		WINDOW_HEIGHT
	);

	if (!buffer_texture)
	{
		printf("Failed to create buffer texture\n");
		return false;
	}

	return true;
}

static void init_objects()
{
	cube = create_cube(0, 0, 0, 2, .25);
}

static void init_3d()
{
	camera_position.x = 0;
	camera_position.y = 0;
	camera_position.z = CAMERA_Z_POS;
}