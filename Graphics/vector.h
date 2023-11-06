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
	float x, y;

} vec2_t;

typedef struct
{
	float x, y, z;

} vec3_t;

typedef struct
{
	float x, y, z, w;

} vec4_t;

typedef struct
{
	vec3_t start;
	vec3_t end;

} normal_t;

//=========================================================
// VECTOR 2 FUNCTIONS
//=========================================================
vec2_t project_2d(vec3_t vector, int FOV);
float vec2_length(vec2_t v);
vec2_t vec2_add(vec2_t a, vec2_t b);
vec2_t vec2_subtract(vec2_t a, vec2_t b);
vec2_t vec2_multiply(vec2_t a, float scale_factor);
vec2_t vec2_divide(vec2_t a, float scale_factor);
float vec2_dot(vec2_t a, vec2_t b);
void vec2_normalize(vec2_t *v);

//=========================================================
// VECTOR 3 FUNCTIONS
//=========================================================

/// <summary>
/// Rotates a given vector around specified axis by the number of degrees provided
/// </summary>
/// <param name="vector"></param>
/// <param name="angle"></param>
/// <param name="axis"></param>
/// <returns>vec3_t vector with rotation applied</returns>
vec3_t rotate(vec3_t vector, float angle, Axis axis);


/// <summary>
/// Calculates the length of a vector (using pythagorean theorem)
/// </summary>
/// <param name="v"></param>
/// <returns></returns>
float vec3_length(vec3_t v);

/// <summary>
/// Adds on vector to another vector 
/// (Centers vector A relative to vector B)
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns>vec3_t A relative to B</returns>
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

/// <summary>
/// Uses the average to returns a vertex centered (averaged) between all vertices provided
/// </summary>
/// <param name="vectors"></param>
/// <param name="length"></param>
/// <returns></returns>
vec3_t get_center_vertex(vec3_t a, vec3_t b, vec3_t c);

/// <summary>
/// Multiplies a vector by a factor
/// </summary>
/// <param name="a"></param>
/// <param name="scale_factor"></param>
/// <returns>vec3_t of vector scaled up by factor</returns>
vec3_t vec3_multiply(vec3_t a, float scale_factor);

/// <summary>
/// Divides a vector by a factor
/// </summary>
/// <param name="a"></param>
/// <param name="scale_factor"></param>
/// <returns>vec3_t of vector scaled down by factor</returns>
vec3_t vec3_divide(vec3_t a, float scale_factor);

/// <summary>
/// Get the normal (perpindicular) vector between 2 vectors ab and ac (center vertex is a)
/// The vertices should be passed in a clockwise order (left hand coordinate system)
/// The vector returned is NOT centered on this vertex, it is only a calculation of 
/// the normal as if originating from origin
/// </summary>
/// <param name="a (center vertex)"></param>
/// <param name="b"></param>
/// <param name="c"></param>
/// <returns>vec3_t normal of length 1</returns>
vec3_t get_normal(vec3_t a, vec3_t b, vec3_t c);

/// <summary>
/// Returns a vector with a unit length of 1
/// </summary>
/// <param name="v"></param>
/// <returns>vec3_t normalized vector</returns>
void vec3_normalize(vec3_t* v);

/// <summary>
/// Returns a vector perpendicular to both a and b.
/// Normal vector returned is NOT relative to the vectors provided, it is centered relative to the origin.
/// To center vector relative to one of these vertices, add the normal vector returned by the function
/// to the vertex you wish to center it to
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

//=========================================================
// VECTOR CONVERSION FUNCTIONS
//=========================================================

vec3_t vec3_from_vec4(vec4_t vector);
vec4_t vec4_from_vec3(vec3_t vector);


/// <summary>
/// Returns a normal ray from start vector to end vector
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="c"></param>
/// <returns>normal_t normal ray centered on surface of vertices</returns>
normal_t get_normal_ray(vec3_t a, vec3_t b, vec3_t c);