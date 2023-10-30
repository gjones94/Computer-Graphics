#include <stdio.h>
#include "graphics.h"
#include "colors.h"
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

vec3_t camera_position; 
triangle_t* triangles_to_render;
float const angle_increment = 0.005f;
int originX = 0;
int originY = 0;
bool running = false;
int previous_frame_time = 0;

bool backface_culling_enabled;
bool wireframe_enabled;
bool fill_enabled;

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
static void destroy_window();

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

void update()
{
	triangles_to_render = NULL;

	for (int m = 0; m < num_meshes; m++)
	{
		mesh_t* mesh = meshes[m];
		int num_faces = mesh->num_faces;

		for (int i = 0; i < num_faces; i++)
		{
			face_t mesh_face = mesh->faces[i];

			// Get each vertex for face
			vec3_t face_vertices[3];

			// face stores vertex index in vertices array
			// vertex index references start with 1 instead of 0, need to get to zero-based index by subtracting 1
			face_vertices[0] = mesh->vertices[mesh_face.a - 1];
			face_vertices[1] = mesh->vertices[mesh_face.b - 1];
			face_vertices[2] = mesh->vertices[mesh_face.c - 1];

			triangle_t projected_triangle;
			vec3_t transformed_vertices[3];

			// Change World Position
			for (int j = 0; j < 3; j++)
			{
				vec3_t transformed_vertex;
				transformed_vertex = rotate(face_vertices[j], mesh->rotation.x, X_AXIS);
				transformed_vertex = rotate(transformed_vertex, mesh->rotation.y, Y_AXIS);
				transformed_vertex = rotate(transformed_vertex, mesh->rotation.z, Z_AXIS);

				// Move point away from camera
				transformed_vertex.z += 5; // move point away from origin
				transformed_vertices[j] = transformed_vertex;
			}

			/*
			==============================================================================================
				 Backface Culling
						 N
						 |
						 a
						/|\
					   / | \
					  c  |  b
						 |
					  camera

				1. Get A->B = B-A
				2. Get A->C = C-A
				3. Get A->Camera = Camera - A
				4. Get Cross Product (AB X AC) = Perpindicular Normal at A
				5. Get Dot Product of N and A->Camera
				6. Evaluate if Normal is at all facing camera or not
			==============================================================================================
			*/

			vec3_t c = transformed_vertices[2]; /*  b   c  */ // b (index) --> c (middle) = CLOCKWISE
			vec3_t b = transformed_vertices[1]; /*   \ /   */
			vec3_t a = transformed_vertices[0]; /*    a    */ // Thumb toward you

			// Get vector AB and vector AC
			vec3_t ab = vec3_subtract(b, a); // b - a
			vec3_t ac = vec3_subtract(c, a); // c - a

			// Get Vertex A to Camera Vector: A->Camera
			vec3_t camera_ray = vec3_subtract(camera_position, a); // camera - a

			// Get Normal Vector of Face/Surface of the mesh
			vec3_t normal = vec3_cross(ab, ac);
			vec3_normalize(&normal);

			// Determine if the Face/Surface Normal is pointing in the same general direction as the ray to the camera
			// Dot product returns a value >= 0 if the surface is facing the camera, otherwise it is < 0
			float dot_product = vec3_dot(normal, camera_ray);
			if (dot_product < 0)
			{
				// Do not show faces that are not facing the camera
				continue;
			}

			// Project and translate to screen coordinates
			for (int j = 0; j < 3; j++)
			{
				// Project into 2d space
				vec2_t projected_vertex = project_2d(transformed_vertices[j], FOV);

				// Translate vertex relative to origin
				projected_vertex.x += get_origin_x();
				projected_vertex.y += get_origin_y();


				// Save triangle mesh with projected points
				projected_triangle.points[j] = projected_vertex;
			}

			//For Debugging / Fun
			draw_normal(normal, transformed_vertices, 3, BLUE);

			// Add triangle for rendering
			array_push(triangles_to_render, projected_triangle);
		}
	}
}

void render()
{
	throttle_fps();

	int size = array_length(triangles_to_render);

	for (int i = 0; i < size; i++)
	{
		fill_triangle(triangles_to_render[i], BLUE);
		draw_triangle(triangles_to_render[i], WHITE);
	}

	array_free(triangles_to_render);

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

	float current_x = (float) x1;
	float current_y = (float) y1;
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
		(int) triangle.points[0].x,
		(int) triangle.points[0].y,
		(int) triangle.points[1].x,
		(int) triangle.points[1].y,
		color
	);

	draw_line(
		(int) triangle.points[1].x,
		(int) triangle.points[1].y,
		(int) triangle.points[2].x,
		(int) triangle.points[2].y,
		color
	);

	draw_line(
		(int) triangle.points[2].x,
		(int) triangle.points[2].y,
		(int) triangle.points[0].x,
		(int) triangle.points[0].y,
		color
	);
}

void draw_normal(vec3_t normal, vec3_t* face_vertices, int num_vertices, uint32_t color)
{
	// Get center of face
	vec3_t face_center = get_center_vertex(face_vertices, num_vertices);

	// Position normal relative to a vertex (normal is currently relative to origin)
	normal = vec3_add(face_center, normal); 

	// Project normal and vertex points into 2d space
	vec2_t projected_normal = project_2d(normal, FOV);
	vec2_t projected_face_center = project_2d(face_center, FOV);

	// Center projected point onto screen coordinates
	projected_face_center.x += get_origin_x();
	projected_face_center.y += get_origin_y();
	projected_normal.x += get_origin_x();
	projected_normal.y += get_origin_y();

	draw_line((int)projected_face_center.x, (int)projected_face_center.y, (int)projected_normal.x, (int)projected_normal.y, BLUE);
}

int get_origin_x()
{
	return originX;
}

int get_origin_y()
{
	return originY;
}

void free_resources()
{
	free_meshes();

	free(buffer);
	destroy_window();
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
	camera_position.z = 0;
}

/// <summary>
/// Waits necessary time to achieve target frame rate
/// </summary>
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

static void destroy_window()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}