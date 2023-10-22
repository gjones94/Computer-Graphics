#pragma once
#include <stdbool.h>
#include "vector.h"
#include <SDL.h>

//=========================================================
// CONFIGURATIONS
//=========================================================
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define COLOR_BACKGROUND 0xFF000000
#define CAMERA_Z_POS -3

//=========================================================
// PUBLIC VARIABLES
//=========================================================
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* buffer_texture;
extern uint32_t* buffer;
extern int originX;
extern int originY;
extern bool running;

//=========================================================
// FUNCTION PROTOTYPES
//=========================================================
bool init_graphics();

/// <summary>
/// Draw grid on window
/// </summary>
void draw_grid(unsigned int spacing, uint32_t grid_color);

/// <summary>
/// Draw pixel on window
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="color"></param>
void draw_pixel(int x, int y, uint32_t color);

/// <summary>
/// Draw Rectangle on window
/// </summary>
void draw_rect(int x, int y, int width, int height, uint32_t grid_color);

vec3d_t get_projection(vec3d_t vector);

vec3d_t rotate(vec3d_t vector, float angle, Axis axis);

/// <summary>
/// Clear the buffer
/// </summary>
/// <param name="color"></param>
void clear_buffer(uint32_t color);

/// <summary>
/// Update texture with buffer and copy texture to renderer
/// </summary>
void render_texture();

/// <summary>
/// Display rendered buffer
/// </summary>
void render();

/// <summary>
/// Free allocated resources
/// </summary>
void free_resources();