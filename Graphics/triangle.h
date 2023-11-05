#pragma once
#include "vector.h"
#include <stdint.h>

typedef struct
{
	vec2_t start;
	vec2_t end;

} normal_t;

typedef struct
{
	vec2_t points[3];
	uint32_t color;
	float avg_depth;
	normal_t normal;

} triangle_t;

void fill_triangle(triangle_t triangle, uint32_t color);