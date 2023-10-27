#include <math.h>
#include <stdio.h>
#include "vector.h"

void print_vertices(vec3_t* points, int num_points)
{
	for (int i = 0; i < num_points; i++)
	{
		print_vertex(points[i]);
	}
}

void print_vertex(vec3_t point)
{
	printf("\n");
	printf("X: %f\tY: %f\tZ: %f\n", point.x, point.y, point.z);
	printf("\n");
}

float vec2_length(vec2_t v)
{
	return sqrtf(powf(v.x, 2) + powf(v.y, 2));
}

vec2_t vec2_add(vec2_t a, vec2_t b)
{
	vec2_t result = { a.x + b.x, a.y + b.y };
	return result;
}

vec2_t vec2_subtract(vec2_t a, vec2_t b)
{
	vec2_t result = { a.x - b.x, a.y - b.y };
	return result;
}

vec2_t vec2_multiply(vec2_t a, float scale_factor)
{
	vec2_t result = { a.x * scale_factor, a.y * scale_factor };
	return result;
}

vec2_t vec2_divide(vec2_t a, float scale_factor)
{
	vec2_t result = { a.x / scale_factor, a.y / scale_factor };
	return result;
}

float vec2_dot(vec2_t a, vec2_t b)
{
	return (a.x * b.x) + (a.y * b.y);
}

void vec2_normalize(vec2_t* v)
{
	float length = vec2_length(*v);

	v->x /= length;
	v->y /= length;
}


/// <summary>
/// Projects a 3d vector into 2d vector
/// </summary>
/// <param name="vector"></param>
/// <param name="FOV"></param>
/// <returns></returns>
vec2_t project_2d(vec3_t vector, int FOV)
{
	vec2_t projected_point =
	{
		.x = (vector.x * FOV) / vector.z,
		.y = (vector.y * FOV) / vector.z,
	};

	return projected_point;
}

vec3_t rotate(vec3_t vector, float angle, Axis axis)
{
	float x = vector.x;
	float y = vector.y;
	float z = vector.z;

	vec3_t vectorRotated = { .x = x, .y = y, .z = z };

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

float vec3_length(vec3_t v)
{
	return sqrtf(powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2));
}

vec3_t vec3_add(vec3_t a, vec3_t b)
{
	vec3_t result = { a.x + b.x, a.y + b.y, a.z + b.z };
	return result;
}

vec3_t get_center_vertex(vec3_t* vectors, int length)
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	for (int i = 0; i < length; i++)
	{
		x += vectors[i].x;
		y += vectors[i].y;
		z += vectors[i].z;
	}

	vec3_t centered_vertex = { .x = x / length, .y = y / length, .z = z / length };
	return centered_vertex;
}

vec3_t vec3_subtract(vec3_t target, vec3_t source)
{
	vec3_t result = { target.x - source.x, target.y - source.y, target.z - source.z };
	return result;
}

vec3_t vec3_multiply(vec3_t a, float scale_factor)
{
	vec3_t result = { a.x * scale_factor, a.y * scale_factor, a.z * scale_factor };
	return result;
}

vec3_t vec3_divide(vec3_t a, float scale_factor)
{
	vec3_t result = { a.x / scale_factor, a.y / scale_factor, a.z / scale_factor };
	return result;
}

vec3_t vec3_cross(vec3_t a, vec3_t b)
{
	vec3_t cross_product = {
		.x = a.y * b.z - a.z * b.y,
		.y = a.z * b.x - a.x * b.z,
		.z = a.x * b.y - a.y * b.x
	};

	return cross_product;
}

float vec3_dot(vec3_t a, vec3_t b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

void vec3_normalize(vec3_t *v)
{
	float length = vec3_length(*v);

	v->x /= (10 * length);
	v->y /= (10 * length);
	v->z /= (10 * length);
}