#include <math.h>
#include <stdio.h>
#include "vector.h"

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

vec3_t get_normal(vec3_t a, vec3_t b, vec3_t c)
{
	// (Left hand coordinate system)
	// 
	//  b -> c  b (index) --> c (middle) = CLOCKWISE
    //   \ /   
	//    a     Thumb toward you
	// 
	// 1. Get A->B = B - A
	// 2. Get A->C = C - A
	// 3. Get Cross Product(AB X AC) = Perpindicular Normal at A

	vec3_t ab = vec3_subtract(b, a); 
	vec3_t ac = vec3_subtract(c, a);

	// Get Normal Vector of Face/Surface of the mesh
	vec3_t normal = vec3_cross(ab, ac); // clockwise (ab then ac)

	// Scale normal to unit length of 1
	vec3_normalize(&normal);

	return normal;
}


void vec3_normalize(vec3_t *v)
{
	float length = vec3_length(*v);

	v->x /= length;
	v->y /= length;
	v->z /= length;
}

vec3_t vec3_from_vec4(vec4_t vector)
{
	vec3_t v = { .x = vector.x, .y = vector.y, .z = vector.z };
	return v;
}

vec3_t get_center_vertex(vec3_t a, vec3_t b, vec3_t c)
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	x += a.x + b.x + c.x;
	y += a.y + b.y + c.y;
	z += a.z + b.z + c.z;

	x /= 3.0f;
	y /= 3.0f;
	z /= 3.0f;

	vec3_t centered_vertex = { .x = x, .y = y, .z = z };
	return centered_vertex;
}

vec4_t vec4_from_vec3(vec3_t vector)
{
	vec4_t v = { .x = vector.x, .y = vector.y, .z = vector.z, .w = 1 };
	return v;
}

normal_t get_normal_ray(vec3_t a, vec3_t b, vec3_t c)
{
	// Get normal and face center
	vec3_t normal = get_normal(a, b, c);
	vec3_t face_center = get_center_vertex(a, b, c);

	// Attach normal to center of face
	normal = vec3_add(normal, face_center);

	normal_t surface_normal = { .start = face_center,  .end = normal };

	return surface_normal;
}