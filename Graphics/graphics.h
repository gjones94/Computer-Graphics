#pragma once
#include <stdbool.h>
#include <SDL.h>
#include "vector.h"
#include "mesh.h"

//=========================================================
// CONFIGURATIONS
//=========================================================
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define COLOR_BACKGROUND 0xFF000000
#define CAMERA_Z_POS -3
#define FOV 800
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

//=========================================================
// FUNCTION PROTOTYPES
//=========================================================
bool init_graphics();

/// <summary>
/// loads vertices for drawing onto buffer
/// </summary>
/// <param name="vertices"></param>
/// <param name="num_vertices"></param>
void load_vertices(vec3_t* vertices, int num_vertices);

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
/// draw a triangle
/// </summary>
void draw_triangle(triangle_t triangle, uint32_t color);

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
