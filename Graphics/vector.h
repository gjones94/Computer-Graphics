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
// DIAGNOSTICS
//=========================================================
void print_vertices(vec3_t* points, int num_points);
void print_vertex(vec3_t point);

//=========================================================
// VECTOR 2D
//=========================================================
vec2_t project_2d(vec3_t vector, int FOV);
float vec2_length(vec2_t v);
vec2_t vec2_add(vec2_t a, vec2_t b);
vec2_t vec2_subtract(vec2_t a, vec2_t b);
vec2_t vec2_multiply(vec2_t a, float scale_factor);
vec2_t vec2_divide(vec2_t a, float scale_factor);
float vec2_dot(vec2_t a, vec2_t b);

//=========================================================
// VECTOR 3D
//=========================================================
vec3_t rotate(vec3_t vector, float angle, Axis axis);
float vec3_length(vec3_t v);
vec3_t vec3_add(vec3_t a, vec3_t b);


/// <summary>
/// Subtracts one vector from another.
/// A vector from A->B is calculated B-A, essentially, you are translating 
/// the vector starting point (A), back to the origin. 
/// This results in one vector (B) where the start is no longer at A, but at the origin
/// </summary>
/// <param name="target"> The arrowhead of the Vector </param>
/// <param name="source"> The tail of the Vector </param>
/// <returns> Source->Target (Target - Source)</returns>
vec3_t vec3_subtract(vec3_t target, vec3_t source);
vec3_t vec3_multiply(vec3_t a, float scale_factor);
vec3_t vec3_divide(vec3_t a, float scale_factor);

/// <summary>
/// Returns a vector perpindicular to both a and b
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <remark>
/// ORDER MATTERS (go in clockwise order)
/// </remark>
/// <returns></returns>
vec3_t vec3_cross(vec3_t a, vec3_t b); 

/// <summary>
/// Returns scalar value indicating the direction similarity of two vectors
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <remark>
/// 0 == (Perpindicular e.g.  |__ --> Camera)
/// Negative == (Face is in opposite direction e.g. \__ --> Camera)
/// Positive == (Face is pointing in same general direction e.g. /__  --> Camera)
/// </remark>
/// <returns></returns>
float vec3_dot(vec3_t a, vec3_t b);
