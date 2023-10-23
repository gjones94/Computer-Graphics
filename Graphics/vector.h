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

} vec2_t;

typedef struct
{
	float x;
	float y;
	float z;

} vec3_t;

//=========================================================
// PROTOTYPES
//=========================================================
void print_vertices(vec3_t* points, int num_points);
void print_vertex(vec3_t point);
vec3_t rotate(vec3_t vector, float angle, Axis axis);
vec2_t project_2d(vec3_t vector, int FOV);
