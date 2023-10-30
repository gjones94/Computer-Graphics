#pragma once
#include <stdbool.h>
#include <SDL.h>
#include "colors.h"
#include "vector.h"
#include "mesh.h"

//=========================================================
// CONFIGURATIONS
//=========================================================
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define COLOR_BACKGROUND BLACK
#define FOV 700
#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

//=========================================================
// PUBLIC VARIABLES
//=========================================================
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* buffer_texture;
extern uint32_t* buffer;
extern vec3_t camera_position; 
extern int originX;
extern int originY;
extern bool running;
extern int previous_frame_time;

//View Options
extern bool backface_culling_enabled;
extern bool normal_enabled;
extern bool wireframe_enabled;
extern bool fill_enabled;

//=========================================================
// FUNCTION PROTOTYPES
//=========================================================
bool init_graphics();

/// <summary>
/// Add mesh to be displayed
/// </summary>
/// <param name="mesh"></param>
void add_mesh(mesh_t* mesh_to_add);

/// <summary>
/// Draw grid on window
/// </summary>
void draw_grid(unsigned int spacing, uint32_t grid_color);

/// <summary>
/// Draw Rectangle on window
/// </summary>
void draw_rect(float x, float y, int width, int height, uint32_t color);

/// <summary>
/// Draw pixel on window
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="color"></param>
void draw_pixel(int x, int y, uint32_t color);

/// <summary>
/// DDA Algorithm (Digital Differential Analyzer)
/// </summary>
void draw_line(int x1, int y1, int x2, int y2, uint32_t color);

/// <summary>
/// Draw a triangle
/// </summary>
void draw_triangle(triangle_t triangle, uint32_t color);

/// <summary>
/// Draw triangle and fill with color
/// </summary>
void fill_triangle(triangle_t triangle, uint32_t color);

/// <summary>
/// Draws a normal vector relative to the face
/// </summary>
void draw_normal(vec3_t normal, vec3_t* face_vertices, int num_vertices, uint32_t color);

/// <summary>
/// Update objects to render
/// </summary>
void update();

/// <summary>
/// Project perspective and render buffer onto display
/// </summary>
void render();

/// <summary>
/// Obtain x coordinate center of window
/// </summary>
int get_origin_x();

/// <summary>
/// Obtain y coordinate center of window
/// </summary>
int get_origin_y();

/// <summary>
/// Frees allocated memory used by graphics
/// </summary>
void free_resources();
