#include <stdio.h>
#include "graphics.h"
#include "colors.h"
#include "light.h"
#include "vector.h"
#include "matrix.h"
#include "triangle.h"
#include "mesh.h"
#include "array.h"
#include "texture.h"
#include "utilities.h"

#define MAX_TRIANGLES 10000
//=========================================================
// SDL INITIALIZATION
//=========================================================
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* buffer_texture = NULL;
uint32_t* buffer = NULL;
float* z_buffer = NULL;

vec3_t camera_position; 
light_t light_source;
mat4_t m_perspective;

triangle_t triangles_to_render[MAX_TRIANGLES];
int num_triangles = 0;
float const angle_increment = 0.003f;
float const scale_increment = 0.002f;
float const translation_increment = 0.001f;
float aspect_ratio;
float fov;

int originX = 0;
int originY = 0;
bool running = false;
int previous_frame_time = 0;

bool backface_culling_enabled = true;
bool wireframe_enabled = true;
bool fill_enabled = false;
bool normal_enabled = false;
bool texture_enabled = false;

//=========================================================
// PRIVATE FUNCTION PROTOTYPES
//=========================================================
static void set_dimensions();
static bool init_window();
static bool init_buffers(void);
static void init_camera();
static void init_light();
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
	init_light();

	// Intialize Perspective Matrix
	fov = get_angle_radians(FOV);
	aspect_ratio = (float) WINDOW_HEIGHT / (float) WINDOW_WIDTH;
	m_perspective = get_perspective_matrix(aspect_ratio, fov, ZNEAR, ZFAR);

	return success;
}

void update()
{
	num_triangles = 0;
	// Set Next Movements for Mesh
	for (int i = 0; i < num_meshes; i++)
	{
		meshes[i]->rotation.x += angle_increment;
		//meshes[i]->rotation.y += angle_increment;
		//meshes[i]->rotation.z += angle_increment;

		//Move mesh away from origin to be in view
		//meshes[i]->translation.y = -1.5;
		//meshes[i]->scale.x = 50;
		//meshes[i]->scale.y = 50;
		//meshes[i]->scale.z = 50;
		meshes[i]->translation.z = 5;
	}

	// Apply movements for mesh using matrices
	for (int m = 0; m < num_meshes; m++)
	{
		mesh_t* mesh = meshes[m];
		int num_faces = mesh->num_faces;

		// Loop through all faces of mesh
		for (int i = 0; i < num_faces; i++)
		{
			// Get mesh face
			face_t mesh_face = mesh->faces[i];
			
			// Get vertices of face
			vec3_t face_vertices[3];
			face_vertices[0] = mesh->vertices[mesh_face.a - 1];
			face_vertices[1] = mesh->vertices[mesh_face.b - 1];
			face_vertices[2] = mesh->vertices[mesh_face.c - 1];

			// Get scale matrix
			mat4_t scale_matrix = get_scale_matrix(mesh->scale.x, mesh->scale.y, mesh->scale.z);

			// Get rotation matrix
			mat4_t rotation_matrix_x = get_rotation_matrix(mesh->rotation.x, X_AXIS);
			mat4_t rotation_matrix_y = get_rotation_matrix(mesh->rotation.y, Y_AXIS);
			mat4_t rotation_matrix_z = get_rotation_matrix(mesh->rotation.z, Z_AXIS);

			// Get translation matrix
			mat4_t translation_matrix = get_translation_matrix(mesh->translation.x, mesh->translation.y, mesh->translation.z);

			// Combine matrices into world matrix
			mat4_t world_matrix = get_identity_matrix();
			world_matrix = get_combined_matrix(scale_matrix, world_matrix);
			world_matrix = get_combined_matrix(rotation_matrix_x, world_matrix);
			world_matrix = get_combined_matrix(rotation_matrix_y, world_matrix);
			world_matrix = get_combined_matrix(rotation_matrix_z, world_matrix);
			world_matrix = get_combined_matrix(translation_matrix, world_matrix);

			// Store transformed vertices
			vec4_t transformed_vertices[3];

			// Apply tranformation of vertex using world matrix
			for (int j = 0; j < 3; j++)
			{
				vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);
				transformed_vertex = m_transform(transformed_vertex, world_matrix);
				transformed_vertices[j] = transformed_vertex;
			}

			// Apply backface culling to triangle
			vec3_t a = vec3_from_vec4(transformed_vertices[0]);
			vec3_t b = vec3_from_vec4(transformed_vertices[1]);
			vec3_t c = vec3_from_vec4(transformed_vertices[2]);

			bool crop_out_surface = cull_backface(a, b, c);
			if (crop_out_surface)
			{
				continue;
			}

			// Obtain surface normal of face
			normal_t surface_normal = get_normal_ray(a, b, c);
		
			// Get average depth for triangle for sorting render order of triangles
			float depth = (float) (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3;

			// Store projected 2d vertices as triangle
			triangle_t projected_triangle;

			// Project and translate to screen coordinates
			for (int j = 0; j < 3; j++)
			{
				// Project into 2d space
				vec4_t projected_vertex = project(m_perspective, transformed_vertices[j]);

				// Scale to Window Size (coordinates are currently normalized to  and 1)
				projected_vertex.x *= (int) WINDOW_WIDTH / 2;
				projected_vertex.y *= (int) WINDOW_HEIGHT / 2;

				// Invert y coordinate due to inverted y screen coordinate
				projected_vertex.y *= -1;

				// Translate vertex relative to origin
				projected_vertex.x += get_origin_x();
				projected_vertex.y += get_origin_y();

				// Save triangle mesh with projected points
				projected_triangle.vertices[j].x = projected_vertex.x;
				projected_triangle.vertices[j].y = projected_vertex.y;

				// Used for depth interpolation for texture mapping of triangle
				projected_triangle.vertices[j].z = projected_vertex.z;
				projected_triangle.vertices[j].w = projected_vertex.w;

				projected_triangle.avg_depth = depth;
			}
			
			// Get Light Intensity From Normal
			vec3_t normal = get_normal(a, b, c);
			float intensity = vec3_dot(normal, light_source.direction);
			uint32_t light_adjusted_color = get_shaded_color(intensity, mesh_face.color);

			projected_triangle.draw_normal = normal_enabled;

			// Add uv coordinates to triangle
			projected_triangle.texture_coordinates[0] = mesh_face.a_uv;
			projected_triangle.texture_coordinates[1] = mesh_face.b_uv;
			projected_triangle.texture_coordinates[2] = mesh_face.c_uv;

			projected_triangle.color = light_adjusted_color;
			projected_triangle.surface_normal = surface_normal;

			// Add triangle for rendering
			if(num_triangles < MAX_TRIANGLES)
				triangles_to_render[num_triangles++] = projected_triangle;
		}
	}
}

