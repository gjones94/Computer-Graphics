#pragma once

typedef enum Axis
{
	X_AXIS,
	Y_AXIS,
	Z_AXIS

} Axis;

//=========================================================
// STRUCTS
//=========================================================
typedef struct
{
	float x;
	float y;

} vec2d_t;

typedef struct
{
	float x;
	float y;
	float z;

} vec3d_t;

//=========================================================
// PROTOTYPES
//=========================================================
void print_vertices(vec3d_t* points, int num_points);
void print_vertex(vec3d_t point);
vec3d_t rotate(vec3d_t vector, float angle, Axis axis);
