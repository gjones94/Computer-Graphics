#pragma once
#include "vector.h"

typedef struct
{
	vec3d_t* vertices;
	int num_vertices;

} cube3d_t;

vec3d_t* vertices;
cube3d_t* create_cube(float origin_x, float origin_y, float origin_z, float size, float point_spacing);
void rotate_cube(cube3d_t* cube, float angle, Axis axis);

