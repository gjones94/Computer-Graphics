#pragma once
#include "vector.h"

typedef struct
{
	vec3_t* vertices;
	int num_vertices;

} cube3d_t;

vec3_t* vertices;
cube3d_t* create_cube(float origin_x, float origin_y, float origin_z, float size);
void rotate_cube(cube3d_t* cube, float angle, Axis axis);

