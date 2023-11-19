#pragma once
#include "vector.h"
#include <stdint.h>
#include "texture.h"

typedef struct
{
	vec4_t vertices[3];
	text2_t texture_coordinates[3];
	uint32_t color;
	float avg_depth;
	normal_t surface_normal;
	bool draw_normal;

} triangle_t;

void swap_triangles(triangle_t* triangle1, triangle_t* triangle2);
void fill_triangle(triangle_t triangle, uint32_t color);
void fill_textured_triangle(triangle_t triangle, uint32_t* texture);
void draw_texel(int x, int y, triangle_t triangle, uint32_t* texture);
void draw_triangle_pixel(int x, int y, triangle_t triangle, uint32_t color);
void sort_by_depth(triangle_t* triangles, int num_triangles);
