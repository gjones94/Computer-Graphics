#include <math.h>
#include <stdio.h>
#include "vector.h"

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


vec3d_t rotate(vec3d_t vector, float angle, Axis axis)
{
	float x = vector.x;
	float y = vector.y;
	float z = vector.z;

	vec3d_t vectorRotated = { .x = x, .y = y, .z = z };

	switch (axis)
	{
	case X_AXIS:
		vectorRotated.y = (y * cosf(angle)) - (z * sinf(angle));
		vectorRotated.z = (z * cosf(angle)) + (y * sinf(angle));
		break;
	case Y_AXIS:
		vectorRotated.x = (x * cosf(angle)) - (z * sinf(angle));
		vectorRotated.z = (z * cosf(angle)) + (x * sinf(angle));
		break;
	case Z_AXIS:
		vectorRotated.x = (x * cosf(angle)) - (y * sinf(angle));
		vectorRotated.y = (y * cosf(angle)) + (x * sinf(angle));
		break;
	}

	return vectorRotated;
}
