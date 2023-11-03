#include <math.h>
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

mat4_t get_rotation_matrix(float angle, Axis axis)
{
	mat4_t matrix = get_identity_matrix();

	/*
		X ROTATION			
		====================================================
		X [	cos(angle),	-sin(angle),	0,	0]
		Y [	sin(angle),	 cos(angle),	0,	0]
		====================================================
	*/

	switch (axis)
	{
		case Z_AXIS:
			matrix.m[0][0] = cosf(angle);
			matrix.m[0][1] = -sinf(angle);
			matrix.m[1][0] = sinf(angle);
			matrix.m[1][1] = cosf(angle);
			// LOCK Z
			break;
		case X_AXIS:
			// LOCK X
			matrix.m[1][1] = cosf(angle);
			matrix.m[1][2] = -sinf(angle);
			matrix.m[2][1] = sinf(angle);
			matrix.m[2][2] = cosf(angle);
			break;
		case Y_AXIS:
			matrix.m[0][0] = cosf(angle);
			matrix.m[0][2] = -sinf(angle);
			// LOCK Y
			matrix.m[2][0] = sinf(angle);
			matrix.m[2][2] = cosf(angle);
			break;

	}

	return matrix;
}

mat4_t get_combined_matrix(mat4_t m1, mat4_t m2)
{
	int num_rows = 4;
	int num_columns = 4;
	mat4_t m_new = get_identity_matrix();
	for (int i = 0; i < num_rows; i++)
	{
		for (int j = 0; j < num_columns; j++)
		{
			float new_value = 0.0f;
			for (int k = 0; k < num_rows; k++)
			{
				new_value += (m1.m[i][k] * m2.m[k][j]);
			}

			m_new.m[i][j] = new_value;
		}
	}

	return m_new;

}

vec4_t m_transform(vec4_t vector, mat4_t matrix)
{
	vec4_t result;
	/*

		Matrix = 4 X 4
		Vector = 4 X 1	

		Matrix Multiplication
		-------------------------------------------------------------------------------------------
		MXN * N*P (Leftside # Columns matches Rightside # Rows, result = M * P)
		MXN * P*M (NO, Leftside # Columns != Rightside # Rows)


		Must multiply with matrix on the left side, and the vector on the right
		-------------------------------------------------------------------------------------------

		Matrix					Vector					Calculation			Result
		==============================================================================================================================
		|	1 0 0 TX	|		|	X	|		|	X*1 + Y*0 + Z*0 + TX*1	|	X + TX
		|	0 1 0 TY	|	*	|	Y	|	=	|	X*0 + Y*1 + Z*0 + TY*1	|	Y + TY
		|	0 0 1 TZ	|		|	Z	|		|	X*0 + Y*0 + Z*1 + TZ*1	|	Z + TZ
		|	0 0 0 1		|		|	1	|		|	X*0 + Y*0 + Z*0 + 1*1	|	  1
		==============================================================================================================================

	*/
	result.x = (matrix.m[0][0] * vector.x) + (matrix.m[0][1] * vector.y) + (matrix.m[0][2] * vector.z) + (matrix.m[0][3] * vector.w);
	result.y = (matrix.m[1][0] * vector.x) + (matrix.m[1][1] * vector.y) + (matrix.m[1][2] * vector.z) + (matrix.m[1][3] * vector.w);
	result.z = (matrix.m[2][0] * vector.x) + (matrix.m[2][1] * vector.y) + (matrix.m[2][2] * vector.z) + (matrix.m[2][3] * vector.w);
	result.w = (matrix.m[3][0] * vector.x) + (matrix.m[3][1] * vector.y) + (matrix.m[3][2] * vector.z) + (matrix.m[3][3] * vector.w);

	return result;
}
