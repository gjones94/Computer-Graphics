#include "matrix.h"
#include "vector.h"

mat4_t m_identity(void)
{
	mat4_t matrix = {
		.m = {
			{ 1, 0, 0, 0 },
			{ 1, 0, 0, 0 },
			{ 1, 0, 0, 0 },
			{ 1, 0, 0, 0 }
		}
	};

	return matrix;
}

mat4_t m_scale(float sx, float sy, float sz)
{
	mat4_t matrix = {
		.m = {
			{sx, 0, 0, 0},
			{0, sy, 0, 0},
			{0, 0, sz, 0},
			{0, 0, 0,  1},
		}
	};

	return matrix;
}

vec4_t m_transform(vec4_t vector, mat4_t matrix)
{
	vector.x = (vector.x * matrix.m[0][0]) + (vector.x * matrix.m[0][1]) + (vector.x * matrix.m[0][2]) + (vector.x * matrix.m[0][3]);
	vector.y = (vector.y * matrix.m[1][0]) + (vector.y * matrix.m[1][1]) + (vector.y * matrix.m[1][2]) + (vector.y * matrix.m[1][3]);
	vector.z = (vector.z * matrix.m[2][0]) + (vector.z * matrix.m[2][1]) + (vector.z * matrix.m[2][2]) + (vector.z * matrix.m[2][3]);
	vector.w = (vector.w * matrix.m[3][0]) + (vector.w * matrix.m[3][1]) + (vector.w * matrix.m[3][2]) + (vector.w * matrix.m[3][3]);

	return vector;
}