#pragma once
#include "vector.h"
#include <stdint.h>
#include "texture.h"

typedef struct
{
	vec2_t vertices[3];
	text2_t texture_coordinates[3];
	uint32_t color;
	float avg_depth;
	normal_t surface_normal;
	bool draw_normal;

} triangle_t;

void swap_triangles(triangle_t* triangle1, triangle_t* triangle2);
void fill_triangle(triangle_t triangle, uint32_t color);
void fill_textured_triangle(triangle_t triangle, uint32_t* texture);
void draw_texel(int x, int y, vec2_t a, vec2_t b, vec2_t c, float u0, float v0, float u1, float v1, float u2, float v2, uint32_t* texture);
void sort_by_depth(triangle_t* triangles, int num_triangles);
