#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "cube.h"
#include "graphics.h"
#include "vector.h"

#define NUM_DIMENSIONS 3.0f
vec3d_t* vertices;

cube3d_t* create_cube(float origin_x, float origin_y, float origin_z, float size, float point_spacing)
{
	cube3d_t* cube = (cube3d_t*) malloc(sizeof(cube3d_t));
	if (cube != NULL)
	{
		float points_per_dimension = (size / point_spacing) + 1; //(include start index)
		int num_points = (int) pow(points_per_dimension, NUM_DIMENSIONS);

		vertices = (vec3d_t*) malloc((size_t)(sizeof(vec3d_t) * num_points));
		if (vertices != NULL)
		{
			int current_vertex = 0;

			float start_x = origin_x - (size / 2);
			float start_y = origin_y - (size / 2);
			float start_z = origin_z - (size / 2);
			float end_x = start_x + size;
			float end_y = start_y + size;
			float end_z = start_z + size;

			for (float x = start_x; x <= end_x; x += point_spacing)
			{
				for (float y = start_y; y <= end_y; y += point_spacing)
				{
					for (float z = start_z; z <= end_z; z += point_spacing)
					{
						vec3d_t vector = { x, y, z };
						vertices[current_vertex++] = vector;
					}
				}
			}
		}
		cube->num_vertices = num_points;
		cube->vertices = vertices;
	}

	return cube;
}

void rotate_cube(cube3d_t* cube, float angle, Axis axis)
{
	for (int i = 0; i < cube->num_vertices; i++)
	{
		cube->vertices[i] = rotate(cube->vertices[i], angle, axis);
	}
}

