#pragma once
#include "vector.h"
#include <stdint.h>

typedef struct
{
	vec2_t points[3];

} triangle_t;

void fill_triangle(triangle_t triangle, uint32_t color);