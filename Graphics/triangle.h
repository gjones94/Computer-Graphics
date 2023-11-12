#pragma once
#include "vector.h"
#include <stdint.h>
#include "texture.h"

typedef struct
{
	vec2_t points[3];
	text2_t texture_points[3];
	uint32_t color;
	float avg_depth;
	normal_t surface_normal;
	bool draw_normal;

} triangle_t;

void fill_triangle(triangle_t triangle, uint32_t color);
void sort_by_depth(triangle_t* triangles, int num_triangles);
