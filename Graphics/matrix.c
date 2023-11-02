#include "matrix.h"
#include "vector.h"

mat4_t get_identity_matrix(void)
{
	mat4_t matrix = {
		.m = {
			{ 1, 0, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 1 }
		}
	};

	return matrix;
}

mat4_t get_scale_matrix(float sx, float sy, float sz)
{
	mat4_t matrix = {
		.m = {
			{ sx, 0,  0,  0 },
			{ 0,  sy, 0,  0 },
			{ 0,  0,  sz, 0 },
			{ 0,  0,  0,  1 },
		}
	};

	return matrix;
}

mat4_t get_translation_matrix(float tx, float ty, float tz)
{
	mat4_t matrix = {
		.m = {
			{ 1,  0,  0,  tx },
			{ 0,  1,  0,  ty },
			{ 0,  0,  1,  tz },
			{ 0,  0,  0,  1 },
		}
	};

	return matrix;
}

vec4_t m_transform(vec4_t vector, mat4_t matrix)
{
	vec4_t result;
	/*
		Matrix = 4 * 4
		Vector = 4 * 1	
		Thus, to multiply, it must be Matrix * Vector

		| 1 0 0 TX |		| X |		| X*1 + Y*0 + Z*0 + TX*1 |
		| 0 1 0 TY |   *	| Y |	=	| X*0 + Y*1 + Z*0 + TY*1 |
		| 0 0 1 TZ |		| Z |		| X*0 + Y*0 + Z*1 + TZ*1 |
		| 0 0 0 1  |		| 1	|		| X*0 + Y*0 + Z*0 + 1*1	 |

	*/
	result.x = (matrix.m[0][0] * vector.x) + (matrix.m[0][1] * vector.y) + (matrix.m[0][2] * vector.z) + (matrix.m[0][3] * vector.z);
	result.y = (matrix.m[1][0] * vector.x) + (matrix.m[1][1] * vector.y) + (matrix.m[1][2] * vector.z) + (matrix.m[1][3] * vector.z);
	result.z = (matrix.m[2][0] * vector.x) + (matrix.m[2][1] * vector.y) + (matrix.m[2][2] * vector.z) + (matrix.m[2][3] * vector.z);
	result.w = (matrix.m[3][0] * vector.x) + (matrix.m[3][1] * vector.y) + (matrix.m[3][2] * vector.z) + (matrix.m[3][3] * vector.z);

	return vector;
}