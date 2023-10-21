#pragma once
#include <stdbool.h>
#include <SDL.h>

/* ======= Properties ======= */

//Configurations
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define COLOR_BACKGROUND 0xFF000000

//SDL Objects
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* buffer_texture;
uint32_t* buffer;

int originX;
int originY;
bool running;
/* ========================== */


/* ========= Methods ======== */

bool init_graphics();

/// <summary>
/// Run graphics
/// </summary>
void start_loop();

/// <summary>
/// Initialize SDL window
/// </summary>
/// <returns></returns>
bool init_window();

/// <summary>
/// Obtain and process user input
/// </summary>
void process_input();

/// <summary>
/// Update any objects during loop
/// </summary>
void update();

/// <summary>
/// Draw grid on window
/// </summary>
void draw_grid(uint32_t grid_color);

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

/* ========================== */