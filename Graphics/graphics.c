#include <stdio.h>
#include "graphics.h"
#include "config.h"
#include "vector.h"
#include "cube.h"
#include "utils.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"

//=========================================================
// SDL INITIALIZATION
//=========================================================
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* buffer_texture = NULL;
uint32_t* buffer = NULL;

//cube3d_t* cube = NULL;
vec3d_t camera_position; 
float angle = 0.0f;
float const angle_increment = 0.01f;
int originX = 0;
int originY = 0;
bool running = false;
int previous_frame_time = 0;

triangle_t* triangles_to_render;
vec3d_t* vertices_to_draw;
int num_vertices_to_draw;

//=========================================================
// PRIVATE FUNCTION PROTOTYPES
//=========================================================
static void set_dimensions();
static bool init_window();
static bool init_buffers(void);
static void init_camera();
static void throttle_fps(); //delay drawing to match desired FPS
static void render_texture(); // Update texture with buffer and copy texture to renderer
static void clear_buffer(uint32_t color);
static void free_resources();

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

	init_camera();

	return success;
}

void load_vertices(vec3d_t* vertices, int num_vertices)
{
	vertices_to_draw = vertices;
	num_vertices_to_draw = num_vertices;
}

void update()
{
	// Update Animation Rotation
	angle += (float) angle_increment;
	if (angle >= 360)
	{
		angle = 0.0f;
	}

	// Update Triangle Mesh with new rotation
	triangles_to_render = NULL;
	for (int i = 0; i < N_CUBE_MESH_FACES; i++)
	{
		vec3d_t face_vertices[3];

		// Get each vertex for face
		face_vertices[0] = cube_vertices[cube_faces[i].a];
		face_vertices[1] = cube_vertices[cube_faces[i].b];
		face_vertices[2] = cube_vertices[cube_faces[i].c];

		triangle_t projected_triangle;
		vec3d_t transformed_vertex;
		for (int j = 0; j < 3; j++)
		{
			// Rotate each vertex in face
			transformed_vertex = rotate(face_vertices[j], angle, X_AXIS);
			transformed_vertex = rotate(transformed_vertex, angle, Y_AXIS);
			transformed_vertex = rotate(transformed_vertex, angle, Z_AXIS);

			// Move point away from camera
			transformed_vertex.z -= camera_position.z;

			// Project point into 2d space
			vec2d_t projected_vertex = project_2d(transformed_vertex, FOV);

			// Translate vertext relative to origin
			projected_vertex.x += get_origin_x();
			projected_vertex.y += get_origin_y();

			// Save triangle mesh with projected points
			projected_triangle.points[j] = projected_vertex;
		}

		// Add triangle for rendering
		array_push(triangles_to_render, projected_triangle);
	}
}

void render()
{
	throttle_fps();

	int size = array_length(triangles_to_render);
	for (int i = 0; i < size; i++)
	{
		draw_triangle(triangles_to_render[i], BLUE);
	}

	render_texture();
	clear_buffer(0xFF000000);

	SDL_RenderPresent(renderer);
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

void draw_line(int x1, int y1, int x2, int y2, uint32_t color)
{
	int x_distance = x2 - x1;
	int y_distance = y2 - y1;

	int side_length = abs(x_distance) > abs(y_distance) ? abs(x_distance) : abs(y_distance);

	float x_increment = x_distance / (float) side_length; //1 if x is the side_length OR rise/run if y is the side_length
	float y_increment = y_distance / (float) side_length; //rise/run if x is the side_length OR 1 if y is the side_length

	float current_x = x1;
	float current_y = y1;
	for (int i = 0; i < side_length; i++)
	{
		draw_pixel((int) round(current_x), (int) round(current_y), color);
		current_x += x_increment;
		current_y += y_increment;
	}
}

void draw_rect(float x, float y, int width, int height, uint32_t color)
{
	int int_x = (int) x;
	int int_y = (int) y;

	for (int i = int_y; i < int_y + height; i++)
	{
		for (int j = int_x; j < int_x + width; j++)
		{
			draw_pixel(j, i, color);
		}
	}
}

void draw_triangle(triangle_t triangle, uint32_t color)
{
	draw_line(
		(int)triangle.points[0].x,
		(int)triangle.points[0].y,
		(int)triangle.points[1].x,
		(int)triangle.points[1].y,
		color
	);

	draw_line(
		(int)triangle.points[1].x,
		(int)triangle.points[1].y,
		(int)triangle.points[2].x,
		(int)triangle.points[2].y,
		color
	);

	draw_line(
		(int)triangle.points[2].x,
		(int)triangle.points[2].y,
		(int)triangle.points[0].x,
		(int)triangle.points[0].y,
		color
	);
}


int get_origin_x()
{
	return originX;
}

int get_origin_y()
{
	return originY;
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

static void init_camera()
{
	camera_position.x = 0;
	camera_position.y = 0;
	camera_position.z = CAMERA_Z_POS;
}

static void throttle_fps()
{
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
	{
		SDL_Delay(time_to_wait);
	}
	previous_frame_time = SDL_GetTicks();
}

static void render_texture()
{
	SDL_UpdateTexture(buffer_texture, NULL, buffer, ((int)sizeof(uint32_t) * WINDOW_WIDTH));
	SDL_RenderCopy(renderer, buffer_texture, NULL, NULL);
}

static void clear_buffer(uint32_t color)
{
	for (int i = 0; i < WINDOW_HEIGHT; i++)
	{
		for (int j = 0; j < WINDOW_WIDTH; j++)
		{
			buffer[(i * WINDOW_WIDTH) + j] = color;
		}
	}
}

static void free_resources()
{
	free(buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}