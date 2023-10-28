#pragma once
#include "vector.h"
#include <stdint.h>

typedef struct
{
	vec2_t points[3];

} triangle_t;

void fill_triangle(triangle_t triangle, uint32_t color);
/// <summary>
/// Sorts vertices from least to greatest by the y coordinate
/// </summary>
/// <param name="vertices"></param>
void sort_triangle_vertices(vec2_t* vertices);


/// <summary>
/// Fill flat bottom triangle
/// </summary>
/// <param name="v0"></param>
/// <param name="v1"></param>
/// <param name="m"> => m.y = v1.y, (solve for m.x)</param>
void fill_flat_bottom_triangle(vec2_t v0, vec2_t v1, vec2_t m, uint32_t color);

/// <summary>
/// Fill flat top triangle
/// </summary>
/// <param name="v1"></param>
/// <param name="m"> => m.y = v1.y, (solve for m.x)</param>
/// <param name="v2"></param>
void fill_flat_top_triangle(vec2_t v1, vec2_t m, vec2_t v2, uint32_t color);

void swap(vec2_t* v1, vec2_t* v2);
