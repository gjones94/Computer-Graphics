#pragma once
#include "vector.h"

typedef struct
{
	float m[4][4];

} mat4_t;

/// <summary>
/// Get Identify Matrix
/// </summary>
/// <param name=""></param>
/// <returns></returns>
mat4_t get_identity_matrix(void);

/// <summary>
/// Get Scale Matrix
/// </summary>
/// <param name="sx: Amount to scale X"></param>
/// <param name="sy: Amount to scale Y"></param>
/// <param name="sz: Amount to scale Z"></param>
/// <returns></returns>
mat4_t get_scale_matrix(float sx, float sy, float sz);

/// <summary>
/// Get Translation Matrix
/// </summary>
/// <param name="tx: Amount to translate X"></param>
/// <param name="ty: Amount to translate Y"></param>
/// <param name="tz: Amount to translate Z"></param>
mat4_t get_translation_matrix(float tx, float ty, float tz);

/// <summary>
/// Get Rotation Matrix
/// </summary>
/// <param name="rx: Amount to rotate X"></param>
/// <param name="ry: Amount to rotate Y"></param>
/// <param name="rz: Amount to rotate Z"></param>
mat4_t get_rotation_matrix(float angle, Axis axis);

/// <summary>
/// Creates perspective matrix to adjust our x, y, and z values
/// </summary>
/// <param name="aspect">h/w ratio of screen</param>
/// <param name="fov">angle of view (in degrees)</param>
/// <param name="znear">the beginning of our viewing plane</param>
/// <param name="zfar">the end of our viewing plane</param>
/// <returns></returns>
mat4_t get_perspective_matrix(float aspect, float fov, float znear, float zfar);

/// <summary>
/// Get Combined Matrix of two matrices m1 * m2
/// </summary>
/// <param name="m1"></param>
/// <param name="m2"></param>
/// <returns></returns>
mat4_t get_combined_matrix(mat4_t m1, mat4_t m2);


/// <summary>
/// Transforms vector by mutiplying against matrix
/// </summary>
/// <param name="vector: Vector to transform"></param>
/// <param name="matrix: Matrix to multiply"></param>
/// <returns>vec4_t transformed vector</returns>
vec4_t m_transform(vec4_t vector, mat4_t matrix);

/// <summary>
/// Projects vector with projection matrix
/// </summary>
/// <param name="m_projection"></param>
/// <param name="vector"></param>
/// <returns>vec4_t projected vector</returns>
vec4_t project(mat4_t m_projection, vec4_t vector);
