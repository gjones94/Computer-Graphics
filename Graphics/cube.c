#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "cube.h"

#define NUM_PLANES 3.0f
vec3d_t* vertices;

cube3d_t* create_cube(float origin_x, float origin_y, float origin_z, float size, float point_density)
{
	cube3d_t* cube = (cube3d_t*) malloc(sizeof(cube3d_t));
	if (cube != NULL)
	{
		float points_per_plane = size / point_density;
		float num_points = pow(points_per_plane, NUM_PLANES);

		vertices = (vec3d_t*)malloc((size_t)(sizeof(vec3d_t) * num_points));
		if (vertices != NULL)
		{
			int current_vertex = 0;

			float start_x = origin_x - (size / 2);
			float start_y = origin_y - (size / 2);
			float start_z = origin_z - (size / 2);
			float end_x = start_x + size;
			float end_y = start_y + size;
			float end_z = start_z + size;

			for (float x = start_x; x < end_x; x += point_density)
			{
				for (float y = start_y; y < end_y; y += point_density)
				{
					for (float z = start_z; z < end_z; z += point_density)
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

void print_vertices(vec3d_t* points, int num_points)
{
	for (int i = 0; i < num_points; i++)
	{
		print_vertex(points[i]);
	}
}

void print_vertex(vec3d_t point)
{
	printf("\n");
	printf("X: %f\tY: %f\tZ: %f\n", point.x, point.y, point.z);
	printf("\n");
}