void render()
{
	throttle_fps();

	//sort_by_depth(triangles_to_render, num_triangles);

	for (int i = 0; i < num_triangles; i++)
	{
		if (fill_enabled)
		{
			fill_triangle(triangles_to_render[i], triangles_to_render[i].color);
		}
		else if (texture_enabled)
		{
			fill_textured_triangle(triangles_to_render[i], mesh_texture);
		}

		if (wireframe_enabled)
		{
			draw_triangle(triangles_to_render[i], WHITE);
		}

		if (triangles_to_render[i].draw_normal)
		{
			draw_ray(triangles_to_render[i].surface_normal, BLUE);
		}
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

	float current_x = (float) x1;
	float current_y = (float) y1;
	for (int i = 0; i <= side_length; i++)
	{
		draw_pixel((int) current_x, (int) current_y, color);
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
		(int) triangle.vertices[0].x,
		(int) triangle.vertices[0].y,
		(int) triangle.vertices[1].x,
		(int) triangle.vertices[1].y,
		color
	);

	draw_line(
		(int) triangle.vertices[1].x,
		(int) triangle.vertices[1].y,
		(int) triangle.vertices[2].x,
		(int) triangle.vertices[2].y,
		color
	);

	draw_line(
		(int) triangle.vertices[2].x,
		(int) triangle.vertices[2].y,
		(int) triangle.vertices[0].x,
		(int) triangle.vertices[0].y,
		color
	);
}

void draw_ray(normal_t ray, uint32_t color)
{
	// Convert to vec4 for projection
	vec4_t start = vec4_from_vec3(ray.start);
	vec4_t end = vec4_from_vec3(ray.end);

	// Project normal
	start = project(m_perspective, start);
	end = project(m_perspective, end);

	// Scale to screen size
	float start_x = start.x * (int) WINDOW_WIDTH / 2;
	float start_y = start.y * (int) WINDOW_HEIGHT / 2;
	float end_x = end.x * (int) WINDOW_WIDTH / 2;
	float end_y = end.y * (int) WINDOW_HEIGHT / 2;

	// Invert y coordinates to account for screen coordinate inversion of y axis
	start_y *= -1;
	end_y *= -1;

	// Center projected point onto screen coordinates
	start_x += get_origin_x();
	start_y += get_origin_y();
	end_x += get_origin_x();
	end_y += get_origin_y();

	draw_line((int)start_x, (int)start_y, (int)end_x, (int)end_y, color);
}

bool cull_backface(vec3_t a, vec3_t b, vec3_t c)
{
	/*
       ==========================
          Backface Culling
                 N
                 |
                 a
                /|\
               / | \
              c  |  b
                 |
               camera
       ==========================
	*/

	/*
	    Get Normal
		1. Get A->B = B - A
		2. Get A->C = C - A
		3. Get Cross Product(AB X AC) = Perpindicular Normal at A
	*/
	vec3_t normal = get_normal(a, b, c);

	/*
        Calculate Dot Product between ray to camera and the normal
		4. Get A->Camera = Camera - A
		5. Get Dot Product of N and A->Camera
		6. Evaluate if Normal is at facing towards the camera
	*/

	vec3_t camera_ray = vec3_subtract(camera_position, a); // camera - a
	float dot_product = vec3_dot(normal, camera_ray);

	// 
	//	A dot product >= 0 means the surface normal is facing towards the camera or is at least perpendicular to the camera
	//                       N                                      N
	//                      /__  Camera            OR               |__  camera 
	//
	if (dot_product < 0)
	{
		// Do not show faces that are not facing the camera
		if (backface_culling_enabled)
		{
			return true;
		}
	}

	return false;
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
	upng_free(png_texture);
	free(buffer);
	free(z_buffer);
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
	buffer = (uint32_t*) malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);
	z_buffer = (float*) malloc(sizeof(float) * WINDOW_WIDTH * WINDOW_HEIGHT);
	if (!buffer || !z_buffer)
	{
		running = false;
		printf("Failed to allocate memory for pixel buffer\n");
		return false;
	}

	buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA32,
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

static void init_light()
{
	// light is looking straight down
	light_source.direction.x = 0;
	light_source.direction.y = 1;
	light_source.direction.z = 0;
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
			buffer[(i * WINDOW_WIDTH) + j] = color; // Clear Color Buffer
			z_buffer[(i * WINDOW_WIDTH) + j] = 1.0f; // Clear related Z Buffer (1.0 is furthest away)
		}
	}
}

static void destroy_window()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}