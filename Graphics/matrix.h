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
mat4_t m_identity(void);

/// <summary>
/// Get Scale Matrix
/// </summary>
/// <param name="sx: Amount to scale X"></param>
/// <param name="sy: Amount to scale Y"></param>
/// <param name="sz: Amount to scale Z"></param>
/// <returns></returns>
mat4_t m_scale(float sx, float sy, float sz);


/// <summary>
/// Transforms vector by mutiplying against matrix
/// </summary>
/// <param name="vector: Vector to transform"></param>
/// <param name="matrix: Matrix to multiply"></param>
/// <returns>vec4_t transformed vector</returns>
vec4_t m_transform(vec4_t vector, mat4_t matrix);