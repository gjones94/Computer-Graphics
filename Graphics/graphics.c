#include <stdio.h>
#include "graphics.h"
#include "colors.h"

//=========================================================
// PUBLIC VARIABLE INITIALIZATION
//=========================================================
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* buffer_texture = NULL;
uint32_t* buffer = NULL;
int originX = 0;
int originY = 0;
bool running = false;

//=========================================================
// PRIVATE FUNCTION PROTOTYPES
//=========================================================
static void set_dimensions();
static bool init_window();
static bool init_buffers(void);

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

	return success;
}

void render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	draw_grid(10, GREY);

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
	if (x < WINDOW_WIDTH && y < WINDOW_HEIGHT)
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
			draw_pixel(i, j, color);
		}
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